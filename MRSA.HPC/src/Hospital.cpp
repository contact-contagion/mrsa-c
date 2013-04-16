/*
 * Hospital.cpp
 *
 *  Created on: Apr 26, 2012
 *      Author: nick
 */

#include "Hospital.h"
#include "TransmissionAlgorithm.h"
#include "Statistics.h"
#include "TransmissionEventRecorder.h"

namespace mrsa {

// 4 uncolonized persons each time
Hospital::Hospital(std::vector<std::string>& vec, Risk risk) : AbstractPlace(vec, risk) {
}

Hospital::~Hospital() {
}

void Hospital::runTransmission() {
	// run the TA for each person.
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	// uncolonized must always go first so that the infected and colonized persons
	// if they cause a colonization of the uncolonized are still in that infected or
	// colonized state.
	for (PersonIter iter = uncolonized.begin(); iter != uncolonized.end(); ++iter) {
		Hospital::processUncolonized(*iter, ta);
	}

	for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
		processInfected(*iter, ta);
	}

	for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
		processColonized(*iter, ta);
	}
}

void Hospital::processUncolonized(Person* person, TransmissionAlgorithm* ta) {
	float risk_multiplier = 1;
	if (activity_type_ == 0) risk_multiplier = risk_.a0_;
	else risk_multiplier = risk_.a1_;

	DiseaseStatus status = ta->runUncolonized(risk_multiplier, 1, 0);
	if (status == COLONIZED) {
		// person has become colonized, so increment the
		// colonization count for places of this type
		person->updateStatus(status, UC_TO_C_FROM_I);
		Statistics::getInstance()->incrementColonizationCount(type_, person->zipCode());
		TransmissionEventRecorder::instance()->recordEvent(repast::RepastProcess::instance()->getScheduleRunner().currentTick(), person,
								this, U_TO_C);
		//Statistics::getInstance()->incrementColonizationFromInfection();
	} else {
		person->updateStatus(status, NA);
	}
}


} /* namespace mrsa */
