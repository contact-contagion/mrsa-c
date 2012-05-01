#define BOOST_TEST_DYN_LINK
//#define BOOST_TEST_MODULE transmission_tests

#include <boost/test/unit_test.hpp>

#include <repast_hpc/RepastProcess.h>

#include "../src/TransmissionAlgorithm.h"
#include "../src/PlaceCreator.h"
#include "../src/PersonsCreator.h"
#include "../src/ActivityCreator.h"

#include "ObserverSetup.h"

using namespace std;
using namespace mrsa;
using namespace repast::relogo;

typedef AgentSet<Person>::as_iterator ASIter;

BOOST_FIXTURE_TEST_SUITE(transmission_test, ObserverSetup);

BOOST_AUTO_TEST_CASE(uncolonized) {

	TransmissionAlgorithm::initialize(0, 0, 0.0, 0, 0);
	// a is 0, so starting with UNCOLONIZED should always return UNCOLONIZED
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->run(0, 0, 0, UNCOLONIZED, 0) == UNCOLONIZED);
	}

	TransmissionAlgorithm::initialize(1, 0, 0.0, 0, 0);
	ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->run(1, 0, 0, UNCOLONIZED, 1) == COLONIZED);
	}

	TransmissionAlgorithm::initialize(0.5, 0, 0.0, 0, 0);
	// a is .5 and risk is 2 so double a is one so always move to colonized.
	ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->run(1, 0, 0, UNCOLONIZED, 2) == COLONIZED);
	}

}

BOOST_AUTO_TEST_CASE(colonized) {

	double b = 1;
	TransmissionAlgorithm::initialize(0, b, 0.0, 0, 0);
	DiseaseStatus status(COLONIZED);
	// b is one so should always move from colonized to infected
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->run(0, 0, 0, status, 0) == INFECTED);
	}

	b = 0;
	double e = 1;
	TransmissionAlgorithm::initialize(0, b, 0.0, 0, e);
	// b is 0 and e is one so should always return to uncolonized
	ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->run(0, 0, 0, status, 0) == UNCOLONIZED);
	}
}

BOOST_AUTO_TEST_CASE(infected) {

	double c = 0;
	double d = 1;
	TransmissionAlgorithm::initialize(0, 0, c, d, 0);
	DiseaseStatus status(INFECTED);
	// d is one so should always move from infected to uncolonized
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->run(0, 0, 0, status, 0) == UNCOLONIZED);
	}

	c = 1;
	d = 0;
	TransmissionAlgorithm::initialize(0, 0, c, d, 0);
	// c is 1 and d is 0 so should move to colonized
	ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->run(0, 0, 0, status, 0) == COLONIZED);
	}
}

struct PlacePredicate {

	std::string id;

	PlacePredicate(std::string hh_id) : id(hh_id) {}

	bool operator()(Place* place) {
		return place->placeId() == id;
	}
};

BOOST_AUTO_TEST_CASE(workplace) {
	PlaceCreator creator;
	vector<Place*> places;
	creator.run("../test_data/places.csv", places);
	std::map<std::string, Place*> placeMap;
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap.insert(pair<string, Place*>(place->placeId(), place));
	}

	PersonsCreator pCreator("../test_data/people.csv", &placeMap, 7);
	obs->create<Person>(14, pCreator);
	AgentSet<Person> persons;
	obs->get(persons);

	Place* place = *(find_if(places.begin(), places.end(), PlacePredicate("1703101010000001")));

	bool first = true;
	for (ASIter iter = persons.begin(); iter != persons.end(); ++iter) {
		Person*p = (*iter);
		BOOST_REQUIRE(p->status() == UNCOLONIZED);
		if (first) {
			// need at least 1 infected person
			// to move the others
			p->updateStatus(INFECTED);
			first = false;
		}
		place->addPerson(p);
	}

	// a is 10 because the risk is .1f, so draw should be 1 and
	// so 4 should be colonized.
	TransmissionAlgorithm::initialize(10, 0, 0.0, 0, 0);
	place->runTransmission();

	int colonized_count = 0;
	for (ASIter iter = persons.begin(); iter != persons.end(); ++iter) {
		if ((*iter)->status() == COLONIZED) ++colonized_count;
	}

	BOOST_REQUIRE(colonized_count == 4);

	for (int i = 0, n = places.size(); i < n; i++) {
		delete places[i];
	}
}

