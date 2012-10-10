/*
 * Hospital.cpp
 *
 *  Created on: Apr 26, 2012
 *      Author: nick
 */

#include "Hospital.h"

namespace mrsa {

// 4 uncolonized persons each time
Hospital::Hospital(std::vector<std::string>& vec, Risk risk) :
		SelectiveTransmissionPlace(vec, risk, 4) {
}

Hospital::~Hospital() {
}


} /* namespace mrsa */
