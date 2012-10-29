/*
 * HospitalStayManager.cpp
 *
 *  Created on: Oct 24, 2012
 *      Author: nick
 */

#include <utility>
#include <repast_hpc/Random.h>

#include "HospitalStayManager.h"

namespace mrsa {

HospitalStayManager::HospitalStayManager(unsigned int y1_length, unsigned int y2_length,
		unsigned int y3_length, unsigned int y4_length, unsigned int y5_length) :
		IHospitalStayManager(), stays(10, std::make_pair(0u, 0u)), duration(0), start_day(0) {

	stays[0] =
			(std::pair<unsigned int, unsigned int>(
					(unsigned int) repast::Random::instance()->createUniIntGenerator(1,
							365 - y1_length).next(), y1_length));
	stays[1] =
			(std::pair<unsigned int, unsigned int>(
					(unsigned int) repast::Random::instance()->createUniIntGenerator(1,
							365 - y2_length).next(), y2_length));
	stays[2] =
			(std::pair<unsigned int, unsigned int>(
					(unsigned int) repast::Random::instance()->createUniIntGenerator(1,
							365 - y3_length).next(), y3_length));
	stays[3] =
			(std::pair<unsigned int, unsigned int>(
					(unsigned int) repast::Random::instance()->createUniIntGenerator(1,
							365 - y4_length).next(), y4_length));
	stays[4] =
			(std::pair<unsigned int, unsigned int>(
					(unsigned int) repast::Random::instance()->createUniIntGenerator(1,
							365 - y5_length).next(), y5_length));
}

HospitalStayManager::~HospitalStayManager() {
}

bool HospitalStayManager::inHospital(int year, int day) {
	bool ret_val = false;
	if (duration > 0) {
		// currently in hospital. check if should leave.
		bool ret_val = day - start_day < duration;
		if (!ret_val) {
			duration = 0;
			start_day = 0;
		}
	} else {
		// year starts with 1
		StayData& data = stays.at(year - 1);
		ret_val = data.second > 0 && data.first == (unsigned int) day;
		if (ret_val) {
			start_day = day;
			duration = data.second;
		}
	}

	return ret_val;

}

} /* namespace mrsa */
