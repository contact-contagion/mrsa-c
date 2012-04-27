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

#include "PersonStatus.h"
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
	bool initializeActivities(std::map<std::string, std::vector<Activity*> *>& map);

	const std::string& personId() const {
		return person_id;
	}

	void updateStatus(PersonStatus status);

	const PersonStatus& status() const {
		return status_;
	}

	int age() const {
		return age_;
	}


	void goToHome();
	void performActivity(int time, bool isWeekday);

private:
	typedef std::vector<Activity*>* ActivityList;
	typedef std::vector<Activity*>::const_iterator ActivityIter;

	std::string person_id;
	Place* _household, *_group_quarters, *_work, *_school, *current;
	std::string tucaseid_weekday, tucaseid_weekend;
	int relate, sex, age_;

	ActivityList weekday_acts;
	ActivityList weekend_acts;
	Activity* current_activity;

	double hourOfInfection;

	PersonStatus status_;

	void changePlace(Place* place);
};

std::ostream& operator<<(std::ostream& os, const Person& id);

} /* namespace mrsa */
#endif /* PERSON_H_ */