BOOST_AUTO_TEST_CASE(school) {
	PlaceCreator creator;
	vector<Place*> places;
	creator.run("../test_data/places.csv", places);
	std::map<std::string, Place*> placeMap;
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap.insert(pair<string, Place*>(place->placeId(), place));
	}

	PersonsCreator pCreator("../test_data/people.csv", &placeMap, 7);
	obs->create<Person>(14, pCreator);
	AgentSet<Person> persons;
	obs->get(persons);

	vector<Person*> forties;
	vector<Person*> twelves;
	for (ASIter iter = persons.begin(); iter != persons.end(); ++iter) {
		Person*p = (*iter);
		if (p->age() == 40) forties.push_back(p);
		else if(p->age() == 12) twelves.push_back(p);
	}

	Place* place = *(find_if(places.begin(), places.end(), PlacePredicate("170993001118")));
	BOOST_REQUIRE(place->placeType() == "School");

	(twelves[0])->updateStatus(INFECTED);
	for (vector<Person*>::iterator iter = forties.begin(); iter != forties.end(); ++iter) {
		place->addPerson(*iter);
	}

	for (vector<Person*>::iterator iter = twelves.begin(); iter != twelves.end(); ++iter) {
		place->addPerson(*iter);
	}

	// a is 1, so draw should be 1 and so tweleves should be colonized
	// others should be uncolonized
	TransmissionAlgorithm::initialize(1, 0, 0.0, 0, 0);
	place->runTransmission();
	BOOST_REQUIRE_EQUAL(twelves[1]->status(), COLONIZED);
	BOOST_REQUIRE_EQUAL(twelves[2]->status(), COLONIZED);
	BOOST_REQUIRE_EQUAL(twelves[3]->status(), COLONIZED);

	BOOST_REQUIRE_EQUAL(forties[0]->status(), UNCOLONIZED);
	BOOST_REQUIRE_EQUAL(forties[1]->status(), UNCOLONIZED);
	BOOST_REQUIRE_EQUAL(forties[2]->status(), UNCOLONIZED);

	place->reset();

	bool first = true;
	for (vector<Person*>::iterator iter = forties.begin(); iter != forties.end(); ++iter) {
		if (first) {
			(*iter)->updateStatus(INFECTED);
			first = false;
		} else {
			(*iter)->updateStatus(UNCOLONIZED);
		}
		place->addPerson(*iter);
	}

	for (vector<Person*>::iterator iter = twelves.begin(); iter != twelves.end(); ++iter) {
		(*iter)->updateStatus(UNCOLONIZED);
		place->addPerson(*iter);
	}

	place->runTransmission();

	BOOST_REQUIRE_EQUAL(twelves[0]->status(), UNCOLONIZED);
	BOOST_REQUIRE_EQUAL(twelves[1]->status(), UNCOLONIZED);
	BOOST_REQUIRE_EQUAL(twelves[2]->status(), UNCOLONIZED);
	BOOST_REQUIRE_EQUAL(twelves[3]->status(), UNCOLONIZED);

	BOOST_REQUIRE_EQUAL(forties[1]->status(), COLONIZED);
	BOOST_REQUIRE_EQUAL(forties[2]->status(), COLONIZED);
}

