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
#include "PersonStats.h"

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

	/**
	 * Gather the base stats from the persons. These base
	 * stats then used to calculate the summary stats.
	 */
	void gatherStats(repast::relogo::AgentSet<Person>& people);

	/**
	 * Writes out the stats to the specified file, including the
	 * summarys stats from PersonStats.
	 *
	 * @param file the path of the file to write the stats to
	 */
	void write(Statistics* p_stats, const std::string& file) const;

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

	// adds the count to the histogram of count sizes
	void addToHistogram(unsigned int count, std::map<unsigned int, unsigned long>& hist);

	std::map<unsigned int, unsigned long> infection_hist;
	std::map<unsigned int, unsigned long> colonized_hist;

	long infection_count, colonized_count;
	double infection_duration, colonization_duration;
	double colonization_from_colonization_r0, colonization_from_infection_r0;
	double infection_from_colonization_r0, infection_from_infection_r0;
};

std::ostream& operator<<(std::ostream& os, const SummaryInfectionStats& stats);

} /* namespace mrsa */
#endif /* SUMMARYINFECTIONSTATS_H_ */
