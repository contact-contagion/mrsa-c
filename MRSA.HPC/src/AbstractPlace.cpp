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
#include "Parameters.h"

namespace mrsa {

AbstractPlace::AbstractPlace(std::vector<std::string>& vec, Risk risk) :
		Place(vec, risk), activity_type_(0), infected(), colonized(), uncolonized() {
}

AbstractPlace::AbstractPlace(std::string id, std::string type, Risk risk) : Place(id, type, risk),
		 activity_type_(0), infected(), colonized(), uncolonized() {}

AbstractPlace::~AbstractPlace() {
}

void AbstractPlace::addPerson(Person* person, int activity_type) {
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
	if (activity_type > activity_type_) activity_type_ = activity_type;
}

// sets the status counts to 0
void AbstractPlace::reset() {
	activity_type_ = 0;
	infected.clear();
	colonized.clear();
	uncolonized.clear();
}

void AbstractPlace::processUncolonized(Person* person, TransmissionAlgorithm* ta) {
	float risk_multiplier = 1;
	if (activity_type_ == 0) risk_multiplier = risk_.a0_;
	else risk_multiplier = risk_.a1_;
	person->updateStatus(ta->runUncolonized(risk_multiplier, infected.size(), colonized.size()));
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
	if (person->canStatusChange()) {
		InfectionStatus status = person->infectionStatus();
		if (status == NONE)
			person->updateInfectionStatus(ta->runInfected());

		else if (status == SELF_CARE) {
			person->updateStatus(ta->runInfectedSelfCare());
		} else if (status == SEEK_CARE) {
			DiseaseStatus disease_status = ta->runInfectedSeekCare();
			person->updateStatus(disease_status);
			if (disease_status == UNCOLONIZED && Parameters::instance()->seekAndDestroyEnabled()) {
				person->initHouseholdTreatment();
			}
		}
	}
}

void AbstractPlace::processColonized(Person* person, TransmissionAlgorithm* ta) {
	float b_risk_multiplier = 1, e_risk_multiplier = 1;
	if (activity_type_ == 0) {
		b_risk_multiplier = risk_.b0_;
		e_risk_multiplier = risk_.x0_;
	}
	else {
		b_risk_multiplier = risk_.b1_;
		e_risk_multiplier = risk_.x1_;
	}
	// updates the status of the specified person given the current
	// disease status counts in this place.
	person->updateStatus(ta->runColonized(b_risk_multiplier, e_risk_multiplier));
}

} /* namespace mrsa */
