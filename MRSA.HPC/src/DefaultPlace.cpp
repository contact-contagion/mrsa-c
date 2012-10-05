/*
 * DefaultPlace.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "DefaultPlace.h"

namespace mrsa {

// low risk = 1f, 4 uncolonized persons each time
DefaultPlace::DefaultPlace(std::vector<std::string>& vec, Risk risk) :	SelectiveTransmissionPlace(vec, risk, 4) {
}

DefaultPlace::~DefaultPlace() {
}


} /* namespace mrsa */
