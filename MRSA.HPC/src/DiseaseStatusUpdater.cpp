/*
 * DiseaseStatusUpdater.cpp
 *
 *  Created on: May 1, 2012
 *      Author: nick
 */

#include "repast_hpc/RepastProcess.h"

#include "DiseaseStatusUpdater.h"

namespace mrsa {

DiseaseStatusUpdater::DiseaseStatusUpdater(float min_infection_duration) :
		min_infection_duration_(min_infection_duration), timestamp(0),infected_count(0),
		colonized_count(0), status_(UNCOLONIZED), yearly_status_stats() {
}

void DiseaseStatusUpdater::incrementColonizationsCaused(float colonization_caused) {
	// get the current StatusStats and increment its colonized_persons count
	StatusStats& stats = yearly_status_stats.back();
	stats.colonized_persons += colonization_caused;
}

void DiseaseStatusUpdater::updateStatus(DiseaseStatus status) {
	if (status_ == UNCOLONIZED && (status == COLONIZED || status == INFECTED)) {
		// set timestamp
		timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();

		StatusStats stats = { status, 0.0f, 0.0f };
		yearly_status_stats.push_back(stats);

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

		if (status == INFECTED) {
			// moved from colonized to infected
			// set time stamp.
			timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();

			StatusStats stats = { status, 0.0f, 0.0f };
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

		timestamp = 0;

		if (status == COLONIZED) {
			// moved from infected to colonized
			// set timestamp
			timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();

			StatusStats stats = { status, 0.0f, 0.0f };
			yearly_status_stats.push_back(stats);
		}
	}

	status_ = status;
}

void DiseaseStatusUpdater::clearYearlyCounts() {
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
