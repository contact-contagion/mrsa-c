/*
 * GeneralQuarters.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#include "GeneralQuarters.h"

namespace mrsa {

const unsigned int MAX_GQ_COMPONENT_SIZE = 30;

GeneralQuarters::GeneralQuarters(std::vector<std::string>& vec, Risk risk) : CompositePlace(vec, risk, MAX_GQ_COMPONENT_SIZE, GQ) {
}

GeneralQuarters::~GeneralQuarters() {
}

void GeneralQuarters::addPerson(Person* person, int activity_type) {
	person->goToCompositePlace(this, activity_type);
}

} /* namespace mrsa */
