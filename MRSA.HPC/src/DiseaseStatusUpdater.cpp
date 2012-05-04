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
		min_infection_duration_(min_infection_duration), timestamp(0), infected_time(0),
		colonized_time(0), infected_count(0), colonized_count(0), status_(UNCOLONIZED) {
}

void DiseaseStatusUpdater::updateStatus(DiseaseStatus status) {
	if (status_ == UNCOLONIZED && status == COLONIZED) {
		// set timestamp
		timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();

	} else if (status_ == COLONIZED && status != COLONIZED) {
		// move from colonized to something else
		// increment colonized counter
		++colonized_count;
		// calc time spent colonized and add to colonized time
		colonized_time += repast::RepastProcess::instance()->getScheduleRunner().currentTick() - timestamp;
		timestamp = 0;

		if (status == INFECTED) {
			// moved from colonized to infected
			// set time stamp.
			timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();
		}

	} else if (status_ == INFECTED && status != INFECTED) {
		// moved from infected to something else.
		// increment infected count
		++infected_count;
		// calc time spent infected and add to infected time
		infected_time += repast::RepastProcess::instance()->getScheduleRunner().currentTick() - timestamp;
		timestamp = 0;

		if (status == COLONIZED) {
			// moved from infected to colonized
			// set timestamp
			timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();
		}
	}

	status_ = status;
}

bool DiseaseStatusUpdater::canStatusChange() {
	// if the current status is infected, then can only change if the minimum
	// disease duration has been reached.
	if (status_ == INFECTED)
		return repast::RepastProcess::instance()->getScheduleRunner().currentTick()
				- timestamp > min_infection_duration_;

	// otherwise, can always change.
	return true;
}

DiseaseStatusUpdater::~DiseaseStatusUpdater() {
}

} /* namespace mrsa */
