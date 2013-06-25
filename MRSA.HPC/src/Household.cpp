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
 * Household.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */
#include <algorithm>

#include "Household.h"
#include "TransmissionAlgorithm.h"
#include "TransmissionEventRecorder.h"
#include "Parameters.h"

namespace mrsa {

// 14 x 24
const int FOURTEEN_DAYS = 336;
//const double CURE_PROB = 0;

Household::Household(std::vector<std::string>& vec, Risk risk) :
		AbstractPlace(vec, risk), members(), source_infectee(0), sd_timestamp(0) {
}

Household::~Household() {
}

void Household::addMember(Person* person) {
	members.push_back(person);
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

	if (source_infectee != 0
			&& repast::RepastProcess::instance()->getScheduleRunner().currentTick() - sd_timestamp
					>= FOURTEEN_DAYS) {
		treatHousehold();
	}

	//writeContactEvent(placeId());
}

void Household::treatHousehold() {
	double cure_probability = Parameters::instance()->getDoubleParameter("gamma");
	std::random_shuffle(members.begin(), members.end(), repast::uni_random);
	for (PersonIter iter = members.begin(); iter != members.end(); ++iter) {
		Person* p = *iter;
		// if draw fails then status remains as it was
		if (p->status() != UNCOLONIZED && p != source_infectee
				&& repast::Random::instance()->nextDouble() <= cure_probability) {
			DiseaseStatus status = p->status();
			p->updateStatus(UNCOLONIZED, NA);
			if (status == INFECTED) {
				TransmissionEventRecorder::instance()->recordEvent(
						repast::RepastProcess::instance()->getScheduleRunner().currentTick(), p,
						this, I_TO_U);
			} else {
				TransmissionEventRecorder::instance()->recordEvent(
						repast::RepastProcess::instance()->getScheduleRunner().currentTick(), p,
						this, C_TO_U);
			}
		}
	}
	sd_timestamp = 0;
	source_infectee = 0;
}

void Household::initHouseholdTreatment(Person* person) {
	if (source_infectee == 0) {
		source_infectee = person;
		sd_timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();
	}
}

} /* namespace mrsa */
