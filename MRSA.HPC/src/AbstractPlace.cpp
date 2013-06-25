/*
*MRSA Model
*
*Copyright (c) 2013 University of Chicago and Argonne National Laboratory
*   All rights reserved.
*  
*   Redistribution and use in source and binary forms, with 
*   or without modification, are permitted provided that the following 
*   conditions are met:
*  
*  	 Redistributions of source code must retain the above copyright notice,
*  	 this list of conditions and the following disclaimer.
*  
*  	 Redistributions in binary form must reproduce the above copyright notice,
*  	 this list of conditions and the following disclaimer in the documentation
*  	 and/or other materials provided with the distribution.
*  
*  	 Neither the name of the Argonne National Laboratory nor the names of its
*     contributors may be used to endorse or promote products derived from
*     this software without specific prior written permission.
*  
*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
*   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
*   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
*   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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
#include "Constants.h"
#include "TransmissionEventRecorder.h"
#include "FileOutput.h"

namespace mrsa {

AbstractPlace::AbstractPlace(std::vector<std::string>& vec, Risk risk) :
		Place(vec, risk), activity_type_(0), infected(), colonized(), uncolonized() {
}

AbstractPlace::AbstractPlace(std::string id, std::string type, Risk risk, unsigned int zip) :
		Place(id, type, risk, zip), activity_type_(0), infected(), colonized(), uncolonized() {
}

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
	if (activity_type > activity_type_)
		activity_type_ = activity_type;
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
	if (activity_type_ == 0)
		risk_multiplier = risk_.a0_;
	else
		risk_multiplier = risk_.a1_;
	DiseaseStatus status = ta->runUncolonized(risk_multiplier, infected.size(), colonized.size());
	if (status == COLONIZED) {

		if (infected.size() > 0) {
			person->updateStatus(status, UC_TO_C_FROM_I);
			// increment the pro-rated number of people colonized by the infectious
			// persons in this place.
			float colonizations_caused = 1.0f / infected.size();
			for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
				(*iter)->incrementColonizationsCaused(colonizations_caused);
			}
		} else if (colonized.size() > 0) {
			person->updateStatus(status, UC_TO_C_FROM_C);
			float colonizations_caused = 1.0f / colonized.size();
			for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
				(*iter)->incrementColonizationsCaused(colonizations_caused);
			}
		}

		// person has become colonized, so increment the
		// colonization count for places of this type
		std::string place_type = type_;
		if (type_ == HOUSEHOLD_TYPE && (person->household() != this)) {
			place_type = OTHER_HOUSEHOLD_TYPE;
		}
		Statistics::getInstance()->incrementColonizationCount(place_type, person->zipCode());
		TransmissionEventRecorder::instance()->recordEvent(
				repast::RepastProcess::instance()->getScheduleRunner().currentTick(), person, this,
				U_TO_C);
	}
}

void AbstractPlace::processInfected(Person* person, TransmissionAlgorithm* ta) {
	if (person->canStatusChange()) {
		InfectionStatus status = person->infectionStatus();
		if (status == NONE) {
			person->updateInfectionStatus(person->seekCare() ? SEEK_CARE : SELF_CARE);

		} else if (status == SELF_CARE) {
			person->updateStatus(ta->runInfectedSelfCare(), NA);
		} else if (status == SEEK_CARE) {
			DiseaseStatus disease_status = ta->runInfectedSeekCare();
			person->updateStatus(disease_status, NA);
			if (disease_status == UNCOLONIZED && Parameters::instance()->seekAndDestroyEnabled()) {
				person->initHouseholdTreatment();
			}
		}

		if (person->status() == UNCOLONIZED) {
			TransmissionEventRecorder::instance()->recordEvent(
					repast::RepastProcess::instance()->getScheduleRunner().currentTick(), person,
					this, I_TO_U);
		} else if (person->status() == COLONIZED) {
			TransmissionEventRecorder::instance()->recordEvent(
					repast::RepastProcess::instance()->getScheduleRunner().currentTick(), person,
					this, I_TO_C);
		}

	}
}

void AbstractPlace::processColonized(Person* person, TransmissionAlgorithm* ta) {
	float b_risk_multiplier = 1, e_risk_multiplier = 1;
	if (activity_type_ == 0) {
		b_risk_multiplier = risk_.b0_;
		e_risk_multiplier = risk_.x0_;
	} else {
		b_risk_multiplier = risk_.b1_;
		e_risk_multiplier = risk_.x1_;
	}
	// updates the status of the specified person given the current
	// disease status counts in this place.
	DiseaseStatus status = ta->runColonized(b_risk_multiplier, e_risk_multiplier);
	person->updateStatus(status, NA);

	if (status == INFECTED) {
		Statistics::getInstance()->incrementInfectionCount(type_, person->zipCode());
		TransmissionEventRecorder::instance()->recordEvent(
				repast::RepastProcess::instance()->getScheduleRunner().currentTick(), person, this,
				C_TO_I);
	} else if (status == UNCOLONIZED) {
		TransmissionEventRecorder::instance()->recordEvent(
				repast::RepastProcess::instance()->getScheduleRunner().currentTick(), person, this,
				C_TO_U);
	}
}

void AbstractPlace::writeContactEvent(const std::string& place_id) {
	if (infected.size() + colonized.size() + uncolonized.size() > 0 /*&& (placeType() == HOSPITAL_TYPE || placeType() == PRISON_TYPE) */) {
		FileOutput& out = *FileOutput::instance();
		out << repast::RepastProcess::instance()->getScheduleRunner().currentTick() << ","
				<< place_id << "," << placeType();

		for (PersonIter pIter = uncolonized.begin(); pIter != uncolonized.end(); ++pIter) {
			out << "," << (*pIter)->personId();
		}
		for (PersonIter pIter = infected.begin(); pIter != infected.end(); ++pIter) {
			out << "," << (*pIter)->personId();
		}

		for (PersonIter pIter = colonized.begin(); pIter != colonized.end(); ++pIter) {
			out << "," << (*pIter)->personId();
		}

		out << "\n";
	}
}

} /* namespace mrsa */
