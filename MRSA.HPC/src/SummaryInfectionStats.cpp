/*
 * SummaryInfectionStats.cpp
 *
 *  Created on: May 2, 2012
 *      Author: nick
 */

// Needed for Boost 1_46 and later due
// to changes in the Boost file system API
#define BOOST_FILESYSTEM_VERSION 2

#include <boost/filesystem.hpp>

#include "SummaryInfectionStats.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

using namespace repast::relogo;

namespace fs = boost::filesystem;

SummaryInfectionStats::SummaryInfectionStats() :
		infection_hist(), colonized_hist(), infection_count(0), colonized_count(0), infection_duration(
				0), colonization_duration(0), colonization_from_colonization_r0(0), colonization_from_infection_r0(
				0) {

}

SummaryInfectionStats::~SummaryInfectionStats() {
}

void SummaryInfectionStats::write(Statistics* p_stats, const std::string& file) const {

	fs::path filepath(file);
	if (!fs::exists(filepath.parent_path()))
		fs::create_directories(filepath.parent_path());
	int i = 1;
	std::string stem = filepath.stem();
	// This will increment i until it hits a unique name
	while (fs::exists(filepath)) {
		++i;
		std::stringstream ss;
		ss << stem << "_" << i << filepath.extension();
		fs::path newName(filepath.parent_path() / ss.str());
		filepath = newName;
	}

	std::ofstream out;
	out.open(filepath.string().c_str());

	out << "total infections: " << infection_count << std::endl << "total colonization: "
			<< colonized_count << std::endl << "avg. infection duration: " << infection_duration
			<< std::endl << "avg. colonization duration: " << colonization_duration << std::endl;

	out << "newly_infected_over_total_infected_r0: " << (p_stats->ni_over_ti_sum / p_stats->count) << std::endl
					<< "newly_infected_over_total_colonized_r0: " << (p_stats->ni_over_tc_sum / p_stats->count) << std::endl
					<< "newly_colonized_over_total_infected_r0: " << (p_stats->nc_over_ti_sum / p_stats->count) << std::endl
					<< "newly_colonized_over_total_colonized_r0: " << (p_stats->nc_over_tc_sum /p_stats->count) << std::endl

					<< "avg. total infected over prev total infected: " << (p_stats->ti_over_pti_sum / p_stats->count) << std::endl
					<< "avg. total colonized over prev total colonized: " << (p_stats->tc_over_ptc_sum / p_stats->count) << std::endl;

	for (SummaryInfectionStats::ConstHistIter iter = infection_hist.begin();
			iter != infection_hist.end(); ++iter) {
		out << "" << iter->second << " persons infected " << iter->first << " times" << std::endl;
	}

	for (SummaryInfectionStats::ConstHistIter iter = colonized_hist.begin();
			iter != colonized_hist.end(); ++iter) {
		out << "" << iter->second << " persons colonized " << iter->first << " times" << std::endl;
	}




	out.close();
}

void SummaryInfectionStats::gatherStats(AgentSet<Person>& people) {

	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
		Person* p = (*iter);
		// setting to UNCOLONIZED will update any "open" stats
		// given that states are only counted when transitioning out
		// of them.
		p->updateStatus(UNCOLONIZED);
		unsigned int i_count = p->status_.infected_count;
		addToHistogram(i_count, infection_hist);
		infection_count += i_count;
		unsigned int c_count = p->status_.colonized_count;
		addToHistogram(c_count, colonized_hist);
		colonized_count += c_count;
		infection_duration += p->status_.infected_time;
		colonization_duration += p->status_.colonized_time;
	}

	if (infection_count != 0)
		infection_duration = infection_duration / infection_count;
	if (colonized_count != 0)
		colonization_duration = colonization_duration / colonized_count;

}

void SummaryInfectionStats::addToHistogram(unsigned int count,
		std::map<unsigned int, unsigned long>& hist) {
	HistIter iter = hist.find(count);
	if (iter == hist.end()) {
		hist.insert(std::pair<unsigned int, unsigned long>(count, 1));
	} else {
		++(iter->second);
	}
}

std::ostream& operator<<(std::ostream& os, const SummaryInfectionStats& stats) {
	os << "SummaryInfectionStats[" << std::endl << "\ttotal infections: " << stats.infection_count
			<< std::endl << "\ttotal colonization: " << stats.colonized_count << std::endl
			<< "\tavg. infection duration: " << stats.infection_duration << std::endl
			<< "\tavg. colonization duration: " << stats.colonization_duration << std::endl;

	for (SummaryInfectionStats::ConstHistIter iter = stats.infection_hist.begin();
			iter != stats.infection_hist.end(); ++iter) {
		os << "\t" << iter->second << " persons infected " << iter->first << " times" << std::endl;
	}

	for (SummaryInfectionStats::ConstHistIter iter = stats.colonized_hist.begin();
			iter != stats.colonized_hist.end(); ++iter) {
		os << "\t" << iter->second << " persons colonized " << iter->first << " times" << std::endl;
	}

	os << "]";

	return os;
}

} /* namespace mrsa */
