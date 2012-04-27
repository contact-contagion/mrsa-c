/*
 * DefaultPlace.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "DefaultPlace.h"

namespace mrsa {

// low risk = 1f, 4 uncolonized persons each time
DefaultPlace::DefaultPlace(std::vector<std::string>& vec) :	SelectiveTransmissionPlace(vec, 1.0f, 4) {
}

DefaultPlace::~DefaultPlace() {
}


} /* namespace mrsa */
