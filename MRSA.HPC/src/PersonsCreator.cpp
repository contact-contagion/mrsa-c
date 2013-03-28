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
#include "StayManager.h"
#include "PrisonStayManager.h"

#include "FileOutput.h"

namespace mrsa {

using namespace std;
using namespace repast;
using namespace boost;

shared_ptr<PlaceStayManager> create_hospital_stay(const vector<string>& data, shared_ptr<PlaceStayManager>& no_stay_manager) {
	unsigned int y1_length = strToUInt(data[H_NIGHTS_1]);
	unsigned int y2_length = strToUInt(data[H_NIGHTS_2]);
	unsigned int y3_length = strToUInt(data[H_NIGHTS_3]);
	unsigned int y4_length = strToUInt(data[H_NIGHTS_4]);
	unsigned int y5_length = strToUInt(data[H_NIGHTS_5]);

	if (y1_length == 0 && y2_length == 0 && y3_length == 0 && y4_length == 0 && y5_length == 0) {
		return no_stay_manager;
	} else {
		return shared_ptr<PlaceStayManager>(new HospitalStayManager(y1_length, y2_length, y3_length, y4_length, y5_length));
	}
}

PlaceStayManager* create_prison_stay(double prison_prob, unsigned int min_jail_duration,
		unsigned int max_jail_duration) {
	Random* random = Random::instance();
	PrisonStayManager* manager(0);
	// one try per year, 10 years
	for (unsigned int i = 0; i < 10; ++i) {
		if(random->nextDouble() <= prison_prob) {
			if (manager == 0) {
				manager = new PrisonStayManager();
			}

			unsigned int duration = (int)Random::instance()->createUniIntGenerator(min_jail_duration, max_jail_duration).next();
			manager->createStayFor(0, duration);
		}
	}

	return manager;
}


PersonsCreator::PersonsCreator(const string& file, map<string, Place*>* map,
		float min_infection_duration, unsigned int min_jail_duration, unsigned int max_jail_duration) :
		reader(file), places(map), min_infection_duration_(min_infection_duration),
		min_jail_duration_(min_jail_duration), max_jail_duration_(max_jail_duration),
		no_stay_manager(new NoStayManager()){

	init();
}

// copy constructor
PersonsCreator::PersonsCreator(const PersonsCreator& creator) :
		reader(creator.reader), places(creator.places), min_infection_duration_(
				creator.min_infection_duration_), min_jail_duration_(creator.min_jail_duration_),
				max_jail_duration_(creator.max_jail_duration_), no_stay_manager(new NoStayManager()) {
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
	if (val.length() > 0 && val != "NA") {
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

	Places places;

	// look up the Persons' places in the map.
	const string& hh_id = vec[HH_ID_IDX];
	places.household = findPlace(hh_id);

	const string& school_id = vec[SCHOOL_ID_IDX];
	places.school = findPlace(school_id);
	const string& gq_id = vec[GQ_ID_IDX];
	places.group_quarters = findPlace(gq_id);
	const string& work_id = vec[WORK_ID_IDX];
	places.work = findPlace(work_id);
	const string& gym_id = vec[GYM_ID_IDX];
	places.gym = findPlace(gym_id);
	const string& daycare_id = vec[DAYCARE_ID_IDX];
	places.daycare = findPlace(daycare_id);
	const string& hosp_id = vec[HOSPITAL_ID_IDX];
	places.hospital = findPlace(hosp_id);

	for (int i = OTHER_H_START_IDX; i <= OTHER_H_END_IDX; ++i) {
		const string& other_hh_id = vec[i];
		places.other_households.push_back(findPlace(other_hh_id));
	}


	unsigned int zip = 0;
	if (places.household != 0) zip = places.household->zipCode();
	else if (places.group_quarters != 0) zip = places.group_quarters->zipCode();
	stringstream ss;
	ss << zip;
	vec.push_back(ss.str());

	// TODO replace with real value when available
	if (places.prison == 0) places.prison = findPlace("G170312716001P1");
	double prison_prob = strToDouble(vec[JAIL_IDX]);
	PlaceStayManager* manager(0);
	if (prison_prob > 0) {
		manager = create_prison_stay(prison_prob, min_jail_duration_, max_jail_duration_);
	}

	if (manager == 0)
		return new Person(id, obs, vec, places, create_hospital_stay(vec, no_stay_manager),
					no_stay_manager, min_infection_duration_);
	else {
		return new Person(id, obs, vec, places, create_hospital_stay(vec, no_stay_manager),
					shared_ptr<PlaceStayManager>(manager), min_infection_duration_);
	}

	// create the Person

}

}
