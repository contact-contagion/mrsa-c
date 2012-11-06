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
#include "Constants.h"

namespace mrsa {

namespace fs = boost::filesystem;

using namespace repast::relogo;

/**
 * Attempts to find an unused file name. If the specified file
 * exists then this appends a _N (where N is a number) to the
 * non-extension part of the file, increasing N until a non-existent file is found.
 */
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

void increment_map_count(std::map<std::string, double>& map, const std::string& key) {
	std::map<std::string, double>::iterator iter = map.find(key);
	if (iter == map.end()) {
		map.insert(std::pair<std::string, double>(key, 1));
	} else {
		++(iter->second);
	}
}

double get_map_value(std::map<std::string, double>& map, const std::string& key) {
	std::map<std::string, double>::iterator iter = map.find(key);
	if (iter == map.end())
		return 0;
	return iter->second;
}

Statistics* Statistics::instance_ = 0;

Statistics* Statistics::getInstance() {
	if (instance_ == 0) {
		instance_ = new Statistics();
	}
	return instance_;
}

Statistics::Statistics() :

		hourly_infected(0), hourly_colonized(0), hourly_uncolonized(0), newly_infected(0), newly_colonized(
				0), yearly_infected_r0(0), yearly_colonized_r0(0), yearly_r0(0), yearly_infected(0), yearly_colonized(
				0), eoy_prevalence_infected(0), eoy_prevalence_colonized(0), yearly_no_seek_infection_duration(
				0), yearly_seek_infection_duration(0), yearly_infection_duration(0), yearly_colonization_duration(
				0), yearly_c_from_i(0), yearly_c_from_c(0), total_c_from_i(0), total_c_from_c(0), total_infected(
				0), total_colonized(0), hospital_stays(0), hospital_stay_duration(0), hospital_colonizations(
				0), hospital_infections(0), colonization_count_map(), infection_count_map(), averages() {
}

Statistics::~Statistics() {
}

void Statistics::incrementColonizationCount(const std::string& type) {
	increment_map_count(colonization_count_map, type);
}

void Statistics::incrementInfectionCount(const std::string& type) {
	increment_map_count(infection_count_map, type);
}

void Statistics::setInitialCounts(repast::relogo::AgentSet<Person>& people) {
	for (unsigned int i = 0, n = people.size(); i < n; i++) {
		Person* person = (people)[i];
		DiseaseStatus status = person->status();
		if (status == COLONIZED) {
			++total_colonized;
			++yearly_colonized;
		} else if (status == INFECTED) {
			++total_infected;
			++yearly_infected;
		}
	}
}

void Statistics::countPerson(Person* person) {
	// get the person's status and increment the appropriate count.
	DiseaseStatus status = person->status();
	if (status == UNCOLONIZED)
		++hourly_uncolonized;
	else if (status == COLONIZED)
		++hourly_colonized;
	else if (status == INFECTED)
		++hourly_infected;
}

void Statistics::hourEnded() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	yearly_c_from_i += ta->colonizedFromInfectionCount();
	yearly_c_from_c += ta->colonizedFromColonizationCount();

	total_c_from_i += ta->colonizedFromInfectionCount();
	total_c_from_c += ta->colonizedFromColonizationCount();

	newly_infected = TransmissionAlgorithm::instance()->newlyInfectedCount();
	newly_colonized = TransmissionAlgorithm::instance()->newlyColonizedCount();

	yearly_colonized += newly_colonized;
	yearly_infected += newly_infected;

	total_infected += newly_infected;
	total_colonized += newly_colonized;
}

