/*
 * PersonStats.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: nick
 */

#define BOOST_FILESYSTEM_VERSION 2

#include <boost/filesystem.hpp>

#include "Statistics.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

namespace fs = boost::filesystem;

using namespace repast::relogo;

std::string findFreeFile(const std::string& file) {
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

	return filepath.string();
}

Statistics* Statistics::instance_ = 0;

Statistics* Statistics::getInstance() {
	if (instance_ == 0) {
		instance_ = new Statistics();
	}
	return instance_;
}

Statistics::Statistics() :

		hourly_infected(0), hourly_colonized(0), hourly_uncolonized(0), yearly_infected_r0(0), yearly_colonized_r0(
				0),

		/* prev_total_infected(0), prev_total_colonized(
		 0), ni_over_ti(0), ni_over_tc(0), nc_over_ti(0), nc_over_tc(0), ti_over_pti(0), tc_over_ptc(
		 0), count(0), ni_over_tc_sum(0), nc_over_tc_sum(0), nc_over_ti_sum(0), ni_over_ti_sum(
		 0), ti_over_pti_sum(0), tc_over_ptc_sum(0),*/yearly_infected(0), yearly_colonized(0), yearly_infection_duration(
				0), yearly_colonization_duration(0), yearly_c_from_i(0), yearly_c_from_c(0), colonization_count_map() {
}

Statistics::~Statistics() {
}

void Statistics::incrementColonizationCount(const std::string& type) {
	ColMapIter iter = colonization_count_map.find(type);
	if (iter == colonization_count_map.end()) {
		colonization_count_map.insert(std::pair<std::string, unsigned long>(type, 1));
	} else {
		++(iter->second);
	}

}

void Statistics::countPerson(Person* person) {
	// get the person's status and increment the appropriate count.
	DiseaseStatus status = person->status();
	if (status == UNCOLONIZED)
		hourly_uncolonized++;
	else if (status == COLONIZED)
		hourly_colonized++;
	else if (status == INFECTED)
		hourly_infected++;
}

void Statistics::calculateHourlyStats() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	yearly_c_from_i += ta->colonizedFromInfectionCount();
	yearly_c_from_c += ta->colonizedFromColonizationCount();

	/*
	 ni_over_tc = newly_infected / total_colonized;
	 nc_over_tc = newly_colonized / total_colonized;
	 nc_over_ti = newly_colonized / total_infected;
	 ni_over_ti = newly_infected / total_infected;

	 ti_over_pti = total_infected / ((double) prev_total_infected);
	 tc_over_ptc = total_colonized / ((double) prev_total_colonized);

	 ++count;
	 ni_over_tc_sum += ni_over_tc;
	 nc_over_tc_sum += nc_over_tc;
	 nc_over_ti_sum += nc_over_ti;
	 ni_over_ti_sum += ni_over_ti;

	 ti_over_pti_sum += ti_over_pti;
	 tc_over_ptc_sum += tc_over_ptc;
	 */
}

// replace params with a struct, pass by referece
void Statistics::updateCountsFromStatsVector(std::list<StatusStats> vec, PersonStats& p_stats) {

	int i_count = 0, c_count = 0;
	double p_from_infection = 0, p_from_colonization;
	bool incr_infected = true, incr_colonized = true;
	for (std::list<StatusStats>::iterator iter = vec.begin(); iter != vec.end(); ++iter) {
		StatusStats& stats = *iter;
		if (stats.duration != 0) {
			if (stats.status == INFECTED) {
				++i_count;
				++(p_stats.infection_count);
				p_stats.infection_duration += stats.duration;
				p_stats.from_infection += stats.colonized_persons;
				p_from_infection += stats.colonized_persons;
				if (incr_infected) {
					incr_infected = false;
					++(p_stats.infected_person_count);
				}
			} else if (stats.status == COLONIZED) {
				++c_count;
				++(p_stats.colonized_count);
				(p_stats.colonization_duration) += stats.duration;
				(p_stats.from_colonization) += stats.colonized_persons;
				p_from_colonization += stats.colonized_persons;
				if (incr_colonized) {
					incr_colonized = false;
					++(p_stats.colonized_person_count);
				}
			}
		} else {
			// is still in this state so don't count
			break;
		}
	}

	// rhs is the avg r0 for this person over the last year
	// then this added to a running sum which will use to take the average
	// over all the persons.
	if (i_count > 0)
		p_stats.avg_infection_r0 += p_from_infection / i_count;
	if (c_count > 0)
		p_stats.avg_colonization_r0 += p_from_colonization / c_count;

}

