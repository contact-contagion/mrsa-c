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

shared_ptr<PlaceStayManager> create_prison_stay(shared_ptr<PlaceStayManager>& no_stay_manager, float prison_prob, int duration) {
	Random* random = Random::instance();
	PrisonStayManager* manager(0);
	// one try per year
	for (int i = 0; i < 10; ++i) {
		if(random->nextDouble() <= prison_prob) {
			if (manager == 0) {
				manager = new PrisonStayManager();
			}
			manager->createStayFor(i, duration);
		}
	}

	if (manager != 0) return shared_ptr<PlaceStayManager>(manager);
	else return no_stay_manager;
}


PersonsCreator::PersonsCreator(const string& file, map<string, Place*>* map,
		float min_infection_duration) :
		reader(file), places(map), min_infection_duration_(min_infection_duration),
		no_stay_manager(new NoStayManager()){

	init();
}

// copy constructor
PersonsCreator::PersonsCreator(const PersonsCreator& creator) :
		reader(creator.reader), places(creator.places), min_infection_duration_(
				creator.min_infection_duration_), no_stay_manager(new NoStayManager()) {
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

bool flip = true;

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

	// TODO remove when zip code info is in Persons file
	if (flip) {
		vec[ZIP_CODE_IDX] = "60615";
		flip = false;
	} else {
		vec[ZIP_CODE_IDX] = "60625";
		flip = true;
	}

	// TODO replace with real values when available
	//places.prison = findPlace("G170312716001P1");
	float prison_prob = 0;
	int prison_duration = 30;

	// create the Person
	return new Person(id, obs, vec, places, create_hospital_stay(vec, no_stay_manager), create_prison_stay(no_stay_manager,
			prison_prob, prison_duration), min_infection_duration_);
}

}
