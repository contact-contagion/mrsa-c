/*
 * PlaceStats.h
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#ifndef PLACESTATS_H_
#define PLACESTATS_H_


#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/Properties.h"

namespace mrsa {

class PlaceStats {

public:
	PlaceStats();
	virtual ~PlaceStats();

	void incrementStayCount();
	void incrementDuration(double val);
	void createDataSources(const std::string& key_prefix, repast::SVDataSetBuilder& builder);

	void reset() {
		infection_count = colonization_count = stay_count = 0;
		duration_ = 0;
	}

	void addToProps(repast::Properties& props, const std::string& key_prefix, int year);

	long infection_count, colonization_count;
	// the number individual stays in this place
	long stay_count;
	// total duration in hours
	double duration_;
};

} /* namespace mrsa */
#endif /* PLACESTATS_H_ */
