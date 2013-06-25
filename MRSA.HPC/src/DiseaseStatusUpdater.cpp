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
# MRSA Model
# 
# Copyright (c) 2012 University of Chicago and Argonne National Laboratory
#    All rights reserved.
#   
#    Redistribution and use in source and binary forms, with 
#    or without modification, are permitted provided that the following 
#    conditions are met:
#   
#   	 Redistributions of source code must retain the above copyright notice,
#   	 this list of conditions and the following disclaimer.
#   
#   	 Redistributions in binary form must reproduce the above copyright notice,
#   	 this list of conditions and the following disclaimer in the documentation
#   	 and/or other materials provided with the distribution.
#   
#   	 Neither the name of the Argonne National Laboratory nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#   
#    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#    ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
#    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
#    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*
 * DiseaseStatusUpdater.cpp
 *
 *  Created on: May 1, 2012
 *      Author: nick
 */

#include "repast_hpc/RepastProcess.h"

#include "DiseaseStatusUpdater.h"
#include "Parameters.h"
#include "Constants.h"

namespace mrsa {

DiseaseStatusUpdater::DiseaseStatusUpdater(float min_infection_duration) :
		min_infection_duration_(min_infection_duration), timestamp(0),infected_count(0),
		colonized_count(0), status_(UNCOLONIZED), infection_status_(NONE), yearly_status_stats() {
}

// increments the number of colonizations caused by this disease
// state
void DiseaseStatusUpdater::incrementColonizationsCaused(float colonization_caused) {
	// get the current StatusStats and increment its colonized_persons count
	StatusStats& stats = yearly_status_stats.back();
	stats.colonized_persons += colonization_caused;
}

void DiseaseStatusUpdater::updateInfectionStatus(InfectionStatus status) {
	infection_status_ = status;
}

void DiseaseStatusUpdater::updateStatus(DiseaseStatus status, ColonizationCause cause) {
	// these cover all the transitions except for INFECTED to INFECTED on
	// which we dont' do anything.
	if (status_ == UNCOLONIZED && (status == COLONIZED || status == INFECTED)) {
		// set timestamp
		timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();
		// hack to allow sim to start on the 1/2 year
		if (timestamp == 0 && cause == FROM_INIT) {
			timestamp = YEAR_START;
		}

		StatusStats stats = { status, NONE, 0.0f, 0.0f, cause};

		yearly_status_stats.push_back(stats);
		infection_status_ = NONE;

	} else if (status_ == COLONIZED && status != COLONIZED) {
		// move from colonized to something else
		// increment colonized counter
		++colonized_count;

		// calc time spent colonized and add to colonized time
		unsigned int duration = repast::RepastProcess::instance()->getScheduleRunner().currentTick()
				- timestamp;
		StatusStats& stats = yearly_status_stats.back();
		stats.duration = duration;
		timestamp = 0;
		infection_status_ = NONE;

		if (status == INFECTED) {
			// moved from colonized to infected
			// set time stamp.
			timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();

			StatusStats stats = { status, NONE, 0.0f, 0.0f, C_TO_I_FROM_NA};
			yearly_status_stats.push_back(stats);
		}

	} else if (status_ == INFECTED && status != INFECTED) {
		// moved from infected to something else.
		// increment infected count
		++infected_count;
		// calc time spent infected and add to infected time
		unsigned int duration = repast::RepastProcess::instance()->getScheduleRunner().currentTick()
				- timestamp;
		StatusStats& stats = yearly_status_stats.back();
		stats.duration = duration;
		stats.infection_status = infection_status_;

		timestamp = 0;
		// no longer colonized so the min duration should go back to the default.
		min_infection_duration_ = (float)Parameters::instance()->getDoubleParameter(MIN_INFECT_PERIOD);

		if (status == COLONIZED) {
			// moved from infected to colonized
			// set timestamp
			timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();

			StatusStats stats = { status, NONE, 0.0f, 0.0f, I_TO_C_FROM_NA};
			yearly_status_stats.push_back(stats);
		}
		infection_status_ = NONE;
	}

	status_ = status;
}

void DiseaseStatusUpdater::resetYearlyCounts() {
	std::list<StatusStats>::iterator iter;
	for (iter = yearly_status_stats.begin(); iter != yearly_status_stats.end(); ++iter) {
		if (iter->duration == 0) {
			break;
		}
	}

	total_status_stats.insert(total_status_stats.end(), yearly_status_stats.begin(), iter);
	yearly_status_stats.erase(yearly_status_stats.begin(), iter);
}

bool DiseaseStatusUpdater::canStatusChange() {
	// if the current status is infected, then can only change if the minimum
	// disease duration has been reached.
	if (status_ == INFECTED) {
		return repast::RepastProcess::instance()->getScheduleRunner().currentTick() - timestamp
				> min_infection_duration_;
	}

	// otherwise, can always change.
	return true;
}

DiseaseStatusUpdater::~DiseaseStatusUpdater() {
}

} /* namespace mrsa */
