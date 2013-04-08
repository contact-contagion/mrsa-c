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
#include "Calendar.h"
#include "PersonsCreator.h"
#include "PlaceCreator.h"
#include "ActivityCreator.h"
#include "TransmissionAlgorithm.h"
#include "Parameters.h"
#include "Constants.h"
#include "TransmissionEventRecorder.h"

//#include "FileOutput.h"
//#include "RegionMap.h"

namespace mrsa {

using namespace repast::relogo;
using namespace repast;
using namespace std;

// counts the number of lines in the specified  persons file.
// and puts the line count and sum of the p_mrsa column the output arguments
void line_count(const std::string& file, unsigned int& line_count, double& p_mrsa_sum) {
	CSVReader reader(file);
	vector<string> vec;
	// skip the header
	reader.skip(1);
	while (reader.next(vec)) {
		p_mrsa_sum += strToDouble(vec[P_MRSA_IDX]);
		++line_count;
	}
}

MRSAObserver::MRSAObserver() :
		personType(0), places(0), people_(0), summary_output_file(), calendar(), propsPtr(0), yearCounter(
				0) {
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
		person->performActivity(calendar);
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

	// create the places.
	PlaceCreator placeCreator;
//	placeCreator.run(placesFile, riskFile, places);

	placeCreator.run(placesFile, props, places);

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
	boost::mpi::communicator world;

	const string personsFile = props.getProperty(PERSONS_FILE);

	unsigned int lines = 0;
	double p_mrsa_sum = 0;
	// count the lines in the file so we know how many
	// persons to create: one per line.
	line_count(personsFile, lines, p_mrsa_sum);
	//std::cout << lines << ", " << p_mrsa_sum << std::endl;

	float min_infection_duration = (float) strToDouble(props.getProperty(MIN_INFECT_PERIOD));
	Parameters* params = Parameters::instance();

	// A PersonsCreator is used as a functor to create the persons
	// in concert with this MRSAObserver.
	PersonsCreator pCreator(personsFile, placeMap, min_infection_duration,
			params->getDoubleParameter(MIN_JAIL_DURATION), params->getDoubleParameter(MAX_JAIL_DURATION),
			params->getDoubleParameter(PEAK_JAIL_DURATION),
			p_mrsa_sum);
	// create N number of persons, where N is the number of non-header
	// lines in the persons file
	personType = create<Person>(lines, pCreator);

	// get all the created Persons and validate them.
	AgentSet<Person> people;
	get(people);

	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); iter++) {
		Person* person = (*iter);
		// check if a person was assigned any places, and if not
		// then remove the person with die().
		person->validate();
	}

	people.clear();
	get(people);
}


void MRSAObserver::calcYearlyStats() {
	yearCounter++;
	Statistics::getInstance()->yearEnded(*people_, yearCounter, *propsPtr);
}

void MRSAObserver::initializeYearlyDataCollection(const string& file) {
	Statistics* stats = Statistics::getInstance();

	// Create a DataSet that will output data to yearly_data.csv
	SVDataSetBuilder builder(file, ",",
			repast::RepastProcess::instance()->getScheduleRunner().schedule());
	stats->createYearlyDataSources(builder);

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
	stats->createHourlyDataSources(builder);

	// add the data set to this Observer, which automatically
	// schedules the data collection, data writing etc.
	addDataSet(builder.createDataSet());
}

void MRSAObserver::atEnd() {
	Statistics::getInstance()->calculateSummaryStats(*people_, summary_output_file, *propsPtr);
	TransmissionEventRecorder::instance()->close();
}

// entry point for model setup.
void MRSAObserver::setup(Properties& props) {
	propsPtr = &props;
	Parameters::initialize(props);
	Parameters* params = Parameters::instance();

	// debug file output
	//FileOutput::initialize("output/size_check.csv");
	//(*FileOutput::instance()) << "tick,id,place_type,size" << std::endl;

	// time stamp to mark start of setup
	std::string time;
	repast::timestamp(time);
	std::cout << "Setup Started at " << time << std::endl;

	TransmissionEventRecorder::initialize(props.getProperty(EVENT_OUTPUT_FILE));

	// create a random distribution used by Person-s to
	// choose which "other household" to go to
	_IntUniformGenerator gen(Random::instance()->engine(), boost::uniform_int<>(0, 3));
	Random::instance()->putGenerator(OH_DIST,
			new DefaultNumberGenerator<_IntUniformGenerator>(gen));

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

	/*
	int i_count = 0;
	int c_count = 0;
	for (size_t i = 0; i < people.size(); ++i) {
		Person* p = people[i];
		if (p->status() == INFECTED) ++i_count;
		else if (p->status() == COLONIZED) ++c_count;
	}
	std::cout << i_count << ", " << c_count << std::endl;
	*/

	// initialize the hourly data collection
	int runNumber = strToInt(props.getProperty("run.number"));

//	initializeHourlyDataCollection(params->getStringParameter(HOURLY_OUTPUT_FILE));
	std::stringstream hourlyDataFileName;
	hourlyDataFileName << props.getProperty(HOURLY_OUTPUT_FILE) << "_" << runNumber << ".csv";
	initializeHourlyDataCollection(hourlyDataFileName.str());

	// initialize yearly data collection
//	initializeYearlyDataCollection(params->getStringParameter(YEARLY_OUTPUT_FILE));
	std::stringstream yearlyDataFileName;
	yearlyDataFileName << props.getProperty(YEARLY_OUTPUT_FILE) << "_" << runNumber << ".csv";
	initializeYearlyDataCollection(yearlyDataFileName.str());

//	summary_output_file = params->getStringParameter(SUMMARY_OUTPUT_FILE);
	std::stringstream summaryOutputFileName;
	summaryOutputFileName << props.getProperty(SUMMARY_OUTPUT_FILE) << "_" << runNumber << ".txt";
	summary_output_file = summaryOutputFileName.str();

	// get a permanent set of people so we don't have to retrieve
	// them each iteration which will be slow. We do this here because
	// prevoius setup type methods may have altered the initial set of
	// persons.
	people_ = new AgentSet<Person>();
	get(*people_);

	// no long set the initial stat counts
	//Statistics* stats = Statistics::getInstance();
	//stats->setInitialCounts(*people_);

	// schedule MRSAObserver::atEnd() to be called with the sim terminates.
	ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
	runner.scheduleEndEvent(
			Schedule::FunctorPtr(
					new MethodFunctor<MRSAObserver>(this, &mrsa::MRSAObserver::atEnd)));

	repast::timestamp(time);
	std::cout << "Setup Finished at " << time << std::endl;
}

} /* namespace mrsa */
