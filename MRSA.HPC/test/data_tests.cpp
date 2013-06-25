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

#include "../src/PlaceCreator.h"
#include "../src/ActivityCreator.h"
#include "../src/Person.h"

using namespace mrsa;
using namespace std;

typedef std::map<string, vector<Activity> >::iterator ActMapIter;

bool act_missing(vector<string>& data, map<string, vector<Activity> >& acts) {
	string id = data[TUCASE_ID_WEEKDAY_IDX];
	ActMapIter iter = acts.find(id);
	if (iter == acts.end())
		return true;

	id = data[TUCASE_ID_WEEKEND_IDX];
	iter = acts.find(id);
	return iter == acts.end();
}

bool places_missing(vector<string>& data, set<string>& places) {
	string id = data[HH_ID_IDX];
	if (places.find(id) != places.end())
		return false;

	id = data[GQ_ID_IDX];
	if (places.find(id) != places.end())
		return false;

	id = data[SCHOOL_ID_IDX];
	if (places.find(id) != places.end())
		return false;

	id = data[WORK_ID_IDX];
	return places.find(id) != places.end();
}

BOOST_AUTO_TEST_CASE(data_integrity) {

	string file_prefixes[] = { "60615", "60615_60653_60609" };

	for (int i = 0; i < 2; i++) {
		string prefix = file_prefixes[i];
		string file_prefix = "../../MRSA Inputs/subsets/" + prefix + "/" + prefix + "_";

		map<string, vector<Activity> > map;
		ActivityCreator actCreator;
		actCreator.run(file_prefix + "activities.csv", map);

		vector<Place*> places;
		PlaceCreator placeCreator;
		placeCreator.run(file_prefix + "places.csv", "../data/risk.csv", places);

		std::set<string> placeSet;
		for (vector<Place*>::iterator iter = places.begin(); iter != places.end(); ++iter) {
			Place* place = *iter;
			placeSet.insert(place->placeId());
		}

		unsigned int missing_acts = 0;
		unsigned int missing_places = 0;

		CSVReader reader(file_prefix + "people.csv");
		vector<string> vec;
		while (reader.next(vec)) {
			if (act_missing(vec, map))
				++missing_acts;
			if (places_missing(vec, placeSet))
				++missing_places;

		}

		std::cout << prefix << ", missing acts: " << missing_acts << ", missing places: "
				<< missing_places << std::endl;

		for (vector<Place*>::iterator iter = places.begin(); iter != places.end(); ++iter) {
			delete (*iter);
		}
	}
}