void Statistics::calculateYearlyStats(repast::relogo::AgentSet<Person>& people) {
	PersonStats p_stats = { 0L, 0L, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0L, 0L};

	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
		Person* p = (*iter);
		updateCountsFromStatsVector(p->status_.yearly_status_stats, p_stats);
		p->status_.clearYearlyCounts();
	}

	yearly_infection_duration = yearly_colonization_duration = 0;

	if (p_stats.infection_count != 0)
		yearly_infection_duration = p_stats.infection_duration / p_stats.infection_count;
	if (p_stats.colonized_count != 0)
		yearly_colonization_duration = p_stats.colonization_duration / p_stats.colonized_count;

	// divide the sum of all the r0 averages by the number of persons that were
	// infected / colonized (i.e. the number of people that contributed to that sum)
	// to get the yearly r0s.
	yearly_infected_r0 = p_stats.avg_infection_r0 / p_stats.infected_person_count;
	yearly_colonized_r0 = p_stats.avg_colonization_r0 / p_stats.colonized_person_count;
	//yearly_colonized_r0 = p_stats.from_colonization / p_stats.colonized_count;

	yearly_colonized = p_stats.colonized_count;
	yearly_infected = p_stats.infection_count;

	double total = 0;
	for (ColMapIter iter = colonization_count_map.begin(); iter != colonization_count_map.end();
			++iter) {
		total += iter->second;
	}

	for (ColMapIter iter = colonization_count_map.begin(); iter != colonization_count_map.end();
			++iter) {
		double val = iter->second;
		iter->second = val / total;
	}
}

void Statistics::calculateSummaryStats(repast::relogo::AgentSet<Person>& people,
		const std::string& file) {
	std::map<unsigned int, unsigned long> infection_hist;
	std::map<unsigned int, unsigned long> colonized_hist;

	PersonStats p_stats = { 0L, 0L, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
		Person* p = (*iter);
		// setting to UNCOLONIZED will update any "open" stats
		// given that states are only counted when transitioning out
		// of them.
		p->updateStatus(UNCOLONIZED);
		// update the status lists with the latest info
		p->status_.clearYearlyCounts();
		updateCountsFromStatsVector(p->status_.total_status_stats, p_stats);

		addToHistogram(p->status_.infected_count, infection_hist);
		addToHistogram(p->status_.colonized_count, colonized_hist);
	}

	std::ofstream out;

	out.open(findFreeFile(file).c_str());

	out << "total infections: " << p_stats.infection_count << std::endl << "total colonization: "
			<< p_stats.colonized_count << std::endl << "total from infection: "
			<< p_stats.from_infection << std::endl << "total from colonization: "
			<< p_stats.from_colonization << std::endl << "avg. infection duration: "
			<< p_stats.infection_duration / p_stats.infection_count << std::endl
			<< "avg. colonization duration: "
			<< p_stats.colonization_duration / p_stats.colonized_count << std::endl

			<< "avg. infected_r0: " << (p_stats.avg_infection_r0 / p_stats.infection_count)
			<< std::endl << "avg. colonized_r0: "
			<< (p_stats.avg_colonization_r0 / p_stats.colonized_count) << std::endl;

	/*
	 out << "newly_infected_over_total_infected_r0: " << (ni_over_ti_sum / count) << std::endl
	 << "newly_infected_over_total_colonized_r0: " << (ni_over_tc_sum / count) << std::endl
	 << "newly_colonized_over_total_infected_r0: " << (nc_over_ti_sum / count) << std::endl
	 << "newly_colonized_over_total_colonized_r0: " << (nc_over_tc_sum / count) << std::endl

	 << "avg. total infected over prev total infected: " << (ti_over_pti_sum / count)
	 << std::endl << "avg. total colonized over prev total colonized: "
	 << (tc_over_ptc_sum / count) << std::endl;
	 */

	for (ConstHistIter iter = infection_hist.begin(); iter != infection_hist.end(); ++iter) {
		out << "" << iter->second << " persons infected " << iter->first << " times" << std::endl;
	}

	for (ConstHistIter iter = colonized_hist.begin(); iter != colonized_hist.end(); ++iter) {
		out << "" << iter->second << " persons colonized " << iter->first << " times" << std::endl;
	}

	out.close();

}

void Statistics::addToHistogram(unsigned int count, std::map<unsigned int, unsigned long>& hist) {
	HistIter iter = hist.find(count);
	if (iter == hist.end()) {
		hist.insert(std::pair<unsigned int, unsigned long>(count, 1));
	} else {
		++(iter->second);
	}
}

LDataSourceAdapter::LDataSourceAdapter(long* stat) :
		repast::TDataSource<double>(), stat_(stat) {
}

LDataSourceAdapter::~LDataSourceAdapter() {
}

double LDataSourceAdapter::getData() {
	return (double) (*stat_);
}

DDataSourceAdapter::DDataSourceAdapter(double* stat) :
		repast::TDataSource<double>(), stat_(stat) {
}

DDataSourceAdapter::~DDataSourceAdapter() {
}

double DDataSourceAdapter::getData() {
	return *stat_;
}

TotalSum::TotalSum(Statistics* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

TotalSum::~TotalSum() {
}

/**
 * Gets the total number of persons using
 * the wrapped PersonStats object.
 */
double TotalSum::getData() {
	return stats_->hourly_uncolonized + stats_->hourly_colonized + stats_->hourly_infected;
}

PlaceCount::PlaceCount(std::map<std::string, double>* place_map, const std::string& place_name) :
		repast::TDataSource<double>(), place_map_(place_map), place_name_(place_name) {
}

PlaceCount::~PlaceCount() {
}

double PlaceCount::getData() {
	std::map<std::string, double>::iterator iter = place_map_->find(place_name_);
	if (iter == place_map_->end())
		return 0;
	return iter->second;
}

} /* namespace mrsa */
