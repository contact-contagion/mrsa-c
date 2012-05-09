/*
 * PersonsCreator.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <vector>

#include "repast_hpc/Utilities.h"

#include "PersonsCreator.h"
#include "Constants.h"

namespace mrsa {

using namespace std;
using namespace repast;

const string RND_INDEX_NAME = "household.index";

PersonsCreator::PersonsCreator(const string& file, map<string, Place*>* map,
		float min_infection_duration) :
		reader(file), places(map), min_infection_duration_(min_infection_duration), households() {

	init();

	for (std::map<string, Place*>::iterator iter = map->begin(); iter != map->end(); ++iter) {
		Place* p = iter->second;
		string type = p->placeType();
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
		if (type == HOUSEHOLD_TYPE)
			households.push_back(p);
	}

	_IntUniformGenerator gen(repast::Random::instance()->engine(),
			boost::uniform_int<>(0, households.size() - 1));
	repast::Random::instance()->putGenerator(RND_INDEX_NAME,
			new DefaultNumberGenerator<_IntUniformGenerator>(gen));

}

// copy constructor
PersonsCreator::PersonsCreator(const PersonsCreator& creator) :
		reader(creator.reader), places(creator.places), min_infection_duration_(
				creator.min_infection_duration_), households(creator.households) {
	init();
}

void PersonsCreator::init() {
	// skip the header line, so the first line read is a person.
	vector<string> vec;
	reader.next(vec);

}

PersonsCreator::~PersonsCreator() {
}

Place* PersonsCreator::findPlace(const string id) {
	string val = trim(id);
	// if the id is valid then look up the place.
	if (val.length() > 0) {
		map<string, Place*>::iterator iter = places->find(val);
		if (iter != places->end()) {
			return iter->second;
		}
	}
	return 0;
}

// creates a Person from a line in the persons data file
Person* PersonsCreator::operator()(repast::AgentId id, repast::relogo::Observer* obs) {

	vector<string> vec;
	// read the line into the vector.
	reader.next(vec);

	// look up the Persons' places in the map.
	const string& hh_id = vec[HH_ID_IDX];
	Place* home = findPlace(hh_id);
	const string& school_id = vec[SCHOOL_ID_IDX];
	Place* school = findPlace(school_id);
	const string& gq_id = vec[GQ_ID_IDX];
	Place* group_quarters = findPlace(gq_id);
	const string& work_id = vec[WORK_ID_IDX];
	Place* work = findPlace(work_id);

	NumberGenerator* gen = repast::Random::instance()->getGenerator(RND_INDEX_NAME);
	// randomly select a household from the list of households
	// as the "other_home"
	Place* other_home = households[(int) gen->next()];
	// make sure other_home is not home.
	while (other_home == home) {
		other_home = households[(int) gen->next()];
	}

	// create the Person
	return new Person(id, obs, vec, home, other_home, group_quarters, work, school, min_infection_duration_);
}

}