// StatusStats tracks the DiseaseStat, colonization count of other persons attributable to
// that state, and the duration of that state.
void Statistics::updateCountsFromStatsVector(Person* p, PersonStats& p_stats) {

	std::list<StatusStats>& vec = p->status_.yearly_status_stats;

	int i_count = 0, c_count = 0;
	double p_from_infection = 0, p_from_colonization = 0;
	for (std::list<StatusStats>::iterator iter = vec.begin(); iter != vec.end(); ++iter) {
		StatusStats& stats = *iter;
		if (stats.duration != 0) {
			if (stats.status == INFECTED) {
				++i_count;
				++(p_stats.infection_count);
				p_stats.infection_duration += stats.duration;
				if (stats.infection_status == SEEK_CARE) {
					p_stats.seek_infection_duration += stats.duration;
					++(p_stats.seek_infection_count);
				} else {
					p_stats.no_seek_infection_duration += stats.duration;
					++(p_stats.no_seek_infection_count);
				}
				p_stats.from_infection += stats.colonized_persons;
				p_from_infection += stats.colonized_persons;

			} else if (stats.status == COLONIZED) {
				++c_count;
				++(p_stats.colonized_count);
				p_stats.colonization_duration += stats.duration;
				p_stats.from_colonization += stats.colonized_persons;
				p_from_colonization += stats.colonized_persons;
			}
		} else {
			// if the duration is 0, then this state is still active
			// so we don't have complete data on it yet.
			break;
		}
	}

	// rhs is the avg r0 for this person over the last year
	// then this added to a running sum which will use to take the average
	// over all the persons.
	if (i_count > 0) {
		p_stats.avg_infection_r0 += p_from_infection / i_count;
		++p_stats.infected_person_count;
	}

	if (c_count > 0) {
		p_stats.avg_colonization_r0 += p_from_colonization / c_count;
		++p_stats.colonized_person_count;
	}

	if (c_count > 0 || i_count > 0) {
		++p_stats.i_or_c_count;
		p_stats.avg_r0 += (p_from_infection + p_from_colonization) / (i_count + c_count);
	}
}

void Statistics::yearEnded(repast::relogo::AgentSet<Person>& people, int year,
		repast::Properties& props) {
	PersonStats p_stats = { };

	// get yearly stats from each person.
	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
		Person* p = (*iter);
		updateCountsFromStatsVector(p, p_stats);
		p->status_.resetYearlyCounts();
		if (p->status() == INFECTED)
			++eoy_prevalence_infected;
		else if (p->status() == COLONIZED)
			++eoy_prevalence_colonized;
	}

	// Add to properties file
	std::stringstream ss1;
	ss1 << "infections_incidence_year_" << year;
	props.putProperty(ss1.str(), yearly_infected);

	std::stringstream ss2;
	ss2 << "colonizations_incidence_year_" << year;
	props.putProperty(ss2.str(), yearly_colonized);

	std::stringstream ss3;
	ss3 << "infections_prevalence_year_" << year;
	props.putProperty(ss3.str(), eoy_prevalence_infected);

	std::stringstream ss4;
	ss4 << "colonizations_prevalence_year_" << year;
	props.putProperty(ss4.str(), eoy_prevalence_colonized);

	yearly_infection_duration = yearly_colonization_duration = 0;

	// average infection duration over all persons = total duration / number of times infected
	if (p_stats.infection_count != 0) {
		yearly_infection_duration = p_stats.infection_duration / p_stats.infection_count;
		yearly_seek_infection_duration = p_stats.seek_infection_duration
				/ p_stats.seek_infection_count;
		yearly_no_seek_infection_duration = p_stats.no_seek_infection_duration
				/ p_stats.no_seek_infection_count;
	}

	// average colonization duration over all persons = total duration / number of time colonized
	if (p_stats.colonized_count != 0) {
		yearly_colonization_duration = p_stats.colonization_duration / p_stats.colonized_count;
	}

	// divide the sum of all the r0 averages by the number of persons that were
	// infected / colonized (i.e. the number of people that contributed to that sum)
	// to get the yearly r0s.
	yearly_infected_r0 = p_stats.avg_infection_r0 / p_stats.infected_person_count;
	yearly_colonized_r0 = p_stats.avg_colonization_r0 / p_stats.colonized_person_count;
	yearly_r0 = p_stats.avg_r0 / p_stats.i_or_c_count;

	++averages.count;
	averages.yearly_infected_r0 += yearly_infected_r0;
	averages.yearly_colonized_r0 += yearly_colonized_r0;
	averages.yearly_infection_duration += yearly_infection_duration;
	averages.yearly_colonization_duration += yearly_colonization_duration;
	averages.yearly_no_seek_infection_duration += yearly_no_seek_infection_duration;
	averages.yearly_seek_infection_duration += yearly_seek_infection_duration;

	hospital_colonizations = get_map_value(colonization_count_map, HOSPITAL_TYPE);
	hospital_infections = get_map_value(infection_count_map, HOSPITAL_TYPE);
	// convert this to days from hours.
	hospital_stay_duration = ceil(hospital_stay_duration / 24);

	std::stringstream ss5;
	ss5 << "hospital_colonizations_year_" << year;
	props.putProperty(ss5.str(), hospital_colonizations);

	std::stringstream ss6;
	ss6 << "hospital_infections_year_" << year;
	props.putProperty(ss6.str(), hospital_infections);

	std::stringstream ss7;
	ss7 << "hospital_stays_year_" << year;
	props.putProperty(ss7.str(), hospital_stays);

	std::stringstream ss8;
	ss8 << "hospital_days_year_" << year;
	props.putProperty(ss8.str(), hospital_stay_duration);

	// get the total number of colonizations by summing the
	// colonization at X place counts
	double total = 0;
	for (ColMapIter iter = colonization_count_map.begin(); iter != colonization_count_map.end();
			++iter) {
		total += iter->second;
	}

	// for each place in the colonization_count_map, get the fraction of colonizations
	// that occured in that place
	for (ColMapIter iter = colonization_count_map.begin(); iter != colonization_count_map.end();
			++iter) {
		double val = iter->second;
		iter->second = val / total;
	}
}

