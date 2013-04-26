/*
 * PersonStats.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: nick
 */

#define BOOST_FILESYSTEM_VERSION 2

#include <boost/filesystem.hpp>

#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/DataSet.h"

#include "Statistics.h"
#include "TransmissionAlgorithm.h"
#include "Constants.h"
#include "DataSourceAdapters.h"
#include "RegionMap.h"

namespace mrsa {

namespace fs = boost::filesystem;

using namespace repast::relogo;

void addValToProps(const std::string& key, int year, repast::Properties& props, long val) {
	std::stringstream ss;
	ss << key << "_year_" << year;
	props.putProperty(ss.str(), val);
}

void addValToProps(const std::string& key, int year, repast::Properties& props, double val) {
	std::stringstream ss;
	ss << key << "_year_" << year;
	props.putProperty(ss.str(), val);
}

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
				0), eoy_prevalence_infected(
				0), eoy_prevalence_colonized(0), yearly_no_seek_infection_duration(0), yearly_seek_infection_duration(
				0), yearly_infection_duration(0), yearly_colonization_duration(0), yearly_c_from_i(
				0), yearly_c_from_c(0), yearly_i_to_c_from_na(0), total_c_from_i(0), total_c_from_c(
				0), total_infected(0), total_colonized(0), colonization_from_infection_override(0), place_stats(), region_stats(), averages() {

	std::vector<char> regions;
	RegionMap::instance()->regions(regions);
	for (size_t i = 0; i < regions.size(); ++i) {
		//region_stats[regions[i]] = RegionStat(regions[i]);
		RegionStat stat = { regions[i], 0, 0, 0, 0, 0, 0 };
		region_stats.insert(std::pair<char, RegionStat>(regions[i], stat));
	}

	place_stats.insert(std::pair<std::string, PlaceStats>(HOUSEHOLD_TYPE, PlaceStats()));
	place_stats.insert(std::pair<std::string, PlaceStats>(OTHER_HOUSEHOLD_TYPE, PlaceStats()));
	place_stats.insert(std::pair<std::string, PlaceStats>(HOSPITAL_TYPE, PlaceStats()));
	place_stats.insert(std::pair<std::string, PlaceStats>(SCHOOL_TYPE, PlaceStats()));
	place_stats.insert(std::pair<std::string, PlaceStats>(WORKPLACE_TYPE, PlaceStats()));
	place_stats.insert(std::pair<std::string, PlaceStats>(GYM_TYPE, PlaceStats()));
	place_stats.insert(std::pair<std::string, PlaceStats>(NURSING_HOME_TYPE, PlaceStats()));
	place_stats.insert(std::pair<std::string, PlaceStats>(DORM_TYPE, PlaceStats()));
	place_stats.insert(std::pair<std::string, PlaceStats>(PRISON_TYPE, PlaceStats()));
}

Statistics::~Statistics() {
}

void Statistics::incrementColonizationCount(const std::string& type, const unsigned int zip_code) {
	place_stats[type].incrementColonizationCount(zip_code);
}

void Statistics::incrementColonizationFromInfection() {
	++colonization_from_infection_override;
}

void Statistics::incrementInfectionCount(const std::string& type, const unsigned int zip_code) {
	place_stats[type].incrementInfectionCount(zip_code);
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
	newly_infected = TransmissionAlgorithm::instance()->newlyInfectedCount();
	newly_colonized = TransmissionAlgorithm::instance()->newlyColonizedCount();

	total_infected += newly_infected;
	total_colonized += newly_colonized;
}

