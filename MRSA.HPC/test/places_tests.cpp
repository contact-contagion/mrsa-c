#define BOOST_TEST_DYN_LINK

#include <sstream>
#include <boost/test/unit_test.hpp>

#include "../src/PlaceCreator.h"
#include "../src/PersonsCreator.h"
#include "../src/ActivityCreator.h"
#include "../src/Person.h"
#include "../src/utility.h"
#include "../src/Prison.h"

#include "ObserverSetup.h"

using namespace mrsa;
using namespace std;
using namespace repast::relogo;

BOOST_FIXTURE_TEST_SUITE(places_tests, ObserverSetup)
;

BOOST_AUTO_TEST_CASE(comp_places) {
	PlaceCreator creator;
	vector<Place*> places;
	Properties props("../config/model.props");
	load_risks(props);
	creator.run(props.getProperty("places.file"), props, places);
	std::map<std::string, Place*> placeMap;
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap.insert(pair<string, Place*>(place->placeId(), place));
	}

	PersonsCreator pCreator(props.getProperty("persons.file"), &placeMap, 7.0f);
	obs->create<Person>(400, pCreator);

	AgentSet<Person> persons;
	obs->get(persons);
	BOOST_REQUIRE_EQUAL(persons.size(), (size_t)400);

	std::vector<string> data;
	data.push_back("1");
	data.push_back("Prison");
	data.push_back("");
	data.push_back("NA");
	data.push_back("41");
	data.push_back("87");
	Prison* prison = new Prison(data, Risk());

	BOOST_REQUIRE_EQUAL(prison->componentCount(), 1);
	for (int i = 0; i < 31; ++i) {
		prison->addPerson(persons.at(i), 0);
	}

	BOOST_REQUIRE_EQUAL(prison->componentCount(), 2);
	BOOST_REQUIRE_EQUAL(prison->componentSize(0), 30);
	BOOST_REQUIRE_EQUAL(prison->componentSize(1), 1);

	prison->reset();
	BOOST_REQUIRE_EQUAL(prison->componentCount(), 2);
	BOOST_REQUIRE_EQUAL(prison->componentSize(0), 0);
	BOOST_REQUIRE_EQUAL(prison->componentSize(1), 0);

	// 31st person should go back into the 2nd component
	prison->addPerson(persons.at(30), 0);
	BOOST_REQUIRE_EQUAL(prison->componentCount(), 2);
	BOOST_REQUIRE_EQUAL(prison->componentSize(0), 0);
	BOOST_REQUIRE_EQUAL(prison->componentSize(1), 1);

	delete prison;
}

BOOST_AUTO_TEST_SUITE_END()

