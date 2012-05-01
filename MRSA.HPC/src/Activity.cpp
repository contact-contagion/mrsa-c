/*
 * Activity.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: nick
 */
#include "repast_hpc/Utilities.h"

#include "Activity.h"

namespace mrsa {

using namespace repast;

const int ID_IDX = 0;
const int PLACE_TYPE_IDX = 1;
const int START_TIME_IDX = 2;
const int STOP_TIME_IDX = 3;
const int ACTIVITY_RISK_IDX = 4;
const int PLACE_RISK_IDX = 5;

Activity::Activity(std::vector<std::string>& vec) :
		tucase_id(vec[ID_IDX]), place_type(vec[PLACE_TYPE_IDX]), start_time(0), end_time(
				0), activity_risk(0), place_risk(0) {

	std::string val = vec[START_TIME_IDX];
	val = trim(val);
	// minutes to hours
	start_time = strToInt(val) / 60.0f;

	val = vec[STOP_TIME_IDX];
	val = trim(val);
	// minutes to hours
	end_time = strToInt(val) / 60.0f;

	val = vec[ACTIVITY_RISK_IDX];
	val = trim(val);
	activity_risk = strToInt(val);

	val = vec[PLACE_RISK_IDX];
	val = trim(val);
	place_risk = (float)strToDouble(val);
	if (place_risk == 0) place_risk = .1f;
}

Activity::Activity(const Activity& act) : tucase_id(act.tucase_id), place_type(act.place_type),
		start_time(act.start_time), end_time(act.end_time), activity_risk(act.activity_risk),
		place_risk(act.place_risk) {
}

Activity& Activity::operator=(const Activity& rhs) {
	if (&rhs != this) {
		this->tucase_id = rhs.tucase_id;
		this->place_type = rhs.place_type;
		this->start_time = rhs.start_time;
		this->end_time = rhs.end_time;
		this->activity_risk = rhs.activity_risk;
		this->place_risk = rhs.place_risk;
	}

	return *this;
}

Activity::~Activity() {
}

} /* namespace mrsa */
