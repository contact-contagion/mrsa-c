/*
 * ActivityCreator.h
 *
 *  Created on: Apr 18, 2012
 *      Author: nick
 */

#ifndef ACTIVITYCREATOR_H_
#define ACTIVITYCREATOR_H_

#include <map>
#include <vector>

#include "Activity.h"

namespace mrsa {

/**
 * Creates the Activities from an activity csv file.
 */
class ActivityCreator {

public:
	ActivityCreator();
	virtual ~ActivityCreator();

	/**
	 * Runs this ActivityCreator on the specified file, put the results in into the specified map.
	 * The map key is the tucaseid, and value is a vector of all the activities with that id.
	 */
	void run(const std::string& file, std::map<std::string, std::vector<Activity> >& map);
};

} /* namespace mrsa */
#endif /* ACTIVITYCREATOR_H_ */
