/*
 * AbstractPlace.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "AbstractPlace.h"
#include "PersonStatus.h"
#include "Person.h"

namespace mrsa {

AbstractPlace::AbstractPlace(std::vector<std::string>& vec, float risk) :
		Place(vec, risk), colonized(0), uncolonized(0), infected(0) {
}

AbstractPlace::~AbstractPlace() {
}

void AbstractPlace::addPerson(Person* person) {
	PersonStatus status = person->status();
	if (status == COLONIZED) colonized++;
	else if (status == UNCOLONIZED) uncolonized++;
	else if (status == INFECTED) infected++;
}

void AbstractPlace::reset() {
	colonized = uncolonized = infected = 0;
}

void AbstractPlace::processPerson(Person* person, TransmissionAlgorithm* ta) {
	PersonStatus status = person->status();
	person->updateStatus(ta->run(infected, colonized, uncolonized, status, risk_));
}

} /* namespace mrsa */
