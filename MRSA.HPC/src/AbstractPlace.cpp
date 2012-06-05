/*
 * AbstractPlace.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "AbstractPlace.h"
#include "DiseaseStatus.h"
#include "Person.h"
#include "Statistics.h"

namespace mrsa {

AbstractPlace::AbstractPlace(std::vector<std::string>& vec, float risk) :
		Place(vec, risk), infected(), colonized(), uncolonized() {
}

AbstractPlace::AbstractPlace(std::string id, std::string type, float risk) : Place(id, type, risk) {}

AbstractPlace::~AbstractPlace() {
}

void AbstractPlace::addPerson(Person* person) {
	// increments the disease status counts
	// based on the person's status
	DiseaseStatus status = person->status();
	if (status == UNCOLONIZED) {
		uncolonized.push_back(person);
	} else if (status == COLONIZED) {
		colonized.push_back(person);
	} else {
		infected.push_back(person);
	}
}

// sets the status counts to 0
void AbstractPlace::reset() {
	infected.clear();
	colonized.clear();
	uncolonized.clear();
}

void AbstractPlace::processUncolonized(Person* person, TransmissionAlgorithm* ta) {
	person->updateStatus(ta->runUncolonized(risk_, infected.size(), colonized.size()));
	if (person->status() == COLONIZED) {
		// person has become colonized, so increment the
		// colonization count for places of this type
		Statistics::getInstance()->incrementColonizationCount(type_);
		if (infected.size() > 0) {
			// increment the pro-rated number of people colonized by the infectious
			// persons in this place.
			float colonizations_caused = 1.0f / infected.size();
			for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
				(*iter)->incrementColonizationsCaused(colonizations_caused);
			}
		} else if (colonized.size() > 0) {
			float colonizations_caused = 1.0f / colonized.size();
			for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
				(*iter)->incrementColonizationsCaused(colonizations_caused);
			}
		}
	}
}

void AbstractPlace::processInfected(Person* person, TransmissionAlgorithm* ta) {
	// updates the status of the specified person given the current
	// disease status counts in this place.
	if (person->canStatusChange()) {
		//float mod = person->seeksCare() ? 1.0f : 0.7f;
		person->updateStatus(ta->runInfected());
	}
}

void AbstractPlace::processColonized(Person* person, TransmissionAlgorithm* ta) {
	// updates the status of the specified person given the current
	// disease status counts in this place.
	person->updateStatus(ta->runColonized());
	if (SEEK_AND_DESTROY && person->status() == INFECTED && person->seeksCare()) {
		// seek and destroy
		person->initSeekAndDestroy();
	}
}

} /* namespace mrsa */
