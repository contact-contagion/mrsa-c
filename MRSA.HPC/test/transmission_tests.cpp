#define BOOST_TEST_DYN_LINK
//#define BOOST_TEST_MODULE transmission_tests

#include <boost/test/unit_test.hpp>

#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Properties.h"

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

	TAParameters params;
	params.a = 0;
	params.b = 0;
	params.e = 0;

	TransmissionAlgorithm::initialize(params);
	// a is 0, so starting with UNCOLONIZED should always return UNCOLONIZED
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->runUncolonized(0, 0, 0) == UNCOLONIZED);
	}

	params.a = 1;
	TransmissionAlgorithm::initialize(params);
	ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->runUncolonized(1, 0, 1) == COLONIZED);
	}

	params.a = 0.5;
	TransmissionAlgorithm::initialize(params);
	// a is .5 and risk is 2 so double a is one so always move to colonized.
	ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->runUncolonized(2, 1, 0) == COLONIZED);
	}

}

BOOST_AUTO_TEST_CASE(colonized) {

	TAParameters params;
	params.a = 0;
	params.b = 1;
	params.e = 0;
	TransmissionAlgorithm::initialize(params);
	// b is one so should always move from colonized to infected
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->runColonized(1, 1) == INFECTED);
	}

	params.b = 0;
	params.e = 1;
	TransmissionAlgorithm::initialize(params);
	// b is 0 and e is one so should always return to uncolonized
	ta = TransmissionAlgorithm::instance();
	for (int i = 0; i < 2000; ++i) {
		BOOST_REQUIRE(ta->runColonized(1, 1) == UNCOLONIZED);
	}
}


//BOOST_AUTO_TEST_CASE(infected) {
//
//	double c = 0;
//	double d = 1;
//	TransmissionAlgorithm::initialize(0, 0, c, d, 0);
//	// d is one so should always move from infected to uncolonized
//	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
//	for (int i = 0; i < 2000; ++i) {
//		BOOST_REQUIRE(ta->runInfected() == UNCOLONIZED);
//	}
//
//	c = 1;
//	d = 0;
//	TransmissionAlgorithm::initialize(0, 0, c, d, 0);
//	// c is 1 and d is 0 so should move to colonized
//	ta = TransmissionAlgorithm::instance();
//	for (int i = 0; i < 2000; ++i) {
//		BOOST_REQUIRE(ta->runInfected() == COLONIZED);
//	}
//}


struct PlacePredicate {

	std::string id;

	PlacePredicate(std::string hh_id) : id(hh_id) {}

	bool operator()(Place* place) {
		return place->placeId() == id;
	}
};

void deletePlaces(std::vector<Place*>& places) {
	for (int i = 0, n = places.size(); i < n; i++) {
		delete places[i];
	}
}

void createPersons(Observer* obs, AgentSet<Person>& persons, std::vector<Place*>& places, int count = 14) {
	PlaceCreator creator;
	Properties props("../config/model.props");
	creator.run("../test_data/places.csv", props, places);
	std::map<std::string, Place*> placeMap;
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap.insert(pair<string, Place*>(place->placeId(), place));
	}

	PersonsCreator pCreator("../test_data/people.csv", &placeMap, 2);
	obs->create<Person>(count, pCreator);
	obs->get(persons);
}

BOOST_AUTO_TEST_CASE(workplace) {
	AgentSet<Person> persons;
	std::vector<Place*> places;
	createPersons(obs, persons, places);

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
		place->addPerson(p, 0);
	}

	// a is 10 because the risk is .1f, so draw should be 1 and
	// so 4 should be colonized.
	TAParameters params;
	params.a = 10;
	params.b = 0;
	params.e = 0;
	TransmissionAlgorithm::initialize(params);
	place->runTransmission();

	int colonized_count = 0;
	for (ASIter iter = persons.begin(); iter != persons.end(); ++iter) {
		if ((*iter)->status() == COLONIZED) ++colonized_count;
	}

	BOOST_REQUIRE(colonized_count == 4);

	deletePlaces(places);
}

BOOST_AUTO_TEST_CASE(school) {
	AgentSet<Person> persons;
	std::vector<Place*> places;
	createPersons(obs, persons, places);

	vector<Person*> forties;
	vector<Person*> twelves;
	for (ASIter iter = persons.begin(); iter != persons.end(); ++iter) {
		Person*p = (*iter);
		if (p->age() == 40) forties.push_back(p);
		else if(p->age() == 12) twelves.push_back(p);
	}

	Place* place = *(find_if(places.begin(), places.end(), PlacePredicate("170993001118")));
	BOOST_REQUIRE(place->placeType() == "school");

	(twelves[0])->updateStatus(INFECTED);
	for (vector<Person*>::iterator iter = forties.begin(); iter != forties.end(); ++iter) {
		place->addPerson(*iter, 0);
	}

	for (vector<Person*>::iterator iter = twelves.begin(); iter != twelves.end(); ++iter) {
		place->addPerson(*iter, 0);
	}

	TAParameters params;
	params.a = 1;
	params.b = 0;
	params.e = 0;
	// a is 1, so draw should be 1 and so tweleves should be colonized
	// others should be uncolonized
	TransmissionAlgorithm::initialize(params);
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
		place->addPerson(*iter, 0);
	}

	for (vector<Person*>::iterator iter = twelves.begin(); iter != twelves.end(); ++iter) {
		(*iter)->updateStatus(UNCOLONIZED);
		place->addPerson(*iter, 0);
	}

	place->runTransmission();

	BOOST_REQUIRE_EQUAL(twelves[0]->status(), UNCOLONIZED);
	BOOST_REQUIRE_EQUAL(twelves[1]->status(), UNCOLONIZED);
	BOOST_REQUIRE_EQUAL(twelves[2]->status(), UNCOLONIZED);
	BOOST_REQUIRE_EQUAL(twelves[3]->status(), UNCOLONIZED);

	BOOST_REQUIRE_EQUAL(forties[1]->status(), COLONIZED);
	BOOST_REQUIRE_EQUAL(forties[2]->status(), COLONIZED);

	deletePlaces(places);
}

