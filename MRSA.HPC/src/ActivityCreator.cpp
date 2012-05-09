/*
 * ActivityCreator.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: nick
 */

#include "ActivityCreator.h"
#include "CSVReader.h"

namespace mrsa {

using namespace std;

ActivityCreator::ActivityCreator() {
}

ActivityCreator::~ActivityCreator() {
}

void ActivityCreator::run(const std::string& file, std::map<std::string, std::vector<Activity> >& map) {
	typedef  std::map<std::string, std::vector<Activity> >::iterator MapIter;

	// use a CSVReader to read the file.
	CSVReader reader(file);
	vector<string> vec;
	// skip the first line
	reader.next(vec);
	// read each row in the file into vec
	while (reader.next(vec)) {
		// create an activity from that row
		Activity activity(vec);
		// find the map entry with that tucaseid
		// if it doesn't exist make a vector inser that into the map
		MapIter iter = map.find(activity.id());
		if (iter == map.end()) {
			vector<Activity> activities;
			activities.push_back(activity);
			map.insert(pair<string, vector<Activity> >(activity.id(), activities));
		} else {
			// vector with this tucaseid does exist so get a reference to it
			// and put that activity in it.
			vector<Activity>& activities = iter->second;
			activities.push_back(activity);
		}
	}
}

} /* namespace mrsa */
