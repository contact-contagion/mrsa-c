/*
 * DiseaseStatusUpdater.cpp
 *
 *  Created on: May 1, 2012
 *      Author: nick
 */

#include "repast_hpc/RepastProcess.h"

#include "DiseaseStatusUpdater.h"
#include "Parameters.h"

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

			StatusStats stats = { status, NONE, 0.0f, 0.0f, NA};
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

			StatusStats stats = { status, NONE, 0.0f, 0.0f, NA};
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
