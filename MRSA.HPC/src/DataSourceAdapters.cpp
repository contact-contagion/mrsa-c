/*
 * DataSourceAdapters.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */
#include <map>
#include <cmath>

#include "DataSourceAdapters.h"

namespace mrsa {

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

HoursToDaysDataSourceAdapter::HoursToDaysDataSourceAdapter(double* stat) :
		repast::TDataSource<double>(), stat_(stat) {
}

HoursToDaysDataSourceAdapter::~HoursToDaysDataSourceAdapter() {
}

double HoursToDaysDataSourceAdapter::getData() {
	return ceil((*stat_) / 24);
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


}

