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
		AbstractStayManager(11) {

	unsigned int start = y1_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
			365 - y1_length).next() : 1;
	if (y1_length > 364) y1_length = 365;
	stay_data[0] = (std::pair<unsigned int, unsigned int>(start, y1_length));

	if (y2_length > 364) y2_length = 365;
	start = y2_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y2_length).next() : 1;
	stay_data[1] = (std::pair<unsigned int, unsigned int>(start, y2_length));

	start = y3_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y3_length).next() : 1;
	if (y3_length > 364) y3_length = 365;
	stay_data[2] = (std::pair<unsigned int, unsigned int>(start, y3_length));

	start = y4_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y4_length).next() : 1;
	if (y4_length > 364) y4_length = 365;
	stay_data[3] =(std::pair<unsigned int, unsigned int>(start, y4_length));

	start = y5_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y5_length).next() : 1;
	if (y5_length > 364) y5_length = 365;
	stay_data[4] =(std::pair<unsigned int, unsigned int>(start, y5_length));

	// repeat 5 - 9
	stay_data[5] = stay_data[0];
	stay_data[6] = stay_data[1];
	stay_data[7] = stay_data[2];
	stay_data[8] = stay_data[3];
	stay_data[9] = stay_data[4];
}

HospitalStayManager::~HospitalStayManager() {
}


} /* namespace mrsa */
