/*
 * DataSourceAdapters.h
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#ifndef DATASOURCEADAPTERS_H_
#define DATASOURCEADAPTERS_H_

#include <map>
#include <string>

#include "repast_hpc/TDataSource.h"

namespace mrsa {

/**
 * Adapts long stat vars to the TDataSource interface.
 */
class LDataSourceAdapter: public repast::TDataSource<double> {

public:
	LDataSourceAdapter(long* stat);
	virtual ~LDataSourceAdapter();

	double getData();

private:
	long* stat_;
};

/**
 * Adapts double stat vars to the TDataSource interface.
 */
class DDataSourceAdapter: public repast::TDataSource<double> {

public:
	DDataSourceAdapter(double* stat);
	virtual ~DDataSourceAdapter();

	double getData();

private:
	double* stat_;
};

/**
 * Adapts double stat vars to the TDataSource interface and
 * divides the result by 24 to convert hours into days
 */
class HoursToDaysDataSourceAdapter: public repast::TDataSource<double> {

public:
	HoursToDaysDataSourceAdapter(double* stat);
	virtual ~HoursToDaysDataSourceAdapter();

	double getData();

private:
	double* stat_;
};


/**
 * Adapts map double values to the TDataSource API. The constructor
 * takes a map key and a map. getData returns the current value
 * for that key.
 */
class PlaceCount: public repast::TDataSource<double> {

public:
	PlaceCount(std::map<std::string, double>* place_map, const std::string& place_name);
	virtual ~PlaceCount();

	double getData();

private:

	std::map<std::string, double>* place_map_;
	std::string place_name_;
};

}



#endif /* DATASOURCEADAPTERS_H_ */
