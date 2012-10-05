/*
 * Workplace.cpp
 *
 *  Created on: Apr 26, 2012
 *      Author: nick
 */

#include "Workplace.h"

namespace mrsa {

// almost no-risk place = 0.1f, 4 uncolonized persons each time
Workplace::Workplace(std::vector<std::string>& vec, Risk risk) :
		SelectiveTransmissionPlace(vec, risk, 4) {
}

Workplace::~Workplace() {
}


} /* namespace mrsa */
