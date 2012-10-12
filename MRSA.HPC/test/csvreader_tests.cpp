#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../src/CSVReader.h"

using namespace mrsa;
using namespace std;

string expected[7][6] = { { "tucaseid", "place_type", "start_time", "stop_time", "activity_risk",
		"activity_type" }, { "6203-182-1997-1", "School", "800", "975", "0", "1" }, {
		"6203-182-1997-1", "Household", "0", "570", "0", "1" }, { "6203-182-1997-1", "Workplace",
		"1100", "1400", "0", "0" }, { "6203-182-1997-2", "School", "1030", "1440", "0", "1" }, {
		"6203-182-1997-2", "Household", "0", "570", "0", "1" }, { "6203-182-1997-2", "Other", "570",
		"720", "0", "0" } };

BOOST_AUTO_TEST_SUITE(csvreader_tests)

BOOST_AUTO_TEST_CASE(csv_reader_unix_line_ending) {

	CSVReader reader("../test_data/activities.csv");
	vector<string> vec;

	for (int i = 0; i < 7; ++i) {
		BOOST_REQUIRE(reader.next(vec));
		for (int j = 0; j < 6; ++j) {
			BOOST_REQUIRE(vec[j] == expected[i][j]);
		}
	}
}

BOOST_AUTO_TEST_CASE(csv_reader_osx_line_ending) {

	CSVReader reader("../test_data/activities_osx_line_endings.csv");
	vector<string> vec;

	for (int i = 0; i < 7; ++i) {
		BOOST_REQUIRE(reader.next(vec));
		for (int j = 0; j < 6; ++j) {
			BOOST_REQUIRE(vec[j] == expected[i][j]);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()


