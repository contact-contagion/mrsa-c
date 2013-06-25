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
	//writeContactEvent(placeId());
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
