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
Household::Household(std::vector<std::string>& vec) :
		AbstractPlace(vec, 2.0f) {
}

Household::~Household() {
}

void Household::runTransmission() {
	// run the TA for each person.
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	// uncolonized must always go first so that the infected and colonized persons
	// if they cause a colonization of the uncolonized are still in that infected or
	// colonized state.
	for (PersonIter iter = uncolonized.begin(); iter != uncolonized.end(); ++iter) {
		processUncolonized(*iter, ta);
	}

	for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
		processInfected(*iter, ta);
	}

	for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
		processColonized(*iter, ta);
	}
}

} /* namespace mrsa */
