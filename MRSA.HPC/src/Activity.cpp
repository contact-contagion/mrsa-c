/*
 * Activity.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: nick
 */
#include "repast_hpc/Utilities.h"
#include "repast_hpc/Random.h"

#include "Activity.h"
#include "Constants.h"

namespace mrsa {

using namespace repast;

const int ID_IDX = 0;
const int PLACE_TYPE_IDX = 1;
const int START_TIME_IDX = 2;
const int STOP_TIME_IDX = 3;
const int ACTIVITY_TYPE_IDX = 4;

// initializes the activities fields from the vector.
Activity::Activity(std::vector<std::string>& vec) :
		tucase_id(vec[ID_IDX]), place_type(vec[PLACE_TYPE_IDX]), start_time(0), end_time(0), activity_type(
				0) {

	std::string val = vec[START_TIME_IDX];
	val = trim(val);
	// minutes to hours
	start_time = strToInt(val) / 60;

	val = vec[STOP_TIME_IDX];
	val = trim(val);
	// minutes to hours
	end_time = strToInt(val) / 60;

	val = vec[ACTIVITY_TYPE_IDX];
	val = trim(val);
	activity_type = strToInt(val);
}

// copy constructor
Activity::Activity(const Activity& act) :
		tucase_id(act.tucase_id), place_type(act.place_type), start_time(act.start_time), end_time(
				act.end_time), activity_type(act.activity_type) {
}

Place* Activity::selectPlace(Places& places) const {
	if (place_type == "Household") return places.household;
	else if (place_type == "Workplace") return places.work;

	else if (place_type == "Other Household") {
		//std::cout << "going to other household" << std::endl;
		NumberGenerator* gen = repast::Random::instance()->getGenerator(OH_DIST);
		//std::cout << gen << std::endl;
		//std::cout << places_.other_households.size() << std::endl;
		return places.other_households[(int) gen->next()];
	}
	else if (place_type == "School") {
		if (places.school != 0) return places.school;
		else return places.daycare;
	}
	else if (place_type == "Group Quarters") return places.group_quarters;

	else if (place_type == "Gym") return places.gym;
	else if (place_type == "Hospital") return places.hospital;
	else {
		// unknown place type so go to household if it exists or if not
		// go to group quarters.
		if (places.household != 0) return places.household;
		else return places.group_quarters;
	}
	return 0;
}

// assignment operator
Activity& Activity::operator=(const Activity& rhs) {
	if (&rhs != this) {
		this->tucase_id = rhs.tucase_id;
		this->place_type = rhs.place_type;
		this->start_time = rhs.start_time;
		this->end_time = rhs.end_time;
		this->activity_type = rhs.activity_type;
	}

	return *this;
}

Activity::~Activity() {
}

} /* namespace mrsa */
