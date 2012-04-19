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

void ActivityCreator::run(const std::string& file, std::map<std::string, std::vector<Activity*>* >& map) {
	typedef  std::map<std::string, std::vector<Activity*>* >::iterator MapIter;

	CSVReader reader(file);
	vector<string> vec;
	// skip the first line
	reader.next(vec);
	while (reader.next(vec)) {
		Activity* activity = new Activity(vec);
		MapIter iter = map.find(activity->id());
		if (iter == map.end()) {
			vector<Activity*>* activities = new vector<Activity*>();
			activities->push_back(activity);
			map.insert(pair<string, vector<Activity*>* >(activity->id(), activities));
		} else {
			vector<Activity*>* activities = iter->second;
			activities->push_back(activity);
		}

	}

}

} /* namespace mrsa */
