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

const string PERSONS_FILE = "persons.file";
const string PLACES_FILE = "places.file";
const string ACTIVITIES_FILE = "activities.file";

const string COLONIZATION_FRAC = "initial.colonization.fraction";
const string INITIAL_INFECTION_COUNT = "initial.infected.count";
const string INFECTION_PERIOD = "minimum.infection.period";
const string FASTER_SCALING = "faster.response.scaling.factor";

const string NONE = "none";
const string SIMPLE = "simple";
const string DETAILED_ACTIVITY = "detailed activity";
const string DETAILED_PLACE = "detailed place";

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
	delete people_;

	for (vector<Place*>::iterator iter  = places.begin(); iter != places.end(); ++iter) {
		delete (*iter);
	}
}

void MRSAObserver::go() {
	person_stats->clear();
	int tick =
			(int) RepastProcess::instance()->getScheduleRunner().currentTick();
	int time = tick % 24;
	int day = (tick / 24) % 7;
	bool is_weekday = (day == 0 || day == 6) ? false : true;

	if (tick % 720 == 0) {
		std::string time;
		repast::timestamp(time);
		std::cout << time << " -- Month: " << (tick / 720) << std::endl;
	}

	for (unsigned int i = 0, n = people_->size(); i < n; i++) {
		Person* person = (*people_)[i];
		person->performActivity(time, is_weekday);
		person_stats->countPerson(person);
	}

	for (unsigned int i = 0, n = places.size(); i < n; ++i) {
		Place* place = places[i];
		place->runTransmission();
		place->reset();
	}
}

void MRSAObserver::initializeActivities(Properties& props) {
	map<string, vector<Activity*>*> map;
	ActivityCreator creator;
	creator.run(props.getProperty(ACTIVITIES_FILE), map);

	// map should now have list of activities by id
	// match these up to persons.
	AgentSet<Person> people;
	get(people);

	for (AgentSet<Person>::as_iterator iter = people.begin();
			iter != people.end(); iter++) {
		Person* person = (*iter);
		//std::cout << (*person) << std::endl;
		if (!person->initializeActivities(map)) {
			person->die();
		}
	}
}

void MRSAObserver::createPlaces(Properties& props,
		map<string, Place*>* placeMap) {
	const string placesFile = props.getProperty(PLACES_FILE);


	PlaceCreator placeCreator;
	placeCreator.run(placesFile, places);
	// we skip the first line so one less person to make

	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place =  places[i];
		placeMap->insert(pair<string, Place*>(place->placeId(), place));
	}
}

void MRSAObserver::createPersons(Properties& props,
		map<string, Place*>* placeMap) {
	const string personsFile = props.getProperty(PERSONS_FILE);
	int lines = line_count(personsFile);

	if (lines == -1)
		throw invalid_argument("Error opening: " + personsFile);

	PersonsCreator pCreator(personsFile, placeMap);
	// we skip the first line so one less person to make
	personType = create<Person>(lines - 1, pCreator);

	AgentSet<Person> people;
	get(people);
	for (AgentSet<Person>::as_iterator iter = people.begin();
			iter != people.end(); iter++) {
		Person* person = (*iter);
		person->validate();
	}
}

void MRSAObserver::initializeDiseaseStatus(Properties& props,
		AgentSet<Person>& people) {
	// initialize disease status
	double diseaseFrac = strToDouble(props.getProperty(COLONIZATION_FRAC));
	Random* random = Random::instance();
	for (AgentSet<Person>::as_iterator iter = people.begin();
			iter != people.end(); iter++) {
		Person* person = (*iter);
		if (random->nextDouble() <= diseaseFrac) {
			person->updateStatus(COLONIZED);
		} else {
			person->updateStatus(UNCOLONIZED);
		}
	}

	people.shuffle();
	unsigned int infected = strToUInt(
			props.getProperty(INITIAL_INFECTION_COUNT));
	infected = infected > people.size() ? people.size() : infected;
	for (unsigned int i = 0; i < infected; i++) {
		people.at(i)->updateStatus(INFECTED);
	}
}

void MRSAObserver::initializeDataCollection() {
	person_stats = new PersonStats();
	SVDataSetBuilder builder("./output/data.csv", ",",
			repast::RepastProcess::instance()->getScheduleRunner().schedule());

	UnColonizedSum* ocSum = new UnColonizedSum(person_stats);
	builder.addDataSource(
			repast::createSVDataSource("uncolonized_count", ocSum,
					std::plus<double>()));

	ColonizedSum* cSum = new ColonizedSum(person_stats);
	builder.addDataSource(
			repast::createSVDataSource("colonized_count", cSum,
					std::plus<double>()));

	InfectionSum* iSum = new InfectionSum(person_stats);
	builder.addDataSource(
			repast::createSVDataSource("infection_count", iSum,
					std::plus<double>()));

	TotalSum* tSum = new TotalSum(person_stats);
	builder.addDataSource(
			repast::createSVDataSource("total_count", tSum,
					std::plus<double>()));

	addDataSet(builder.createDataSet());
}

void MRSAObserver::setup(Properties& props) {
	std::string time;
	repast::timestamp(time);
	std::cout << "Setup Started at " << time << std::endl;

	double a = strToDouble(props.getProperty("a"));
	double b = strToDouble(props.getProperty("b"));
	double c = strToDouble(props.getProperty("c"));
	double d = strToDouble(props.getProperty("d"));
	double e = strToDouble(props.getProperty("e"));

	TransmissionAlgorithm::initialize(a, b, c, d, e);

	map<string, Place*>* placeMap = new map<string, Place*>();
	createPlaces(props, placeMap);
	createPersons(props, placeMap);
	delete placeMap;

	initializeActivities(props);
	AgentSet<Person> people;
	get(people);

	initializeDiseaseStatus(props, people);
	people.ask(&Person::goToHome);

	initializeDataCollection();
	// get the set of people so we don't have to grab
	// them each iteration. We do this here because
	// other setup type methods may alter the set.
	people_ = new AgentSet<Person>();
	get(*people_);

	repast::timestamp(time);
	std::cout << "Setup Finished at " << time << std::endl;
}

} /* namespace mrsa */
