/*
 * PlaceStats.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#include "repast_hpc/DataSet.h"

#include "PlaceStats.h"
#include "DataSourceAdapters.h"

namespace mrsa {

PlaceStats::PlaceStats() :
		infection_count(0), colonization_count(0), stay_count(0), duration_(0) {
}

PlaceStats::~PlaceStats() {
}

void PlaceStats::incrementStayCount() {
	++stay_count;
}

void PlaceStats::incrementDuration(double val) {
	duration_ += val;
}

void PlaceStats::createDataSources(const std::string& key_prefix,
		repast::SVDataSetBuilder& builder) {
	builder.addDataSource(
			repast::createSVDataSource(key_prefix + "_colonization_incidence",
					new LDataSourceAdapter(&colonization_count), std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource(key_prefix + "_infection_incidence",
					new LDataSourceAdapter(&infection_count), std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource(key_prefix + "_stays", new LDataSourceAdapter(&stay_count),
					std::plus<double>()));

	builder.addDataSource(
			repast::createSVDataSource(key_prefix + "_days",
					new HoursToDaysDataSourceAdapter(&duration_), std::plus<double>()));

}

void PlaceStats::addToProps(repast::Properties& props, const std::string& key_prefix, int year) {
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
}

} /* namespace mrsa */
