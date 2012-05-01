/*
 * AbstractPlace.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "AbstractPlace.h"
#include "DiseaseStatus.h"
#include "Person.h"

namespace mrsa {

AbstractPlace::AbstractPlace(std::vector<std::string>& vec, float risk) :
		Place(vec, risk), colonized(0), uncolonized(0), infected(0) {
}

AbstractPlace::~AbstractPlace() {
}

void AbstractPlace::addPerson(Person* person) {
	// increments the disease status counts
	// based on the person's status
	DiseaseStatus status = person->status();
	if (status == COLONIZED) colonized++;
	else if (status == UNCOLONIZED) uncolonized++;
	else if (status == INFECTED) infected++;
}

// sets the status counts to 0
void AbstractPlace::reset() {
	colonized = uncolonized = infected = 0;
}

void AbstractPlace::processPerson(Person* person, TransmissionAlgorithm* ta) {
	// updates the status of the specified person given the current
	// disease status counts in this place.
	if (person->canStatusChange()) {
		DiseaseStatus status = person->status();
		person->updateStatus(ta->run(infected, colonized, uncolonized, status, risk_));
	}
}

} /* namespace mrsa */
