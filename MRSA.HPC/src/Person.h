/*
 * Person.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <map>

#include "relogo/Turtle.h"

#include "DiseaseStatus.h"
#include "Place.h"
#include "Activity.h"

namespace mrsa {

const int PERSON_ID_IDX = 0;
const int HH_ID_IDX = 1;
const int RELATE_IDX = 2;
const int SEX_IDX = 3;
const int AGE_IDX = 4;
const int GQ_ID_IDX = 5;
const int SCHOOL_ID_IDX = 6;
const int WORK_ID_IDX = 7;
const int TUCASE_ID_WEEKEND_IDX = 8;
const int TUCASE_ID_WEEKDAY_IDX = 9;


class Person : public repast::relogo::Turtle {

	friend std::ostream& operator<<(std::ostream& os, const Person& id);

public:

	Person(repast::AgentId id, repast::relogo::Observer* obs, std::vector<std::string>& vec, Place* home, Place* group_quarters,
			Place* work, Place* school);
	virtual ~Person();

	/**
	 * If this Person has no Places, then it kills itself.
	 */
	void validate();

	/**
	 * Initialize the activity vectors for this Person, return true on successful
	 * initialization, otherwise false.
	 */
	bool initializeActivities(std::map<std::string, std::vector<Activity> >& map);

	/**
	 * Gets the person id of this Person. This is the id that is in the
	 * data files. Repast HPC will also assign this Persons an AgentId.
	 */
	const std::string& personId() const {
		return person_id;
	}

	/**
	 * Update this Person's disease status to the specified status.
	 */
	void updateStatus(DiseaseStatus status);

	/**
	 * Gets this Person's disease status.
	 */
	const DiseaseStatus& status() const {
		return status_;
	}

	/**
	 * Gets the age of this Person.
	 */
	int age() const {
		return age_;
	}

	/**
	 * Gets the Place where this Person is currently.
	 */
	const Place* currentPlace() const {
		return current;
	}

	/**
	 * Makes this Person, go home. This typically sets the current place for this
	 * Person to its household, but it may move it to other places, such as
	 * general quarters etc.
	 */
	void goToHome();

	/**
	 * Performs the current activity for the specified time
	 * and weekday / weekend.
	 */
	void performActivity(int time, bool isWeekday);

private:
	typedef std::vector<Activity> ActivityList;
	typedef std::vector<Activity>::const_iterator ActivityIter;

	std::string person_id;
	Place* _household, *_group_quarters, *_work, *_school, *current;
	std::string tucaseid_weekday, tucaseid_weekend;
	int relate, sex, age_;

	ActivityList weekday_acts;
	ActivityList weekend_acts;

	double hourOfInfection;

	DiseaseStatus status_;

	/**
	 * Changes this Person's place to the specified place.
	 */
	void changePlace(Place* place);
};

std::ostream& operator<<(std::ostream& os, const Person& id);

} /* namespace mrsa */
#endif /* PERSON_H_ */
