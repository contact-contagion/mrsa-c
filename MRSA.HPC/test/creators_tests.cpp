#define BOOST_TEST_DYN_LINK

#include <sstream>
#include <boost/test/unit_test.hpp>


#include "../src/PlaceCreator.h"
#include "../src/PersonsCreator.h"
#include "../src/Person.h"

#include "ObserverSetup.h"


using namespace mrsa;
using namespace std;
using namespace repast::relogo;

std::string intToString(int i) {
	std::stringstream out;
	out << i;
	return out.str();
}

BOOST_FIXTURE_TEST_SUITE(creator_tests, ObserverSetup);

BOOST_AUTO_TEST_CASE(place_creator) {
	PlaceCreator creator;
	vector<Place*> places;
	creator.run("../test_data/places.csv", places);

	BOOST_REQUIRE_EQUAL(places.size(), (size_t)13);

	std::string ids[] = {"10049883", "10051140", "10052705", "1703101010000001",
		"1703101010000020", "170993001101", "170993001102", "170993001104", "170993001118", "2038461", "2038468",
		"G170313904001C01", "G170314103001C01"};

	std::string types[] = {"Hospital", "Hospital", "Hospital", "Workplace", "Workplace",
		"School", "School", "School", "School", "Household", "Household", "College Dorm",
		"College Dorm"
	};

	for (unsigned int i = 0; i < 13; ++i) {
		BOOST_CHECK_EQUAL((places[i])->placeId(), ids[i]);
		BOOST_CHECK_EQUAL((places[i])->placeType(), types[i]);
	}
}

BOOST_AUTO_TEST_CASE(people_creator) {
	PlaceCreator creator;
	vector<Place*> places;
	creator.run("../test_data/places.csv", places);
	std::map<std::string, Place*> placeMap;
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap.insert(pair<string, Place*>(place->placeId(), place));
	}

	PersonsCreator pCreator("../test_data/people.csv", &placeMap);
	obs->create<Person>(14, pCreator);

	AgentSet<Person> persons;
	obs->get(persons);

	BOOST_REQUIRE_EQUAL(persons.size(), 14);

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
}



BOOST_AUTO_TEST_SUITE_END()
