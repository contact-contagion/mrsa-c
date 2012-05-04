/*
 * DiseaseStatusUpdater.h
 *
 *  Created on: May 1, 2012
 *      Author: nick
 */

#ifndef DISEASESTATUSUPDATER_H_
#define DISEASESTATUSUPDATER_H_

#include "DiseaseStatus.h"

namespace mrsa {

/**
 * Manages the updates from one disease state to another
 * and captures some stats about disease state duration.
 *
 */
class DiseaseStatusUpdater {
public:
	DiseaseStatusUpdater(float min_infection_duration);
	virtual ~DiseaseStatusUpdater();

	/**
	 * Update the disease status to new status.
	 */
	void updateStatus(DiseaseStatus status);

	/**
	 * Gets whether or not this Person's disease status can change. For example,
	 * if a Person is infected there is a minimum amount of time that has to pass
	 * with the Person in the infected state before he or she can become
	 * uninfected.
	 */
	bool canStatusChange();

	/**
	 * Gets the current status.
	 */
	DiseaseStatus status() const {
		return status_;
	}

private:
	float min_infection_duration_;
	// records the time at which a status change occured, the
	// total time spent infected, and the total time spent colonized.
	unsigned int timestamp, infected_time, colonized_time;
	// records how many times this updater has been infected, or colonized.
	unsigned int infected_count, colonized_count;

	DiseaseStatus status_;
};

} /* namespace mrsa */
#endif /* DISEASESTATUSUPDATER_H_ */
