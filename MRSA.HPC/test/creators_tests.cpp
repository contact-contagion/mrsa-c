#define BOOST_TEST_DYN_LINK

#include <sstream>
#include <boost/test/unit_test.hpp>

#include "../src/PlaceCreator.h"
#include "../src/PersonsCreator.h"
#include "../src/ActivityCreator.h"
#include "../src/Person.h"
#include "../src/utility.h"

#include "ObserverSetup.h"

using namespace mrsa;
using namespace std;
using namespace repast::relogo;

std::string intToString(int i) {
	std::stringstream out;
	out << i;
	return out.str();
}

BOOST_FIXTURE_TEST_SUITE(creator_tests, ObserverSetup)
;

BOOST_AUTO_TEST_CASE(place_creator) {
	PlaceCreator creator;
	vector<Place*> places;
	Properties props("../config/model.props");
	load_risks(props);
	creator.run("../test_data/places2.csv", props, places);

	BOOST_REQUIRE_EQUAL(places.size(), (size_t)9);

	vector<string> vec;
	CSVReader reader("../test_data/places2.csv");
	reader.skip(1);
	for (unsigned int i = 0; i < 9; ++i) {
		reader.next(vec);
		BOOST_CHECK_EQUAL((places[i])->placeId(), vec[0]);
		string type = vec[1];
		// lower case the type for easier, less error prone comparisons
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
		BOOST_CHECK_EQUAL((places[i])->placeType(), type);
		BOOST_CHECK_EQUAL(places[i]->zipCode(), strToUInt(vec[6]));
	}
}

Person* find_person(AgentSet<Person>& persons, const string& id) {
	for (size_t i = 0; i < persons.size(); ++i) {
		if (persons[i]->personId() == id) return persons[i];
	}
	return 0;
}

BOOST_AUTO_TEST_CASE(people_creator) {
	PlaceCreator creator;
	vector<Place*> places;
	Properties props("../config/model.props");
	load_risks(props);
	creator.run("../test_data/places2.csv", props, places);

	std::map<std::string, Place*> placeMap;
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap.insert(pair<string, Place*>(place->placeId(), place));
	}

	PersonsCreator pCreator("../test_data/persons2.csv", &placeMap, 7.0f, 10, 30);
	obs->create<Person>(9, pCreator);

	AgentSet<Person> persons;
	obs->get(persons);
	BOOST_REQUIRE_EQUAL(persons.size(), 9);

	vector<string> vec;
	CSVReader reader("../test_data/persons2.csv");
	reader.skip(1);

	for (int i = 0; i < 9; ++i) {
		reader.next(vec);
		Person* p = find_person(persons, vec[0]);
		BOOST_REQUIRE_NE((void*)0, p);
		BOOST_REQUIRE_EQUAL(p->zipCode(), p->household()->zipCode());
		BOOST_REQUIRE_EQUAL(p->household()->placeId(), vec[1]);
	}

	/*
	std::set<string> expected;
	for (int i = 0; i < 14; ++i) {
		int id = 5450835 + i;
		expected.insert(intToString(id));
	}

	for (int i = 0; i < 14; ++i) {
		Person* person = persons[i];
		std::set<string>::iterator iter;
		iter = expected.find(person->personId());
		BOOST_REQUIRE(iter != expected.end());
		expected.erase(iter);
	}
	*/
}

BOOST_AUTO_TEST_CASE(activity_creator) {

	typedef map<string, vector<Activity> >::iterator MapIter;
	ActivityCreator creator;
	map<string, vector<Activity> > map;
	creator.run("../test_data/activities.csv", map);

	BOOST_REQUIRE_EQUAL(map.size(), 2);
	MapIter iter = map.find("6203-182-1997-1");
	BOOST_REQUIRE(iter != map.end());

	vector<Activity>& acts1 = iter->second;
	BOOST_REQUIRE_EQUAL(acts1.size(), 3);
	Activity act = acts1[0];
	BOOST_REQUIRE_EQUAL(act.id(), "6203-182-1997-1");
	BOOST_REQUIRE_EQUAL(act.getPlaceType(), "School");

	act = acts1[1];
	BOOST_REQUIRE_EQUAL(act.id(), "6203-182-1997-1");
	BOOST_REQUIRE_EQUAL(act.getPlaceType(), "Household");

	act = acts1[2];
	BOOST_REQUIRE_EQUAL(act.id(), "6203-182-1997-1");
	BOOST_REQUIRE_EQUAL(act.getPlaceType(), "Workplace");

	iter = map.find("6203-182-1997-2");
	BOOST_REQUIRE(iter != map.end());
	vector<Activity>& acts2 = iter->second;
	BOOST_REQUIRE_EQUAL(acts2.size(), 3);
	act = acts2[0];
	BOOST_REQUIRE_EQUAL(act.id(), "6203-182-1997-2");
	BOOST_REQUIRE_EQUAL(act.getPlaceType(), "School");

	act = acts2[1];
	BOOST_REQUIRE_EQUAL(act.id(), "6203-182-1997-2");
	BOOST_REQUIRE_EQUAL(act.getPlaceType(), "Household");

	act = acts2[2];
	BOOST_REQUIRE_EQUAL(act.id(), "6203-182-1997-2");
	BOOST_REQUIRE_EQUAL(act.getPlaceType(), "Other");
}
BOOST_AUTO_TEST_SUITE_END()
