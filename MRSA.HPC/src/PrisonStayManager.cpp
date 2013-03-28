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
	stay_data[year] =
			(std::pair<unsigned int, unsigned int>(
					(unsigned int) repast::Random::instance()->createUniIntGenerator(1,
							365 - duration).next(), duration));
}

} /* namespace mrsa */
