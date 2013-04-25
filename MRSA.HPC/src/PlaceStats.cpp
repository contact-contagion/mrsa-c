/*
 * PlaceStats.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#include "repast_hpc/DataSet.h"

#include "PlaceStats.h"
#include "DataSourceAdapters.h"
#include "RegionMap.h"

namespace mrsa {

PlaceStats::PlaceStats() :
		infection_count(0), colonization_count(0), stay_count(0), duration_(0), col_fraction(0), region_stats(){

	std::vector<char> regions;
	RegionMap::instance()->regions(regions);
	for (size_t i = 0; i < regions.size(); ++i) {
		RegionPlaceStat stat = { regions[i], 0, 0, 0, 0 };
		region_stats.insert(std::pair<char, RegionPlaceStat>(regions[i], stat));
	}
}

PlaceStats::~PlaceStats() {
}

void PlaceStats::incrementStayCount(unsigned int zip_code) {
	++stay_count;
	++region_stats[RegionMap::instance()->region(zip_code)].stay_count;
}

void PlaceStats::incrementDuration(double val, unsigned int zip_code) {
	duration_ += val;
	region_stats[RegionMap::instance()->region(zip_code)].duration += val;
}

void PlaceStats::incrementColonizationCount(unsigned int zip_code) {
	++colonization_count;
	++region_stats[RegionMap::instance()->region(zip_code)].colonization_count;
}

void PlaceStats::incrementInfectionCount(unsigned int zip_code) {
	++infection_count;
	++region_stats[RegionMap::instance()->region(zip_code)].infection_count;
}

void PlaceStats::createDataSources(const std::string& key_prefix, repast::SVDataSetBuilder& builder,
		bool include_region) {
	builder.addDataSource(
			repast::createSVDataSource(key_prefix + "_colonizations",
					new LDataSourceAdapter(&colonization_count), std::plus<double>()));

	builder.addDataSource(
				repast::createSVDataSource(key_prefix + "_colonization_fraction",
						new DDataSourceAdapter(&col_fraction), std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource(key_prefix + "_infections",
					new LDataSourceAdapter(&infection_count), std::plus<double>()));

	if (key_prefix == "jail" || key_prefix == "hospital") {
		builder.addDataSource(
				repast::createSVDataSource(key_prefix + "_stays",
						new LDataSourceAdapter(&stay_count), std::plus<double>()));

		builder.addDataSource(
				repast::createSVDataSource(key_prefix + "_days",
						new HoursToDaysDataSourceAdapter(&duration_), std::plus<double>()));
	}

	if (include_region) {
		for (std::map<char, RegionPlaceStat>::iterator iter = region_stats.begin();
				iter != region_stats.end(); ++iter) {

			const char& region = iter->first;
			RegionPlaceStat& stat = iter->second;

			builder.addDataSource(
					repast::createSVDataSource(key_prefix + "_colonizations_" + region,
							new LDataSourceAdapter(&stat.colonization_count), std::plus<double>()));

			builder.addDataSource(
					repast::createSVDataSource(key_prefix + "_infections_" + region,
							new LDataSourceAdapter(&stat.infection_count), std::plus<double>()));

			builder.addDataSource(
					repast::createSVDataSource(key_prefix + "_stays_" + region,
							new LDataSourceAdapter(&stat.stay_count), std::plus<double>()));

			builder.addDataSource(
					repast::createSVDataSource(key_prefix + "_days_" + region,
							new HoursToDaysDataSourceAdapter(&stat.duration), std::plus<double>()));
		}
	}
}

void PlaceStats::addToProps(repast::Properties& props, const std::string& key_prefix, int year,
		bool include_region) {
	std::stringstream ss;
	ss << key_prefix << "_colonizations_year_" << year;
	props.putProperty(ss.str(), colonization_count);

	ss.str("");
	ss.clear();
	ss << key_prefix << "_infections_year_" << year;
	props.putProperty(ss.str(), infection_count);

	ss.str("");
	ss.clear();
	ss << key_prefix << "_stays_year_" << year;
	props.putProperty(ss.str(), stay_count);

	ss.str("");
	ss.clear();
	ss << key_prefix << "_days_year_" << year;
	props.putProperty(ss.str(), ceil(duration_ / 24));

	if (include_region) {

		for (std::map<char, RegionPlaceStat>::iterator iter = region_stats.begin();
				iter != region_stats.end(); ++iter) {

			const char& region = iter->first;
			RegionPlaceStat& stat = iter->second;

			ss.str("");
			ss.clear();
			ss << key_prefix << "_colonizations_" << region << "_year_" << year;
			props.putProperty(ss.str(), stat.colonization_count);

			ss.str("");
			ss.clear();
			ss << key_prefix << "_infections_" << region << "_year_" << year;
			props.putProperty(ss.str(), stat.infection_count);

			ss.str("");
			ss.clear();
			ss << key_prefix << "_stays_" << region << "_year_" << year;
			props.putProperty(ss.str(), stat.stay_count);

			ss.str("");
			ss.clear();
			ss << key_prefix << "_days_" << region << "_year_" << year;
			props.putProperty(ss.str(), ceil(stat.duration / 24));
		}
	}

}

} /* namespace mrsa */