// StatusStats tracks the DiseaseStat, colonization count of other persons attributable to
// that state, and the duration of that state.
void Statistics::updateCountsFromStatsVector(Person* p, PersonStats& p_stats) {

	std::list<StatusStats>& vec = p->status_.yearly_status_stats;

	int i_count = 0, c_count = 0;
	double p_from_infection = 0, p_from_colonization = 0;
	RegionStat& region_stat = region_stats[RegionMap::instance()->region(p->zipCode())];
	for (std::list<StatusStats>::iterator iter = vec.begin(); iter != vec.end(); ++iter) {
		StatusStats& stats = *iter;

		if (stats.col_cause == UC_TO_C_FROM_C) {
			++yearly_c_from_c;
			++region_stat.c_from_c;
		} else if (stats.col_cause == UC_TO_C_FROM_I) {
			++yearly_c_from_i;
			++region_stat.c_from_i;
		} else if (stats.col_cause == I_TO_C_FROM_NA) {
			++yearly_i_to_c_from_na;
			++region_stat.i_to_c_from_na;
		}

		if (stats.duration != 0) {
			if (stats.status == INFECTED) {
				++i_count;
				// only count infections if they have not been
				// caused by initialization
				if (stats.col_cause != FROM_INIT) {
					++(p_stats.infection_count);
					++region_stat.infection_incidence;
				}
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
				// only count colonizations if they have not been
				// caused by initialization
				if (stats.col_cause != FROM_INIT) {
					++(p_stats.colonized_count);
					++region_stat.colonization_incidence;
				}

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

void Statistics::yearEnded(repast::Context<repast::relogo::RelogoAgent>::const_bytype_iterator begin,
		repast::Context<repast::relogo::RelogoAgent>::const_bytype_iterator end, int year,
		repast::Properties& props) {
	PersonStats p_stats = { };

	RegionMap* region_map = RegionMap::instance();
	// get yearly stats from each person.
	//for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
	for (repast::Context<repast::relogo::RelogoAgent>::const_bytype_iterator iter = begin; iter != end; ++iter) {
		Person* p = (Person*)((*iter).get());
		updateCountsFromStatsVector(p, p_stats);
		p->status_.resetYearlyCounts();
		// only count infections / colonizations that have been caused during the model run
		// and not as part of initialization
		if (p->status() == INFECTED
				&& p->status_.yearly_status_stats.back().col_cause != FROM_INIT) {
			++region_stats[region_map->region(p->zipCode())].infection_prevalence;
			++eoy_prevalence_infected;
		} else if (p->status() == COLONIZED
				&& p->status_.yearly_status_stats.back().col_cause != FROM_INIT) {
			++region_stats[region_map->region(p->zipCode())].colonization_prevalence;
			++eoy_prevalence_colonized;
		}
	}

	yearly_infected = p_stats.infection_count;
	yearly_colonized = p_stats.colonized_count;

	total_c_from_i += yearly_c_from_i;
	total_c_from_c += yearly_c_from_c;

	// Add to properties file
	addValToProps("infections_incidence", year, props, yearly_infected);
	addValToProps("colonizations_incidence", year, props, yearly_colonized);
	addValToProps("infections_prevalence", year, props, eoy_prevalence_infected);
	addValToProps("colonizations_prevalence", year, props, eoy_prevalence_colonized);

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

	place_stats[PRISON_TYPE].addToProps(props, "jail", year, true);
	place_stats[HOSPITAL_TYPE].addToProps(props, "hospital", year, false);

	addValToProps(HOUSEHOLD_COL_COUNT, year, props,  place_stats[HOUSEHOLD_TYPE].colonization_count);
	addValToProps(OTHER_H_COL_COUNT, year, props, place_stats[OTHER_HOUSEHOLD_TYPE].colonization_count);
	addValToProps(SCHOOL_COL_COUNT, year, props, place_stats[SCHOOL_TYPE].colonization_count);
	addValToProps(WORKPLACE_COL_COUNT, year, props, place_stats[WORKPLACE_TYPE].colonization_count);
	addValToProps(GYM_COL_COUNT, year, props, place_stats[GYM_TYPE].colonization_count);
	addValToProps(DORM_COL_COUNT, year, props, place_stats[DORM_TYPE].colonization_count);
	addValToProps(NURSING_HOME_COL_COUNT, year, props, place_stats[NURSING_HOME_TYPE].colonization_count);

	// get the total number of colonizations by summing the
	// colonization at X place counts
	double total = 0;
	for (std::map<std::string, PlaceStats>::iterator iter = place_stats.begin();
			iter != place_stats.end(); ++iter) {
		total += iter->second.colonization_count;
	}

	for (std::map<std::string, PlaceStats>::iterator iter = place_stats.begin();
			iter != place_stats.end(); ++iter) {
		iter->second.updateColonizationFraction(total);
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
			/*
			 << "avg. infected_r0: " << (averages.yearly_infected_r0 / averages.count) << std::endl
			 << "avg. colonized_r0: " << (averages.yearly_colonized_r0 / averages.count) << std::endl
			 */
			<< std::endl;

	props.putProperty("total_infections", total_infected);
	props.putProperty("colonizations", total_colonized);
	props.putProperty("total_from_infection", total_c_from_i);
	props.putProperty("total_from_colonization", total_c_from_c);
	/*
	 props.putProperty("avg_infected_r0", (averages.yearly_infected_r0 / averages.count));
	 props.putProperty("avg_colonized_r0", (averages.yearly_colonized_r0 / averages.count));
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

void Statistics::createHourlyDataSources(repast::SVDataSetBuilder& builder) {
	// data source for counting the number of uncolonized persons
	builder.addDataSource(
			repast::createSVDataSource("uncolonized_count",
					new LDataSourceAdapter(&hourly_uncolonized), std::plus<double>()));

	// data source for counting the number of colonized persons
	builder.addDataSource(
			repast::createSVDataSource("colonized_count", new LDataSourceAdapter(&hourly_colonized),
					std::plus<double>()));

	// data source for counting the number of infected persons
	builder.addDataSource(
			repast::createSVDataSource("infection_count", new LDataSourceAdapter(&hourly_infected),
					std::plus<double>()));

	// data source for the total number of persons
	TotalSum* tSum = new TotalSum(this);
	builder.addDataSource(repast::createSVDataSource("total_count", tSum, std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("newly_infected", new LDataSourceAdapter(&newly_infected),
					std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("newly_colonized", new LDataSourceAdapter(&newly_colonized),
					std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("running_total_infected",
					new DDataSourceAdapter(&total_infected), std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource("running_total_colonized",
					new DDataSourceAdapter(&total_colonized), std::plus<double>()));
}

void Statistics::createYearlyDataSources(repast::SVDataSetBuilder& builder) {
	builder.addDataSource(
			createSVDataSource("infection_incidence_count",
					new LDataSourceAdapter(&yearly_infected), std::plus<double>()));
	builder.addDataSource(
			createSVDataSource("colonized_incidence_count",
					new LDataSourceAdapter(&yearly_colonized), std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("infection_prevalence_count",
					new LDataSourceAdapter(&eoy_prevalence_infected), std::plus<double>()));
	builder.addDataSource(
			createSVDataSource("colonized_prevalence_count",
					new LDataSourceAdapter(&eoy_prevalence_colonized), std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("colonizations_from_infection",
					new LDataSourceAdapter(&yearly_c_from_i), std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("colonizations_from_colonization",
					new LDataSourceAdapter(&yearly_c_from_c), std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("infected_to_colonized_count",
					new LDataSourceAdapter(&yearly_i_to_c_from_na), std::plus<double>()));

	std::vector<char> regions;
	RegionMap::instance()->regions(regions);
	for (size_t i = 0; i < regions.size(); ++i) {
		RegionStat& region_stat = region_stats[regions[i]];
		std::stringstream ss;
		ss << regions[i];
		std::string region(ss.str());

		builder.addDataSource(
				createSVDataSource("infection_incidence_count_" + region,
						new LDataSourceAdapter(&(region_stat.infection_incidence)),
						std::plus<double>()));

		builder.addDataSource(
				createSVDataSource("colonized_incidence_count_" + region,
						new LDataSourceAdapter(&(region_stat.colonization_incidence)),
						std::plus<double>()));

		builder.addDataSource(
				createSVDataSource("infection_prevalence_count_" + region,
						new LDataSourceAdapter(&(region_stat.infection_prevalence)),
						std::plus<double>()));

		builder.addDataSource(
				createSVDataSource("colonized_prevalence_count_" + region,
						new LDataSourceAdapter(&(region_stat.colonization_prevalence)),
						std::plus<double>()));

		builder.addDataSource(
				createSVDataSource("colonizations_from_infection_" + region,
						new LDataSourceAdapter(&(region_stat.c_from_i)), std::plus<double>()));

		builder.addDataSource(
				createSVDataSource("colonizations_from_colonization_" + region,
						new LDataSourceAdapter(&(region_stat.c_from_c)), std::plus<double>()));

		builder.addDataSource(
				createSVDataSource("infected_to_colonized_count_" + region,
						new LDataSourceAdapter(&(region_stat.i_to_c_from_na)),
						std::plus<double>()));

	}

	builder.addDataSource(
			createSVDataSource("avg_seek_care_infection_duration",
					new DDataSourceAdapter(&yearly_seek_infection_duration), std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("avg_no_seek_care_infection_duration",
					new DDataSourceAdapter(&yearly_no_seek_infection_duration),
					std::plus<double>()));

	builder.addDataSource(
			createSVDataSource("avg_infection_duration",
					new DDataSourceAdapter(&yearly_infection_duration), std::plus<double>()));
	builder.addDataSource(
			createSVDataSource("avg_colonization_duration",
					new DDataSourceAdapter(&yearly_colonization_duration), std::plus<double>()));


	place_stats[PRISON_TYPE].createDataSources("jail", builder, true);
	place_stats[HOSPITAL_TYPE].createDataSources("hospital", builder, true);
	place_stats[HOUSEHOLD_TYPE].createDataSources(HOUSEHOLD_TYPE, builder, true);
	place_stats[OTHER_HOUSEHOLD_TYPE].createDataSources(OTHER_HOUSEHOLD_TYPE, builder, true);
	place_stats[SCHOOL_TYPE].createDataSources(SCHOOL_TYPE, builder, true);
	place_stats[WORKPLACE_TYPE].createDataSources(WORKPLACE_TYPE, builder, true);
	place_stats[GYM_TYPE].createDataSources("sports_facilities", builder, true);
	place_stats[NURSING_HOME_TYPE].createDataSources(NURSING_HOME_TYPE, builder, true);
	place_stats[DORM_TYPE].createDataSources(DORM_TYPE, builder, true);
}

/**
	 * Increments the hospital stay count.
	 */
	void Statistics:: incrementHospitalStayCount(unsigned int zip_code) {
		place_stats[HOSPITAL_TYPE].incrementStayCount(zip_code);
	}

	/**
	 * Increments the hospital stay duration count by the specified amount.
	 */
	void Statistics:: incrementHospitalDurationCount(double hours, unsigned int zip_code) {
		place_stats[HOSPITAL_TYPE].incrementDuration(hours, zip_code);
	}

	/**
	 * Increments the prison stay count.
	 */
	void Statistics:: incrementPrisonStayCount(unsigned int zip_code) {
		place_stats[PRISON_TYPE].incrementStayCount(zip_code);
	}

	/**
	 * Increments the prison stay duration count by the specified amount.
	 */
void Statistics:: incrementPrisonDurationCount(double hours, unsigned int zip_code) {
		place_stats[PRISON_TYPE].incrementDuration(hours, zip_code);
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

} /* namespace mrsa */
