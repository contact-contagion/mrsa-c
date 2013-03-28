/*
 * AbstractStayManager.cpp
 *
 *  Created on: Mar 7, 2013
 *      Author: nick
 */

#include <iostream>
#include "AbstractStayManager.h"

namespace mrsa {

AbstractStayManager::AbstractStayManager(size_t years) :
		stay_data(years, std::make_pair(0u, 0u)), duration(0), start_day(0) {
}

AbstractStayManager::~AbstractStayManager() {
}

int count = 0;
bool AbstractStayManager::inPlace(int year, int day) {
	bool ret_val = false;

	if (duration > 0) {
		// currently in place. check if should leave.
		ret_val = (day - start_day) < duration;
		if (!ret_val) {
			duration = 0;
			start_day = 0;
		}
	} else {
		// year starts with 1
		StayData& data = stay_data.at(year - 1);
		ret_val = data.second > 0 && data.first == (unsigned int) day;
		if (ret_val) {
			start_day = day;
			duration = data.second;
		}
	}

	return ret_val;
}

} /* namespace mrsa */
