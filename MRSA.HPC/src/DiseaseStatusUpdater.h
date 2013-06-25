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
 * DiseaseStatusUpdater.h
 *
 *  Created on: May 1, 2012
 *      Author: nick
 */

#ifndef DISEASESTATUSUPDATER_H_
#define DISEASESTATUSUPDATER_H_

#include <list>

#include "DiseaseStatus.h"

namespace mrsa {

// tracks info about an individual's
// colonization or infection
struct StatusStats {
	// the disease status
	DiseaseStatus status;
	InfectionStatus infection_status;
	// the number of other colonized persons attributable
	// to this infection / colonization
	float colonized_persons;
	// the duration of the infection / colonization
	float duration;

	ColonizationCause col_cause;
};

/**
 * Manages the updates from one disease state to another
 * and captures some stats about disease state duration.
 *
 */
class DiseaseStatusUpdater {

	friend class Statistics;

public:
	DiseaseStatusUpdater(float min_infection_duration);
	virtual ~DiseaseStatusUpdater();

	/**
	 * Update the disease status to new status.
	 */
	void updateStatus(DiseaseStatus status, ColonizationCause cause);

	/**
	 * Updates the infection status.
	 */
	void updateInfectionStatus(InfectionStatus status);

	void setMinInfectionDuration(float min_infection_duration) {
		min_infection_duration_ = min_infection_duration;
	}

	/**
	 * Increment the number colonization caused by this person by the specified
	 * amount.
	 *
	 * @param colonizations_caused the amount to increment by
	 */
	void incrementColonizationsCaused(float colonization_caused);

	/**
	 * Gets whether or not this Person's disease status can change. For example,
	 * if a Person is infected there is a minimum amount of time that has to pass
	 * with the Person in the infected state before he or she can become
	 * uninfected.
	 */
	bool canStatusChange();


	/**
	 * Resets any yearly counts, perparing this DiseaseStatusUpdater
	 * for capturing new yearly data.
	 */
	void resetYearlyCounts();

	/**
	 * Gets the current status.
	 */
	DiseaseStatus status() const {
		return status_;
	}

	/**
	 * Gets the current infection status.
	 */
	InfectionStatus infectionStatus() const {
		return infection_status_;
	}


private:
	float min_infection_duration_; // default_infection_duration_;
	// records the time at which a status change occured, the
	// total time spent infected, and the total time spent colonized.
	unsigned int timestamp;
	// records how many times this updater has been infected, or colonized.
	unsigned int infected_count, colonized_count;

	DiseaseStatus status_;
	InfectionStatus infection_status_;

	// the current years infections / colonization stats
	std::list<StatusStats> yearly_status_stats;
	// all preceeding years infections / colonization stats
	std::list<StatusStats> total_status_stats;

};

} /* namespace mrsa */
#endif /* DISEASESTATUSUPDATER_H_ */
