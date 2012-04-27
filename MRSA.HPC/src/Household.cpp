/*
 * Household.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "Household.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

// high risk = 2f
Household::Household(std::vector<std::string>& vec) : AbstractPlace(vec, 2.0f) , persons() {
}

Household::~Household() {
}

void Household::addPerson(Person* person) {
	AbstractPlace::addPerson(person);
	persons.push_back(person);
}

void Household::runTransmission() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	for (PersonIter iter = persons.begin(); iter != persons.end(); ++iter) {
		processPerson(*iter, ta);
	}
}

void Household::reset() {
	AbstractPlace::reset();
	persons.clear();
}

} /* namespace mrsa */