BOOST_AUTO_TEST_CASE(household) {
	AgentSet<Person> persons;
	std::vector<Place*> places;
	createPersons(obs, persons, places, 2);

	Person* p1 = persons[0];
	p1->updateStatus(UNCOLONIZED);
	BOOST_REQUIRE(p1->status() == UNCOLONIZED);
	Person* p2 = persons[1];
	p2->updateStatus(INFECTED);
	BOOST_REQUIRE(p2->status() == INFECTED);

	Place* hh = *(find_if(places.begin(), places.end(), PlacePredicate("2038461")));

	repast::Properties props;
	mrsa::Parameters::initialize(props);

	// household now has one uncolonized and one infected person.
	hh->addPerson(p1, 0);
	hh->addPerson(p2, 0);

	TAParameters params;
	params.a = 0;
	params.b = 0;
	params.e = 0;
	// a is 0, so p1 should remain uncolonized.
	TransmissionAlgorithm::initialize(params);
	hh->runTransmission();
	BOOST_REQUIRE(p1->status() == UNCOLONIZED);

	// a is 1, so p1 should be colonized
	params.a = 1;
	TransmissionAlgorithm::initialize(params);
	hh->runTransmission();
	BOOST_REQUIRE(p1->status() == COLONIZED);

	hh->reset();
	p1->updateStatus(UNCOLONIZED);
	p2->updateStatus(INFECTED);
	hh->addPerson(p1, 0);
	hh->addPerson(p2, 0);

	params.a = 0.5;
	// a is .5 but risk for household is 2 so should be
	// colonized
	TransmissionAlgorithm::initialize(params);
	hh->runTransmission();
	BOOST_REQUIRE(p1->status() == COLONIZED);

	params.a = 0;
	params.b = 1;
	TransmissionAlgorithm::initialize(params);
	hh->reset();
	hh->addPerson(p1, 0);
	hh->runTransmission();
	BOOST_REQUIRE(p1->status() == INFECTED);

	// run the schedule for two ticks, to increment the tick count by 3.
	// this means that the minimum disease duration has passed and p1
	// can move to uncolonized
	repast::RepastProcess::instance()->getScheduleRunner().scheduleStop(3);
	repast::RepastProcess::instance()->getScheduleRunner().run();

//	double c = 0;
//	double d = 1;
//	TransmissionAlgorithm::initialize(0, 0, c, d, 0);
//	hh->reset();
//	hh->addPerson(p1);
//	hh->runTransmission();
//	BOOST_REQUIRE(p1->status() == UNCOLONIZED);

	deletePlaces(places);
}

BOOST_AUTO_TEST_CASE(activity_test) {
	// create the persons
	AgentSet<Person> persons;
	std::vector<Place*> places;
	createPersons(obs, persons, places);

	Calendar cal;


	// no matter the time or the day, should be at home as
	// no activities.
	for (ASIter iter = persons.begin(); iter != persons.end(); ++iter) {
		Person* p = (*iter);
		p->goToHome();
		for (int i = 0; i <= 24; ++i) {
			cal.hour_of_day = i;
			p->performActivity(cal);
			BOOST_REQUIRE_EQUAL(p->currentPlace()->placeType(), "household");
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

	cal.hour_of_day = 14;
	p1->performActivity(cal);
	BOOST_REQUIRE_EQUAL(p1->currentPlace()->placeType(), "school");

	// gone back home
	cal.hour_of_day = 17;
	p1->performActivity(cal);
	BOOST_REQUIRE_EQUAL(p1->currentPlace()->placeType(), "household");

	// go to work
	cal.hour_of_day = 19;
	p1->performActivity(cal);
	BOOST_REQUIRE_EQUAL(p1->currentPlace()->placeType(), "workplace");

	cal.hour_of_day = 14;
	p2->performActivity(cal);
	BOOST_REQUIRE_EQUAL(p2->currentPlace()->placeType(), "school");

	// gone back home
	cal.hour_of_day = 17;
	p2->performActivity(cal);
	BOOST_REQUIRE_EQUAL(p2->currentPlace()->placeType(), "household");

	// go to work
	cal.hour_of_day = 19;
	p2->performActivity(cal);
	BOOST_REQUIRE_EQUAL(p2->currentPlace()->placeType(), "workplace");

	deletePlaces(places);
}

// this test will fail if not run by itself
BOOST_AUTO_TEST_CASE(disease_update_test) {
	AgentSet<Person> persons;
	std::vector<Place*> places;
	createPersons(obs, persons, places);

	Person* p1 = persons[0];
	p1->updateStatus(INFECTED);

	Person* p2 = persons[1];
	p2->updateStatus(COLONIZED);

	Person* p3 = persons[2];

	// infected then drop to colonized
	p3->updateStatus(INFECTED);
	p3->updateStatus(COLONIZED);

	repast::RepastProcess::instance()->getScheduleRunner().scheduleStop(2);
	repast::RepastProcess::instance()->getScheduleRunner().run();

	p1->updateStatus(UNCOLONIZED);
	p2->updateStatus(INFECTED);
	p3->updateStatus(UNCOLONIZED);

	deletePlaces(places);
}

BOOST_AUTO_TEST_SUITE_END()

