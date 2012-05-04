/*
 * SummaryInfectionStats.h
 *
 *  Created on: May 2, 2012
 *      Author: nick
 */

#ifndef SUMMARYINFECTIONSTATS_H_
#define SUMMARYINFECTIONSTATS_H_

#include <map>

#include "relogo/AgentSet.h"

#include "Person.h"

namespace mrsa {

/**
 * Gather summary stats about infection and colonization rates.
 *
 */
class SummaryInfectionStats {

	friend std::ostream& operator<<(std::ostream& os, const SummaryInfectionStats& stats);

public:
	SummaryInfectionStats();
	virtual ~SummaryInfectionStats();

	void gatherStats(repast::relogo::AgentSet<Person>& people);

	/**
	 * Gets the total number of infected persons.
	 */
	const long infectedCount() const {
		return infection_count;
	}

	/**
	 * Gets the total number of colonized persons.
	 */
	const long colonizedCount() const {
		return colonized_count;
	}

	/**
	 * Gets average time spent in the infection state.
	 */
	const double avgInfectedDuration() const {
		return infection_duration;
	}

	/**
	 * Gets average time spent in the colonized state.
	 */
	const double avgColonizedDuration() const {
		return colonization_duration;
	}

private:
	typedef std::map<unsigned int, unsigned long>::const_iterator ConstHistIter;
	typedef std::map<unsigned int, unsigned long>::iterator HistIter;

	void addToHistogram(unsigned int count, std::map<unsigned int, unsigned long>& hist);

	std::map<unsigned int, unsigned long> infection_hist;
	std::map<unsigned int, unsigned long> colonized_hist;

	long infection_count, colonized_count;
	double infection_duration, colonization_duration;
	double colonization_r0, infection_r0;
};

std::ostream& operator<<(std::ostream& os, const SummaryInfectionStats& stats);

} /* namespace mrsa */
#endif /* SUMMARYINFECTIONSTATS_H_ */
