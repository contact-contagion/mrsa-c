/*
 * PersonsCreator.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <vector>

#include "repast_hpc/Utilities.h"

#include "PersonsCreator.h"

namespace mrsa {

using namespace std;
using namespace repast;

PersonsCreator::PersonsCreator(const string& file, map<string, Place*>* map) :
		reader(file), places(map) {
	// skip the first line
	vector<string> vec;
	reader.next(vec);
}

PersonsCreator::PersonsCreator(const PersonsCreator& creator) :
		reader(creator.reader), places(creator.places) {
	// skip the first line
	vector<string> vec;
	reader.next(vec);
}

PersonsCreator::~PersonsCreator() {
}

Place* PersonsCreator::findPlace(const string id) {
	string val = trim(id);
	if (val.length() > 0) {
		map<string, Place*>::iterator iter = places->find(val);
		if (iter != places->end()) {
			return iter->second;
		}
	}

	return 0;
}

Person* PersonsCreator::operator()(repast::AgentId id,
		repast::relogo::Observer* obs) {
	vector<string> vec;
	reader.next(vec);

	const string& hh_id = vec[HH_ID_IDX];
	Place* home = findPlace(hh_id);
	const string& school_id = vec[SCHOOL_ID_IDX];
	Place* school = findPlace(school_id);
	const string& gq_id = vec[GQ_ID_IDX];
	Place* group_quarters = findPlace(gq_id);
	const string& work_id = vec[WORK_ID_IDX];
	Place* work = findPlace(work_id);

	return new Person(id, obs, vec, home, group_quarters, work, school);
}

}
