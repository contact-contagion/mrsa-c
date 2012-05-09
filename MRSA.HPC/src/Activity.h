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
#include <iostream>

namespace mrsa {

/**
 * Names a places that a person will go to a particular time, together
 * with the disease risk associated with that going there.
 */
class Activity {

public:
	Activity(std::vector<std::string>& vec);
	// copy constructor
	Activity(const Activity& act);
	virtual ~Activity();

	/**
	 * Gets the risk associated with this activity.
	 */
	float risk() const {
		return activity_risk + place_risk;
	}

	/**
	 * Gets the id of this Activity.
	 */
	const std::string& id() const {
		return tucase_id;
	}

	/**
	 * Gets the place type of this activity.
	 */
	const std::string& getPlaceType() const {
		return place_type;
	}

	/**
	 * Gets the place risk associated with this activity.
	 */
	const float getPlaceRisk() const {
		return place_risk;
	}

	/**
	 * Gets whether or not the time space of this activity
	 * contains the specified time.
	 *
	 * @return true if it is does contain this time, otherwise false.
	 */
	bool contains(int time) const {
		//std::cout << place_type << ": " << start_time << " - " << end_time << std::endl;
		return start_time <= time && end_time > time;
	}

	// assignment operator
	Activity& operator=(const Activity&);

private:
	std::string tucase_id, place_type;
	int start_time, end_time;
	int activity_risk;
	float place_risk;

};

} /* namespace mrsa */
#endif /* ACTIVITY_H_ */
