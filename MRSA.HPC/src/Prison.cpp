/*
 * GeneralQuarters.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#include "Prison.h"

namespace mrsa {

const int MAX_PRISON_COMPONENT_SIZE = 30;

Prison::Prison(std::vector<std::string>& vec, Risk risk) : CompositePlace(vec, risk, MAX_PRISON_COMPONENT_SIZE, PRISON) {
}

Prison::~Prison() {
}

void Prison::addPerson(Person* person, int activity_type) {
	person->goToCompositePlace(this, activity_type);
}

} /* namespace mrsa */
