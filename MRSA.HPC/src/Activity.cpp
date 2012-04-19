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
	start_time = strToInt(val) / 60;

	val = vec[STOP_TIME_IDX];
	val = trim(val);
	// minutes to hours
	end_time = strToInt(val) / 60;

	val = vec[ACTIVITY_RISK_IDX];
	val = trim(val);
	activity_risk = strToInt(val);

	val = vec[PLACE_RISK_IDX];
	val = trim(val);
	place_risk = (float)strToDouble(val);
	if (place_risk == 0) place_risk = .1f;
}

Activity::~Activity() {
}

} /* namespace mrsa */
