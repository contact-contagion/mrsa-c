/*
 * Household.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "Household.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

// sets the risk 2.0
Household::Household(std::vector<std::string>& vec) : AbstractPlace(vec, 2.0f) , persons() {
}

Household::~Household() {
}

void Household::addPerson(Person* person) {
	// update the disease status counters via AbstractPlace
	AbstractPlace::addPerson(person);
	persons.push_back(person);
}

void Household::runTransmission() {
	// run the TA for each person.
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	for (PersonIter iter = persons.begin(); iter != persons.end(); ++iter) {
		processPerson(*iter, ta);
	}
}

void Household::reset() {
	// resets the disease counters to 0
	AbstractPlace::reset();
	// clears the persons vector
	persons.clear();
}

} /* namespace mrsa */