BOOST_AUTO_TEST_CASE(household) {
	PlaceCreator creator;
	vector<Place*> places;
	creator.run("../test_data/places.csv", places);
	std::map<std::string, Place*> placeMap;
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap.insert(pair<string, Place*>(place->placeId(), place));
	}


	PersonsCreator pCreator("../test_data/people.csv", &placeMap, 2);
	obs->create<Person>(2, pCreator);
	repast::relogo::AgentSet<Person> persons;
	obs->get(persons);

	Person* p1 = persons[0];
	p1->updateStatus(UNCOLONIZED);
	BOOST_REQUIRE(p1->status() == UNCOLONIZED);
	Person* p2 = persons[1];
	p2->updateStatus(INFECTED);
	BOOST_REQUIRE(p2->status() == INFECTED);

	Place* hh = *(find_if(places.begin(), places.end(), PlacePredicate("2038461")));

	// household now has one uncolonized and one infected person.
	hh->addPerson(p1);
	hh->addPerson(p2);

	// a is 0, so p1 should remain uncolonized.
	TransmissionAlgorithm::initialize(0, 0, 0.0, 0, 0);
	hh->runTransmission();
	BOOST_REQUIRE(p1->status() == UNCOLONIZED);

	// a is 1, so p1 should be colonized
	TransmissionAlgorithm::initialize(1, 0, 0.0, 0, 0);
	hh->runTransmission();
	BOOST_REQUIRE(p1->status() == COLONIZED);

	hh->reset();
	p1->updateStatus(UNCOLONIZED);
	p2->updateStatus(INFECTED);
	hh->addPerson(p1);
	hh->addPerson(p2);
	// a is .5 but risk for household is 2 so should be
	// colonized
	TransmissionAlgorithm::initialize(0.5, 0, 0.0, 0, 0);
	hh->runTransmission();
	BOOST_REQUIRE(p1->status() == COLONIZED);

	double b = 1;
	TransmissionAlgorithm::initialize(0, b, 0.0, 0, 0);
	hh->reset();
	hh->addPerson(p1);
	hh->runTransmission();
	BOOST_REQUIRE(p1->status() == INFECTED);

	// run the schedule for two ticks, to increment the tick count by 2.
	// this means that the minimum disease duration has passed and p1
	// can move to uncolonized
	repast::RepastProcess::instance()->getScheduleRunner().scheduleStop(2);
	repast::RepastProcess::instance()->getScheduleRunner().run();

	double c = 0;
	double d = 1;
	TransmissionAlgorithm::initialize(0, 0, c, d, 0);
	hh->reset();
	hh->addPerson(p1);
	hh->runTransmission();
	BOOST_REQUIRE(p1->status() == UNCOLONIZED);

	for (int i = 0, n = places.size(); i < n; i++) {
		delete places[i];
	}
}

BOOST_AUTO_TEST_CASE(activity_test) {
	// create the persons
	PlaceCreator creator;
	vector<Place*> places;
	creator.run("../test_data/places.csv", places);
	std::map<std::string, Place*> placeMap;
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap.insert(pair<string, Place*>(place->placeId(), place));
	}

	PersonsCreator pCreator("../test_data/people.csv", &placeMap, 7);
	obs->create<Person>(14, pCreator);

	AgentSet<Person> persons;
	obs->get(persons);

	// no matter the time or the day, should be at home as
	// no activities.
	for (ASIter iter = persons.begin(); iter != persons.end(); ++iter) {
		Person* p = (*iter);
		p->goToHome();
		for (int i = 0; i <= 24; ++i) {
			p->performActivity(i, true);
			BOOST_REQUIRE_EQUAL(p->currentPlace()->placeType(), "Household");
		}
	}

	// create the activities
	ActivityCreator actCreator;
	map<string, vector<Activity> > map;
	actCreator.run("../test_data/activities.csv", map);

	Person* p1(0);
	Person* p2(0);

	for (ASIter iter = persons.begin(); iter != persons.end(); ++iter) {
		Person* p = (*iter);
		p->initializeActivities(map);
		if (p->personId() == "5450835") p1 = p;
		else if (p->personId() == "5450836") p2 = p;
	}

	BOOST_REQUIRE(p1 != 0);
	BOOST_REQUIRE(p2 != 0);

	p1->performActivity(14, true);
	BOOST_REQUIRE_EQUAL(p1->currentPlace()->placeType(), "School");
	// gone back home
	p1->performActivity(17, true);
	BOOST_REQUIRE_EQUAL(p1->currentPlace()->placeType(), "Household");
	// go to work
	p1->performActivity(19, true);
	BOOST_REQUIRE_EQUAL(p1->currentPlace()->placeType(), "Workplace");

	p2->performActivity(14, false);
	BOOST_REQUIRE_EQUAL(p2->currentPlace()->placeType(), "School");
	// gone back home
	p2->performActivity(17, false);
	BOOST_REQUIRE_EQUAL(p2->currentPlace()->placeType(), "Household");
	// go to work
	p2->performActivity(19, false);
	BOOST_REQUIRE_EQUAL(p2->currentPlace()->placeType(), "Workplace");
}

BOOST_AUTO_TEST_SUITE_END()

