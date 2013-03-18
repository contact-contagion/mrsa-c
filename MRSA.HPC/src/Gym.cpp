/*
 * Gym.cpp
 *
 *  Created on: Mar 11, 2013
 *      Author: nick
 */

#include "Gym.h"

namespace mrsa {

const unsigned int MAX_GQ_COMPONENT_SIZE = 30;

Gym::Gym(std::vector<std::string>& vec, Risk risk) : CompositePlace(vec, risk, MAX_GQ_COMPONENT_SIZE, GYM) {
}

Gym::~Gym() {}

void Gym::addPerson(Person* person, int activity_type) {
	person->goToCompositePlace(this, activity_type);
}


} /* namespace mrsa */
