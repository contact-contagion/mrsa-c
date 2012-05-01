/*
 * Tests for data integrity. These can be compiled with the unit tests.
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
		placeCreator.run(file_prefix + "places.csv", places);

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
