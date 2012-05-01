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

namespace mrsa {

using namespace repast::relogo;
using namespace repast;
using namespace std;

// property constants, used to look up
// property values from the model properties file.
const string PERSONS_FILE = "persons.file";
const string PLACES_FILE = "places.file";
const string ACTIVITIES_FILE = "activities.file";

const string COLONIZATION_FRAC = "initial.colonization.fraction";
const string INITIAL_INFECTION_COUNT = "initial.infected.count";
const string INFECTION_PERIOD = "minimum.infection.period";
const string FASTER_SCALING = "faster.response.scaling.factor";

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

MRSAObserver::MRSAObserver() :
		personType(0), person_stats(0), places(0), people_(0) {
}

MRSAObserver::~MRSAObserver() {
	delete person_stats;
	// delete the permanent AgentSet used to hold the
	// population.
	delete people_;

	// delete each place from the list of places.
	for (vector<Place*>::iterator iter  = places.begin(); iter != places.end(); ++iter) {
		delete (*iter);
	}
}

// called each tick
void MRSAObserver::go() {
	// clear the stats, as they are per tick.
	person_stats->clear();
	// using the current tick determine the simulated hour and
	// day and whether this is a weekday or weekend.
	int tick =
			(int) RepastProcess::instance()->getScheduleRunner().currentTick();
	int time = tick % 24;
	int day = (tick / 24) % 7;
	bool is_weekday = (day == 0 || day == 6) ? false : true;

	// print out time stamp each simluated month
	// on process 0
	if (RepastProcess::instance()->rank() == 0 && tick % 720 == 0) {
		std::string time;
		repast::timestamp(time);
		std::cout << time << " -- Month: " << (tick / 720) << std::endl;
	}

	// for each person,
	for (unsigned int i = 0, n = people_->size(); i < n; i++) {
		Person* person = (*people_)[i];
		// perform the activity for the specified time and day
		person->performActivity(time, is_weekday);
		// update the stats -- update the disease status counts
		// with the status of the current person
		person_stats->countPerson(person);
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
	for (AgentSet<Person>::as_iterator iter = people.begin();
			iter != people.end(); iter++) {
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
void MRSAObserver::createPlaces(Properties& props,
		map<string, Place*>* placeMap) {
	const string placesFile = props.getProperty(PLACES_FILE);


	// create the places.
	PlaceCreator placeCreator;
	placeCreator.run(placesFile, places);

	// put the places in a map. Key is the place id
	// so that the PersonsCreator can easily look up
	// places and assign them to Persons.
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place =  places[i];
		placeMap->insert(pair<string, Place*>(place->placeId(), place));
	}
}

// Creates the Persons. The Persons are turtles and
// so must be created by the Observer using a functor.
void MRSAObserver::createPersons(Properties& props,
		map<string, Place*>* placeMap) {
	const string personsFile = props.getProperty(PERSONS_FILE);
	// count the lines in the file so we know how many
	// persons to create: one per line.
	int lines = line_count(personsFile);

	if (lines == -1)
		throw invalid_argument("Error opening: " + personsFile);

	// A PersonsCreator is used a functor to create the persons
	// in concert with this MRSAObserver.
	PersonsCreator pCreator(personsFile, placeMap);
	// First line is the header info so we create one less
	// than the number of lines in the file.
	personType = create<Person>(lines - 1, pCreator);

	// get all the created Persons and validate them.
	AgentSet<Person> people;
	get(people);
	for (AgentSet<Person>::as_iterator iter = people.begin();
			iter != people.end(); iter++) {
		Person* person = (*iter);
		// check if a person was assigned any places, and if not
		// then remove the person with die().
		person->validate();
	}
}

// initialize the disease state of all the persons in the model
// from some initial parameters.
void MRSAObserver::initializeDiseaseStatus(Properties& props,
		AgentSet<Person>& people) {

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
	unsigned int count = 0;
	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
		Person* p = (*iter);
		// infect if colonized
		if (p->status() == UNCOLONIZED) {
			p->updateStatus(INFECTED);
			++count;
			// enough colonized so break.
			if (count == infected) break;
		}
	}
}

// initialize the data collection by creating
// a data set and adding data sources to it.
void MRSAObserver::initializeDataCollection() {
	// PersonStats counts number of colonized,
	// uncolonized and infected persons.
	person_stats = new PersonStats();

	// Create a DataSet that will output data to data.csv
	SVDataSetBuilder builder("./output/data.csv", ",",
			repast::RepastProcess::instance()->getScheduleRunner().schedule());

	// data source for counting the number of uncolonized persons
	UnColonizedSum* ocSum = new UnColonizedSum(person_stats);
	builder.addDataSource(
			repast::createSVDataSource("uncolonized_count", ocSum,
					std::plus<double>()));

	// data source for counting the number of colonized persons
	ColonizedSum* cSum = new ColonizedSum(person_stats);
	builder.addDataSource(
			repast::createSVDataSource("colonized_count", cSum,
					std::plus<double>()));

	// data source for counting the number of infected persons
	InfectionSum* iSum = new InfectionSum(person_stats);
	builder.addDataSource(
			repast::createSVDataSource("infection_count", iSum,
					std::plus<double>()));

	// data source for the total number of persons
	TotalSum* tSum = new TotalSum(person_stats);
	builder.addDataSource(
			repast::createSVDataSource("total_count", tSum,
					std::plus<double>()));

	// add the data set to this Observer, which automatically
	// schedules the data collection, data writing etc.
	addDataSet(builder.createDataSet());
}

// entry point for model setup.
void MRSAObserver::setup(Properties& props) {
	// time stamp to mark start of setup
	std::string time;
	repast::timestamp(time);
	std::cout << "Setup Started at " << time << std::endl;

	// setup the transmission algorithm with the
	// a, b, c etc. parameters.
	double a = strToDouble(props.getProperty("a"));
	double b = strToDouble(props.getProperty("b"));
	double c = strToDouble(props.getProperty("c"));
	double d = strToDouble(props.getProperty("d"));
	double e = strToDouble(props.getProperty("e"));

	TransmissionAlgorithm::initialize(a, b, c, d, e);

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
	people.ask(&Person::goToHome);

	// initialize the data collection
	initializeDataCollection();

	// get a permanent set of people so we don't have to retrieve
	// them each iteration which will be slow. We do this here because
	// prevoius setup type methods may have altered the initial set of
	// persons.
	people_ = new AgentSet<Person>();
	get(*people_);

	repast::timestamp(time);
	std::cout << "Setup Finished at " << time << std::endl;
}

} /* namespace mrsa */
