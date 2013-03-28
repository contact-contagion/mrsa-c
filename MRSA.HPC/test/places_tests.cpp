#define BOOST_TEST_DYN_LINK

#include <sstream>
#include <boost/test/unit_test.hpp>
#include <cstdio>

#include "../src/PlaceCreator.h"
#include "../src/PersonsCreator.h"
#include "../src/ActivityCreator.h"
#include "../src/Person.h"
#include "../src/utility.h"
#include "../src/Prison.h"
#include "../src/TransmissionEventRecorder.h"
#include "../src/RegionMap.h"

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

	PersonsCreator pCreator(props.getProperty("persons.file"), &placeMap, 7.0f, 10, 30);
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

BOOST_AUTO_TEST_CASE(init_file_reader) {
	Properties props("../config/model.props");
	props.putProperty(COL_INFECT_INIT_FILE, "../test_data/colonization_infection_init.csv");
	load_init_col_inf_fractions(props);

	std::vector<unsigned int> zips;
	RegionMap::instance()->zipCodes(zips);

	for (size_t i = 0; i < zips.size(); ++i) {
		std::stringstream ss;
		ss << zips[i];
		BOOST_REQUIRE_EQUAL("0.03", props.getProperty(ss.str() + "_colonized"));
		BOOST_REQUIRE_EQUAL("0.005", props.getProperty(ss.str() + "_infected"));
	}
}

BOOST_AUTO_TEST_CASE(event_recorder) {
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

	PersonsCreator pCreator(props.getProperty("persons.file"), &placeMap, 7.0f, 10, 30);
	obs->create<Person>(2, pCreator);

	AgentSet<Person> persons;
	obs->get(persons);

	remove("events.csv");
	TransmissionEventRecorder::initialize("events.csv");
	TransmissionEventRecorder* recorder = TransmissionEventRecorder::instance();
	recorder->recordEvent(1, persons[0], places[0], U_TO_C);
	recorder->recordEvent(1, persons[1], places[1], I_TO_C);
	delete recorder;

	CSVReader reader("events.csv");
	std::vector<string> data;
	reader.next(data);
	BOOST_REQUIRE_EQUAL(data.size(), 6);
	//tick, person_id, place_id, place_type, zip_code, event_type
	BOOST_REQUIRE_EQUAL(data[0], "tick");
	BOOST_REQUIRE_EQUAL(data[1], "person_id");
	BOOST_REQUIRE_EQUAL(data[2], "place_id");
	BOOST_REQUIRE_EQUAL(data[3], "place_type");
	BOOST_REQUIRE_EQUAL(data[4], "zip_code");
	BOOST_REQUIRE_EQUAL(data[5], "event_type");

	reader.next(data);
	BOOST_REQUIRE_EQUAL(data.size(), 6);
	//tick, person_id, place_id, place_type, zip_code, event_type
	BOOST_REQUIRE_EQUAL(data[0], "1");
	BOOST_REQUIRE_EQUAL(data[1], persons[0]->personId());
	BOOST_REQUIRE_EQUAL(data[2], places[0]->placeId());
	BOOST_REQUIRE_EQUAL(data[3], places[0]->placeType());
	//BOOST_REQUIRE_EQUAL(data[4], persons[0]->zipCode());
	BOOST_REQUIRE_EQUAL(data[5], U_TO_C);

	reader.next(data);
	BOOST_REQUIRE_EQUAL(data.size(), 6);
	//tick, person_id, place_id, place_type, zip_code, event_type
	BOOST_REQUIRE_EQUAL(data[0], "1");
	BOOST_REQUIRE_EQUAL(data[1], persons[1]->personId());
	BOOST_REQUIRE_EQUAL(data[2], places[1]->placeId());
	BOOST_REQUIRE_EQUAL(data[3], places[1]->placeType());
	//BOOST_REQUIRE_EQUAL(data[4], "0");
	BOOST_REQUIRE_EQUAL(data[5], I_TO_C);

	BOOST_REQUIRE_EQUAL(reader.next(data), false);
}
BOOST_AUTO_TEST_SUITE_END()

