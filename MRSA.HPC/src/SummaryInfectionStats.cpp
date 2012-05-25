/*
 * SummaryInfectionStats.cpp
 *
 *  Created on: May 2, 2012
 *      Author: nick
 */

#include "SummaryInfectionStats.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

using namespace repast::relogo;

SummaryInfectionStats::SummaryInfectionStats() : infection_hist(), colonized_hist(),
		infection_count(0), colonized_count(0), infection_duration(0), colonization_duration(0),
		colonization_from_colonization_r0(0), colonization_from_infection_r0(0) {

}

SummaryInfectionStats::~SummaryInfectionStats() {
}

void SummaryInfectionStats::gatherStats(AgentSet<Person>& people) {

	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
		Person* p = (*iter);
		// setting to UNCOLONIZED will update any "open" stats
		// given that states are only counted when transitioning out
		// of them.
		p->updateStatus(UNCOLONIZED);
		unsigned int i_count =  p->status_.infected_count;
		addToHistogram(i_count, infection_hist);
		infection_count += i_count;
		unsigned int c_count = p->status_.colonized_count;
		addToHistogram(c_count, colonized_hist);
		colonized_count += c_count;
		infection_duration += p->status_.infected_time;
		colonization_duration += p->status_.colonized_time;
	}

	if (infection_count != 0) infection_duration = infection_duration / infection_count;
	if (colonized_count != 0) colonization_duration = colonization_duration / colonized_count;

}

void SummaryInfectionStats::addToHistogram(unsigned int count, std::map<unsigned int, unsigned long>& hist) {
	HistIter iter = hist.find(count);
	if (iter == hist.end()) {
		hist.insert(std::pair<unsigned int, unsigned long>(count, 1));
	} else {
		++(iter->second);
	}
}

std::ostream& operator<<(std::ostream& os, const SummaryInfectionStats& stats) {
	os << "SummaryInfectionStats[" << std::endl <<
			"\ttotal infections: " << stats.infection_count << std::endl <<
			"\ttotal colonization: " << stats.colonized_count << std::endl <<
			"\tavg. infection duration: " << stats.infection_duration << std::endl <<
			"\tavg. colonization duration: " << stats.colonization_duration << std::endl;


	for (SummaryInfectionStats::ConstHistIter iter = stats.infection_hist.begin(); iter != stats.infection_hist.end(); ++iter) {
		os << "\t" << iter->second << " persons infected " << iter->first << " times" << std::endl;
	}

	for (SummaryInfectionStats::ConstHistIter iter = stats.colonized_hist.begin(); iter != stats.colonized_hist.end(); ++iter) {
			os << "\t" << iter->second << " persons colonized " << iter->first << " times" << std::endl;
	}

	os << "]";

	return os;
}

} /* namespace mrsa */
