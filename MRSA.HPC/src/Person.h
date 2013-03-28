/*
 * Person.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <map>
#include <boost/shared_ptr.hpp>

#include "relogo/Turtle.h"

#include "DiseaseStatusUpdater.h"
#include "Places.h"
#include "Activity.h"
#include "HospitalStayManager.h"
#include "Calendar.h"

namespace mrsa {

const int PERSON_ID_IDX = 0;
const int HH_ID_IDX = 1;
const int RELATE_IDX = 2;
const int SEX_IDX = 3;
const int AGE_IDX = 4;
const int GQ_ID_IDX = 5;
const int SCHOOL_ID_IDX = 6;
const int WORK_ID_IDX = 7;
const int TUCASE_ID_WEEKEND_IDX = 9;
const int TUCASE_ID_WEEKDAY_IDX = 8;
const int DAYCARE_ID_IDX = 10;
const int GYM_ID_IDX = 11;
const int HOSPITAL_ID_IDX = 12;
const int OTHER_H_START_IDX = 13;
const int OTHER_H_END_IDX = 16;

const int H_NIGHTS_1 = 17;
const int H_NIGHTS_2 = 18;
const int H_NIGHTS_3 = 19;
const int H_NIGHTS_4 = 20;
const int H_NIGHTS_5 = 21;

const int JAIL_IDX = 22;
const int SEEK_CARE_IDX = 23;
const int P_MRSA_IDX = 24;
const int INFECTED_IDX = 25;


// forward declaration
class CompositePlace;

class Person: public repast::relogo::Turtle {

	friend std::ostream& operator<<(std::ostream& os, const Person& id);
	friend class Statistics;

public:

	Person(repast::AgentId id, repast::relogo::Observer* obs, std::vector<std::string>& vec,
			Places places, boost::shared_ptr<PlaceStayManager> hosp_manager, boost::shared_ptr<PlaceStayManager> prison_manager,
			float min_infection_duration);
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
	void updateStatus(DiseaseStatus status, ColonizationCause cause = NA);

	/**
	 * Gets whether or not this Person's disease status can change. For example,
	 * if a Person is infected there is a minimum amount of time that has to pass
	 * with the Person in the infected state before he or she can become
	 * uninfected.
	 */
	bool canStatusChange();

	/**
	 * Increment the number colonization caused by this person by the specified
	 * amount.
	 *
	 * @param colonizations_caused the amount to increment by
	 */
	void incrementColonizationsCaused(float colonization_caused);

	/**
	 * Gets this Person's disease status.
	 */
	const DiseaseStatus status() const {
		return status_.status();
	}

	/**
	 * Gets the age of this Person.
	 */
	int age() const {
		return age_;
	}

	/**
	 * Gets the zip code of this person.
	 */
	unsigned int zipCode() const {
		return zip_code;
	}

	/**
	 * Gets the current infection status.
	 */
	InfectionStatus infectionStatus() const {
		return status_.infectionStatus();
	}

	/**
	 * Updates this Person's current infection status.
	 */
	void updateInfectionStatus(InfectionStatus);

	/**
	 * Gets the Place where this Person is currently.
	 */
	const Place* currentPlace() const {
		return places_.current;
	}

	/**
	 * Gets this Person's household.
	 */
	const Place* household() const {
		return places_.household;
	}

	const bool seekCare() const {
		return seek_care;
	}

	/**
	 * Initializes treatment within this Person's household.
	 */
	void initHouseholdTreatment();

	/**
	 * Makes this Person, go home. This typically sets the current place for this
	 * Person to its household, but it may move it to other places, such as
	 * general quarters etc.
	 */
	void goToHome();

	/**
	 * Makes this Person go to the specified compoment place.
	 */
	//void goToCompositePlace(CompositePlace* place, int activity_type);

	/**
	 * Performs the current activity for the specified time
	 * and weekday / weekend.
	 */
	void performActivity(Calendar& calendar); //int time, int day_of_year, int year,  bool isWeekday);

private:
	typedef std::vector<Activity> ActivityList;
	typedef std::vector<Activity>::const_iterator ActivityIter;

	std::string person_id;
	unsigned int zip_code;

	Places places_;
	boost::shared_ptr<PlaceStayManager> hosp_manager_;
	boost::shared_ptr<PlaceStayManager> prison_manager_;
	std::string tucaseid_weekday, tucaseid_weekend;
	int relate, sex, age_;
	ActivityList weekday_acts;
	ActivityList weekend_acts;

	DiseaseStatusUpdater status_;
	double entered_hospital_time, entered_prison_time;
	bool seek_care;

	/**
	 * Changes this Person's place to the specified place.
	 */
	void changePlace(Place* place, int activity_type);

	// checks whether this person should go to the hospital
	//bool hospitalCheck(int time);
};

std::ostream& operator<<(std::ostream& os, const Person& id);

} /* namespace mrsa */
#endif /* PERSON_H_ */
