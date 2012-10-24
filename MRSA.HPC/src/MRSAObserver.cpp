/*
 * MRSAObserver.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <map>

#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/DataSet.h"
// for timestamp
#include "repast_hpc/io.h"

#include "MRSAObserver.h"
#include "PersonsCreator.h"
#include "PlaceCreator.h"
#include "ActivityCreator.h"
#include "TransmissionAlgorithm.h"
#include "Parameters.h"
#include "Constants.h"

namespace mrsa {

using namespace repast::relogo;
using namespace repast;
using namespace std;

// counts the number of lines in the specified file.
// We need to know this because we have to tell the Observer how
// many persons to create and each line in the file is a person.
int line_count(const std::string& file) {
	int numLines = 0;
	ifstream in(file.c_str());
	if (!in.is_open())
		return -1;

	string line;
	while (getline(in, line))
		++numLines;
	in.close();

	return numLines;
}

Calendar::Calendar() :
		hour_of_day(0), day_of_week(0), day_of_year(1), year(1) {
}

void Calendar::increment() {
	++hour_of_day;
	if (hour_of_day > 23) {
		hour_of_day = 0;
		++day_of_week;
		if (day_of_week > 6) {
			day_of_week = 0;
		}
		++day_of_year;
		if (day_of_year > 365) {
			day_of_year = 1;
			++year;
		}
	}
}

bool Calendar::isWeekDay() {
	return !(day_of_week == 0 || day_of_week == 6);
}

MRSAObserver::MRSAObserver() :
personType(0), places(0), people_(0), summary_output_file(), calendar() {
}

MRSAObserver::~MRSAObserver() {
	// delete the permanent AgentSet used to hold the
	// population.
	delete people_;

	// delete each place from the list of places.
	for (vector<Place*>::iterator iter = places.begin(); iter != places.end(); ++iter) {
		delete (*iter);
	}
}

// called each tick
void MRSAObserver::go() {
	// reset the hourly stats, as they are per tick.
	Statistics* stats = Statistics::getInstance();
	stats->resetHourlyStats();
	calendar.increment();

	int tick = (int) RepastProcess::instance()->getScheduleRunner().currentTick();
	// print out time stamp each simulated month
	// on process 0
	if (RepastProcess::instance()->rank() == 0 && tick % 720 == 0) {
		std::string time;
		repast::timestamp(time);
		std::cout << time << " -- Month: " << (tick / 720) << std::endl;
	}

	// for each person,
	for (unsigned int i = 0, n = people_->size(); i < n; i++) {
		Person* person = (*people_)[i];
		// perform the activity for the specified time and day_of_week
		person->performActivity(calendar.hour_of_day, calendar.day_of_year, calendar.year, calendar.isWeekDay());
		// update the stats -- update the disease status counts
		// with the status of the current person
		stats->countPerson(person);
	}

	// for each place,
	for (unsigned int i = 0, n = places.size(); i < n; ++i) {
		Place* place = places[i];
		// run the transmission algorithm specific to
		// this place.
		place->runTransmission();
		// reset the place for the next iteration
		place->reset();
	}

	stats->hourEnded();
	// this has to be called last so that we capture the
	// initial infection counts that were set during setup.
	TransmissionAlgorithm::instance()->resetCounts();
}

// load the activities and match them to the persons
void MRSAObserver::initializeActivities(Properties& props) {
	// create the activities -- adding them to a map where
	// the key is the activity list id.
	map<string, vector<Activity> > map;
	ActivityCreator creator;
	creator.run(props.getProperty(ACTIVITIES_FILE), map);

	// map should now have list of activities by id
	// match these up to persons.
	AgentSet<Person> people;
	get(people);

	// each person finds his / her list of activities
	// in the map
	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); iter++) {
		Person* person = (*iter);
		//std::cout << (*person) << std::endl;
		if (!person->initializeActivities(map)) {
			// if the person can't find its activities then
			// we remove it from the simulation
			person->die();
		}
	}
}

// creates the places. The places are pointers to
// Place so they can be easily shared among persons.
void MRSAObserver::createPlaces(Properties& props, map<string, Place*>* placeMap) {
	const string placesFile = props.getProperty(PLACES_FILE);
	const string riskFile = props.getProperty(RISK_FILE);

	// create the places.
	PlaceCreator placeCreator;
	placeCreator.run(placesFile, riskFile, places);

	// put the places in a map. Key is the place id
	// so that the PersonsCreator can easily look up
	// places and assign them to Persons.
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap->insert(pair<string, Place*>(place->placeId(), place));
	}
}

// Creates the Persons. The Persons are turtles and
// so must be created by the Observer using a functor.
void MRSAObserver::createPersons(Properties& props, map<string, Place*>* placeMap) {
	const string personsFile = props.getProperty(PERSONS_FILE);
	// count the lines in the file so we know how many
	// persons to create: one per line.
	int lines = line_count(personsFile);

	if (lines == -1)
		throw invalid_argument("Error opening: " + personsFile);

	float min_infection_duration = (float) strToDouble(props.getProperty(MIN_INFECT_PERIOD));

	// A PersonsCreator is used as a functor to create the persons
	// in concert with this MRSAObserver.
	PersonsCreator pCreator(personsFile, placeMap, min_infection_duration);
	// First line is the header info so we create one less
	// than the number of lines in the file.
	personType = create<Person>(lines - 1, pCreator);

	// get all the created Persons and validate them.
	AgentSet<Person> people;
	get(people);
	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); iter++) {
		Person* person = (*iter);
		// check if a person was assigned any places, and if not
		// then remove the person with die().
		person->validate();
	}
}

// initialize the disease state of all the persons in the model
// from some initial parameters.
void MRSAObserver::initializeDiseaseStatus(Properties& props, AgentSet<Person>& people) {

	// set the specified fraction of persons to colonized
	// using a random draw.
	double diseaseFrac = strToDouble(props.getProperty(COLONIZATION_FRAC));
	Random* random = Random::instance();
	unsigned int colonized = 0;
	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
		Person* person = (*iter);
		if (random->nextDouble() <= diseaseFrac) {
			person->updateStatus(COLONIZED);
			++colonized;
		} else {
			person->updateStatus(UNCOLONIZED);
		}
	}

	// shuffle the list of people and set the first
	// N to infected, assuming he / she is not colonized.
	people.shuffle();
	unsigned int infected = strToUInt(props.getProperty(INITIAL_INFECTION_COUNT));
	infected = infected > people.size() - colonized ? people.size() - colonized : infected;
	if (infected > 0) {
		unsigned int count = 0;
		for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
			Person* p = (*iter);
			// infect if colonized
			if (p->status() == UNCOLONIZED) {
				p->updateStatus(INFECTED);
				++count;
				// enough colonized so break.
				if (count == infected)
					break;
			}
		}
	}
}

void MRSAObserver::calcYearlyStats() {
	Statistics::getInstance()->yearEnded(*people_);
}

void MRSAObserver::initializeYearlyDataCollection(const string& file) {
	Statistics* stats = Statistics::getInstance();

	// Create a DataSet that will output data to yearly_data.csv
	SVDataSetBuilder builder(file, ",",
			repast::RepastProcess::instance()->getScheduleRunner().schedule());

	builder.addDataSource(
			createSVDataSource("infection_count", new LDataSourceAdapter(&stats->yearly_infected),
					std::plus<double>()));
	builder.addDataSource(
			createSVDataSource("colonized_count", new LDataSourceAdapter(&stats->yearly_colonized),
					std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("colonizations_from_infection",
					new LDataSourceAdapter(&stats->yearly_c_from_i), std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("colonizations_from_colonization",
					new LDataSourceAdapter(&stats->yearly_c_from_c), std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("avg_seek_care_infection_duration",
					new DDataSourceAdapter(&stats->yearly_seek_infection_duration),
					std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("avg_no_seek_care_infection_duration",
					new DDataSourceAdapter(&stats->yearly_no_seek_infection_duration),
					std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("avg_infection_duration",
					new DDataSourceAdapter(&stats->yearly_infection_duration),
					std::plus<double>()));
	builder.addDataSource(
			createSVDataSource("avg_colonization_duration",
					new DDataSourceAdapter(&stats->yearly_colonization_duration),
					std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("infected_r0",
					new DDataSourceAdapter(&stats->yearly_infected_r0), std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("colonized_r0",
					new DDataSourceAdapter(&stats->yearly_colonized_r0), std::plus<double>()));

	//builder.addDataSource(
	//			repast::createSVDataSource("combined_r0",
	//					new DDataSourceAdapter(&stats->yearly_r0), std::plus<double>()));

	string place_names[] = { "household", "hospital", "school", "workplace", "gym", "nursing home",
			"college dorm", "prison" };
	for (int i = 0; i < 8; i++) {
		std::string prefix = place_names[i] == "gym" ? "sports_facilities" : place_names[i];
		std::string header = prefix + "_colonization_fraction";
		builder.addDataSource(
				createSVDataSource(header,
						new PlaceCount(&stats->colonization_count_map, place_names[i]),
						std::plus<double>()));
	}

	repast::DataSet* ds = builder.createDataSet();
	// add it to the inherit dataSets vector.
	// this insures that it will be written on end
	// and properly cleaned up.
	Observer::dataSets.push_back(ds);
	// schedule the execution for yearly intervals.
	// 8760 = 24 * 365
	ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
	// schedule calcYearlyStats at year + .15
	runner.scheduleEvent(8760.15, 8760.0,
			Schedule::FunctorPtr(
					new MethodFunctor<MRSAObserver>(this, &MRSAObserver::calcYearlyStats)));
	// schedule record at year + .2
	runner.scheduleEvent(8760.2, 8760.0,
			Schedule::FunctorPtr(new MethodFunctor<repast::DataSet>(ds, &repast::DataSet::record)));
	// schedule write at year + .3
	runner.scheduleEvent(8760.3, 8760.0,
			Schedule::FunctorPtr(new MethodFunctor<repast::DataSet>(ds, &repast::DataSet::write)));
	// schedule write at year + .4
	runner.scheduleEvent(8760.4, 8760.0,
			Schedule::FunctorPtr(
					new MethodFunctor<Statistics>(stats, &Statistics::resetYearlyStats)));

}

// initialize the data collection by creating
// a data set and adding data sources to it.
void MRSAObserver::initializeHourlyDataCollection(const string& file) {
	Statistics* stats = Statistics::getInstance();

	// Create a DataSet that will output data to hourly_data.csv
	SVDataSetBuilder builder(file, ",",
			repast::RepastProcess::instance()->getScheduleRunner().schedule());

	// data source for counting the number of uncolonized persons
	builder.addDataSource(
			repast::createSVDataSource("uncolonized_count",
					new LDataSourceAdapter(&stats->hourly_uncolonized), std::plus<double>()));

	// data source for counting the number of colonized persons
	builder.addDataSource(
			repast::createSVDataSource("colonized_count",
					new LDataSourceAdapter(&stats->hourly_colonized), std::plus<double>()));

	// data source for counting the number of infected persons
	builder.addDataSource(
			repast::createSVDataSource("infection_count",
					new LDataSourceAdapter(&stats->hourly_infected), std::plus<double>()));

	// data source for the total number of persons
	TotalSum* tSum = new TotalSum(stats);
	builder.addDataSource(repast::createSVDataSource("total_count", tSum, std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("newly_infected",
					new LDataSourceAdapter(&stats->newly_infected), std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("newly_colonized",
					new LDataSourceAdapter(&stats->newly_colonized), std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("running_total_infected",
					new DDataSourceAdapter(&stats->total_infected), std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("running_total_colonized",
					new DDataSourceAdapter(&stats->total_colonized), std::plus<double>()));

	// add the data set to this Observer, which automatically
	// schedules the data collection, data writing etc.
	addDataSet(builder.createDataSet());
}

void MRSAObserver::atEnd() {
	Statistics::getInstance()->calculateSummaryStats(*people_, summary_output_file);
}

// entry point for model setup.
void MRSAObserver::setup(Properties& props) {
	Parameters::initialize(props);
	Parameters* params = Parameters::instance();

	// time stamp to mark start of setup
	std::string time;
	repast::timestamp(time);
	std::cout << "Setup Started at " << time << std::endl;

	// create a random distribution used by Person-s to
	// choose which "other household" to go to
	_IntUniformGenerator gen(Random::instance()->engine(), boost::uniform_int<>(0, 3));
	Random::instance()->putGenerator(OH_DIST,
			new DefaultNumberGenerator<_IntUniformGenerator>(gen));

	//double duration_mean = params->getDoubleParameter(HOSPITAL_STAY_DURATION_MEAN);
	//double duration_sd = params->getDoubleParameter(HOSPITAL_STAY_DURATION_SD);
	//_NormalGenerator ngen(Random::instance()->engine(),
	//		boost::normal_distribution<>(duration_mean, duration_sd));
	//Random::instance()->putGenerator(HOSPITAL_STAY_DURATION,
	//		new DefaultNumberGenerator<_NormalGenerator>(ngen));

	// setup the transmission algorithm with the
	// a, b, c etc. parameters.
	TAParameters taParams;
	taParams.a = params->getDoubleParameter(A);
	taParams.b = params->getDoubleParameter(B);
	taParams.e = params->getDoubleParameter(E);
	taParams.alpha = params->getDoubleParameter(ALPHA);
	taParams.beta = params->getDoubleParameter(THETA);
	taParams.gamma = params->getDoubleParameter(GAMMA);
	taParams.rho = params->getDoubleParameter(Z);

	TransmissionAlgorithm::initialize(taParams);

	// create the persons and places
	// use a pointer because it is eventually passed
	// to the PersonsCreator whose copy constructor is
	// called and so a pointer is faster to copy.
	map<string, Place*>* placeMap = new map<string, Place*>();
	createPlaces(props, placeMap);
	createPersons(props, placeMap);

	delete placeMap;

	// initialize the activities
	initializeActivities(props);
	AgentSet<Person> people;
	get(people);

	// initialize the disease status for each person.
	initializeDiseaseStatus(props, people);
	//people.ask(&Person::goToHome);

	// initialize the hourly data collection
	initializeHourlyDataCollection(params->getStringParameter(HOURLY_OUTPUT_FILE));
	// initialize yearly data collection
	initializeYearlyDataCollection(params->getStringParameter(YEARLY_OUTPUT_FILE));
	summary_output_file = params->getStringParameter(SUMMARY_OUTPUT_FILE);

	// get a permanent set of people so we don't have to retrieve
	// them each iteration which will be slow. We do this here because
	// prevoius setup type methods may have altered the initial set of
	// persons.
	people_ = new AgentSet<Person>();
	get(*people_);

	Statistics* stats = Statistics::getInstance();
	stats->setInitialCounts(*people_);

	// schedule MRSAObserver::atEnd() to be called with the sim terminates.
	ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
	runner.scheduleEndEvent(
			Schedule::FunctorPtr(
					new MethodFunctor<MRSAObserver>(this, &mrsa::MRSAObserver::atEnd)));

	repast::timestamp(time);
	std::cout << "Setup Finished at " << time << std::endl;
}

} /* namespace mrsa */
