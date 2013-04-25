/*
 * PlaceStats.h
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#ifndef PLACESTATS_H_
#define PLACESTATS_H_

#include <map>

#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/Properties.h"

namespace mrsa {

struct RegionPlaceStat {

	char region;
	long infection_count, colonization_count, stay_count;
	double duration;

	void reset() {
		infection_count = colonization_count = stay_count = 0;
		duration = 0;
	}

};

class PlaceStats {

public:
	PlaceStats();
	virtual ~PlaceStats();

	void incrementStayCount(unsigned int zip_code);
	void incrementDuration(double val, unsigned int zip_code);

	void incrementColonizationCount(unsigned int zip_code);
	void incrementInfectionCount(unsigned int zip_code);

	void createDataSources(const std::string& key_prefix, repast::SVDataSetBuilder& builder, bool include_region);

	void reset() {
		infection_count = colonization_count = stay_count = 0;
		duration_ = 0;
		for (std::map<char, RegionPlaceStat>::iterator iter = region_stats.begin();
				iter != region_stats.end(); ++iter) {
			iter->second.reset();
		}
	}

	void updateColonizationFraction(double colonization_sum) {
		col_fraction = colonization_sum == 0 ? 0 : colonization_count / colonization_sum;
	}

	void addToProps(repast::Properties& props, const std::string& key_prefix, int year, bool include_region);

	long infection_count, colonization_count;
	// the number individual stays in this place
	long stay_count;
	// total duration in hours
	double duration_;
	double col_fraction;

	std::map<char, RegionPlaceStat> region_stats;
};

} /* namespace mrsa */
#endif /* PLACESTATS_H_ */
