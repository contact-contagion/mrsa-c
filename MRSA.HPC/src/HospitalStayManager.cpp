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
		AbstractStayManager(10) {

	unsigned int length = y1_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
			365 - y1_length).next() : y1_length;
	stay_data[0] = (std::pair<unsigned int, unsigned int>(length, y1_length));

	length = y2_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y2_length).next() : y2_length;
	stay_data[1] = (std::pair<unsigned int, unsigned int>(length, y2_length));

	length = y3_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y3_length).next() : y3_length;
	stay_data[2] = (std::pair<unsigned int, unsigned int>(length, y3_length));

	length = y4_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y4_length).next() : y4_length;
	stay_data[3] =(std::pair<unsigned int, unsigned int>(length, y4_length));

	length = y5_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y5_length).next() : y5_length;
	stay_data[4] =(std::pair<unsigned int, unsigned int>(length, y5_length));
}

HospitalStayManager::~HospitalStayManager() {
}


} /* namespace mrsa */
