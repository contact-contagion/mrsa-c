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
	// the number of other colonized persons attributable
	// to this infection / colonization
	float colonized_persons;
	// the duration of the infection / colonization
	float duration;
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
	void updateStatus(DiseaseStatus status);

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

private:
	float min_infection_duration_;
	// records the time at which a status change occured, the
	// total time spent infected, and the total time spent colonized.
	unsigned int timestamp;
	// records how many times this updater has been infected, or colonized.
	unsigned int infected_count, colonized_count;

	DiseaseStatus status_;

	// the current years infections / colonization stats
	std::list<StatusStats> yearly_status_stats;
	// all preceeding years infections / colonization stats
	std::list<StatusStats> total_status_stats;

};

} /* namespace mrsa */
#endif /* DISEASESTATUSUPDATER_H_ */