void Statistics::calculateSummaryStats(repast::relogo::AgentSet<Person>& people,
		const std::string& file, repast::Properties& props) {
	std::map<unsigned int, unsigned long> infection_hist;
	std::map<unsigned int, unsigned long> colonized_hist;

	PersonStats p_stats = { };

	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
		Person* p = (*iter);
		// setting to UNCOLONIZED will update any "open" stats
		// given that states are only counted when transitioning out
		// of them.
		//p->updateStatus(UNCOLONIZED);
		// update the status lists with the latest info
		// this will copy the yearly into the total
		p->status_.resetYearlyCounts();
		// update the counts from vector containing all the status stats
		// rather than the yearly one.
		updateCountsFromStatsVector(p, p_stats);

		addToHistogram(p->status_.infected_count, infection_hist);
		addToHistogram(p->status_.colonized_count, colonized_hist);

	}
	//std::cout << "summary avg_infection: " << p_stats.avg_infection_r0 << ", summary infected p count: " << p_stats.infected_person_count << std::endl;
	//std::cout << "summary infection_r0: "
	//		<< p_stats.avg_infection_r0 / p_stats.infected_person_count << std::endl;

	std::ofstream out;
	out.open(findFreeFile(file).c_str());

	// write the stats out to the file
	out << "total infections: " << total_infected << std::endl << "total colonizations: "
			<< total_colonized << std::endl << "total from infection: " << total_c_from_i
			<< std::endl << "total from colonization: " << total_c_from_c << std::endl
			<< "avg. seek care infection duration: "
			<< averages.yearly_seek_infection_duration / averages.count << std::endl
			<< "avg. no seek care infection duration: "
			<< averages.yearly_no_seek_infection_duration / averages.count << std::endl
			<< "avg. infection duration: " << averages.yearly_infection_duration / averages.count
			<< std::endl << "avg. colonization duration: "
			<< averages.yearly_colonization_duration / averages.count << std::endl
			<< "avg. infected_r0: " << (averages.yearly_infected_r0 / averages.count) << std::endl
			<< "avg. colonized_r0: " << (averages.yearly_colonized_r0 / averages.count) << std::endl
			<< std::endl;

	props.putProperty("total_infections", total_infected);
	props.putProperty("colonizations", total_colonized);
	props.putProperty("total_from_infection", total_c_from_i);
	props.putProperty("total_from_colonization", total_c_from_c);
	props.putProperty("avg_infected_r0", (averages.yearly_infected_r0 / averages.count));
	props.putProperty("avg_colonized_r0", (averages.yearly_colonized_r0 / averages.count));

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
