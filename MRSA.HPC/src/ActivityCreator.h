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

class ActivityCreator {

public:
	ActivityCreator();
	virtual ~ActivityCreator();

	void run(const std::string& file, std::map<std::string, std::vector<Activity> >& map);
};

} /* namespace mrsa */
#endif /* ACTIVITYCREATOR_H_ */
