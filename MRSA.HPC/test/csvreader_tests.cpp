/*
*MRSA Model
*
*Copyright (c) 2013 University of Chicago and Argonne National Laboratory
*   All rights reserved.
*  
*   Redistribution and use in source and binary forms, with 
*   or without modification, are permitted provided that the following 
*   conditions are met:
*  
*  	 Redistributions of source code must retain the above copyright notice,
*  	 this list of conditions and the following disclaimer.
*  
*  	 Redistributions in binary form must reproduce the above copyright notice,
*  	 this list of conditions and the following disclaimer in the documentation
*  	 and/or other materials provided with the distribution.
*  
*  	 Neither the name of the Argonne National Laboratory nor the names of its
*     contributors may be used to endorse or promote products derived from
*     this software without specific prior written permission.
*  
*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
*   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
*   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
*   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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


