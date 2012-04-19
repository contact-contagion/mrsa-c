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

namespace mrsa {

using namespace repast::relogo;
using namespace repast;
using namespace std;

const string PERSONS_FILE = "persons.file";
const string PLACES_FILE = "places.file";
const string ACTIVITIES_FILE = "activities.file";

const string COLONIZATION_FRAC = "initial.colonization.fraction";
const string INITIAL_INFECTION_COUNT = "initial.infected.count";
const string TRANSITION_RULE = "transition.rule";
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
		transition(new NoOpTransitionFunctor()), person_stats(0), places(0) {

}

MRSAObserver::~MRSAObserver() {
	delete transition;
	delete person_stats;
	delete places;
}

void MRSAObserver::go() {
	person_stats->clear();

	int tick =
			(int) RepastProcess::instance()->getScheduleRunner().currentTick();
	int time = tick % 24;
	int day = (tick / 24) % 7;
	bool is_weekday = (day == 0 || day == 6) ? false : true;

	//std::cout << "tick: " << tick << ", time: " << time << ", day: " << day << ", weekday: " << is_weekday << std::endl;

	AgentSet<Person> people;
	get(people);
	for (unsigned int i = 0, n = people.size(); i < n; i++) {
		Person* person = people[i];
		person->performActivity(time, is_weekday);
		person->updatePlaceCounters();
		person_stats->countPerson(person);
	}

	people.apply(*transition);

	for (unsigned int i = 0, n = places->size(); i < n; i++) {
		Place* place = (*places)[i];
		place->place_colonized = place->place_infected =
				place->place_uncolonized = 0;
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
	int lines = line_count(placesFile);
	//std::cout << "places line count: " << lines << std::endl;

	if (lines == -1)
		throw invalid_argument("Error opening: " + placesFile);

	PlaceCreator placeCreator(placesFile);
	// we skip the first line so one less person to make
	placeType = create<Place>(lines - 1, placeCreator);

	// use a pointer here because this map could be very large
	// so we don't want to copy it around.

	places = new AgentSet<Place>();
	get(*places);
	for (int i = 0, n = places->size(); i < n; i++) {
		Place* place = (*places)[i];
		placeMap->insert(pair<string, Place*>(place->place_id, place));
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
	std::cout << "Setup Started at " << time  << std::endl;

	double a = strToDouble(props.getProperty("a"));
	double b = strToDouble(props.getProperty("b"));
	double c = strToDouble(props.getProperty("c"));
	double d = strToDouble(props.getProperty("d"));
	double e = strToDouble(props.getProperty("e"));
	double scaling = strToDouble(props.getProperty(FASTER_SCALING));

	StatusCalculator::initialize(a, b, c, d, e, scaling);

	map<string, Place*>* placeMap = new map<string, Place*>();
	createPlaces(props, placeMap);
	createPersons(props, placeMap);
	delete placeMap;

	initializeActivities(props);
	AgentSet<Person> people;
	get(people);

	initializeDiseaseStatus(props, people);
	people.ask(&Person::goToHome);
	// update the stats to clear out any place
	// infection etc. counts triggered by going home.
	//place_stats->update();

	string transition_rule = props.getProperty(TRANSITION_RULE);

	if (transition_rule == NONE) {
		transition = new NoOpTransitionFunctor();
	} else if (transition_rule == DETAILED_PLACE) {
		float infection_period = (float) strToDouble(
				props.getProperty(INFECTION_PERIOD));
		transition = new DetailedPlaceTransitionFunctor(infection_period);
	} else {
		throw domain_error("Invalid transition rule");
	}

	initializeDataCollection();

	// update the place counters
	for (unsigned int i = 0, n = people.size(); i < n; i++) {
		Person* person = people[i];
		person->updatePlaceCounters();
	}

	repast::timestamp(time);
	std::cout << "Setup Finished at " << time << std::endl;
}

} /* namespace mrsa */
