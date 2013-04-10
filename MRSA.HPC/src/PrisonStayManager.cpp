/*
 * PrisonStayManager.cpp
 *
 *  Created on: Mar 7, 2013
 *      Author: nick
 */

#include "repast_hpc/Random.h"
#include "PrisonStayManager.h"

namespace mrsa {

PrisonStayManager::PrisonStayManager() :
		AbstractStayManager(10) {
}

PrisonStayManager::~PrisonStayManager() {
}

void PrisonStayManager::createStayFor(int year, unsigned int duration) {
	unsigned int start = 1;
	if (duration < 364) {
		start =
				(unsigned int) repast::Random::instance()->createUniIntGenerator(1, 365 - duration).next();
	} else {
		start = 1;
		duration = 365;
	}

	stay_data[year] = std::pair<unsigned int, unsigned int>(start, duration);
}

} /* namespace mrsa */
