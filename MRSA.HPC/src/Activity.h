/*
 * Activity.h
 *
 *  Created on: Apr 18, 2012
 *      Author: nick
 */

#ifndef ACTIVITY_H_
#define ACTIVITY_H_

#include <vector>
#include <string>

namespace mrsa {

class Activity {

public:
	Activity(std::vector<std::string>& vec);
	virtual ~Activity();

	float risk() const {
		return activity_risk + place_risk;
	}

	const std::string& id() const {
		return tucase_id;
	}

	const std::string& getPlaceType() const {
		return place_type;
	}

	const float getPlaceRisk() const {
		return place_risk;
	}

	bool within(int time) const {
		return start_time <= time && end_time > time;
	}

private:
	std::string tucase_id, place_type;
	int start_time, end_time, activity_risk;
	float place_risk;

};

} /* namespace mrsa */
#endif /* ACTIVITY_H_ */
