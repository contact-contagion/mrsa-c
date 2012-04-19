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

enum PersonStatus {UNCOLONIZED, COLONIZED, INFECTED};

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

	const std::string& id() const {
		return person_id;
	}

	void updateStatus(PersonStatus status);

	PersonStatus getStatus() const {
		return _status;
	}

	void goToHome();
	void performActivity(int time, bool isWeekday);

	// increments appropriate counter (infected etc.) at
	// this Person's current place.
	void updatePlaceCounters();

	//void simpleTransition();
	void detailedPlaceTransition(float infection_period);
	//void detailedActivityTransition();

private:
	typedef std::vector<Activity*>* ActivityList;
	typedef std::vector<Activity*>::const_iterator ActivityIter;

	std::string person_id;
	Place* _household, *_group_quarters, *_work, *_school, *current;
	std::string tucaseid_weekday, tucaseid_weekend;
	int relate, sex, age;

	ActivityList weekday_acts;
	ActivityList weekend_acts;
	Activity* current_activity;

	double hourOfInfection;

	PersonStatus _status;

	void changePlace(Place* place);
};

std::ostream& operator<<(std::ostream& os, const Person& id);

class StatusCalculator {

private:
	static StatusCalculator* instance_;
	double a_, b_, c_, d_, e_;
	double scaling;

	PersonStatus chooseOne(double p1, double p2, double p3, double risk);

protected:
	StatusCalculator(double a, double b, double c, double d, double e, double scalingFactor);

public:
	static void initialize(double a, double b, double c, double d, double e, double scalingFactor);
	static StatusCalculator* instance();
	virtual ~StatusCalculator();

	PersonStatus next(PersonStatus start, PersonStatus other, double risk, bool faster);

};


} /* namespace mrsa */
#endif /* PERSON_H_ */
