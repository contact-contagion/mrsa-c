/*
 * Person.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <cassert>

#include "repast_hpc/Utilities.h"

#include "Person.h"
#include "Household.h"

namespace mrsa {

using namespace repast::relogo;
using namespace repast;
using namespace std;

Person::Person(repast::AgentId id, repast::relogo::Observer* obs, std::vector<std::string>& vec,
		Place* home, Place* other_home, Place* group_quarters, Place* work, Place* school, float min_infection_duration,
		bool seek_care) :
		Turtle(id, obs), person_id(vec[PERSON_ID_IDX]), _household(home), other_household(other_home), _group_quarters(
				group_quarters), _work(work), _school(school), current(0), tucaseid_weekday(
				vec[TUCASE_ID_WEEKDAY_IDX]), tucaseid_weekend(vec[TUCASE_ID_WEEKEND_IDX]), relate(
				0), sex(0), age_(0), weekday_acts(), weekend_acts(), status_(min_infection_duration), seek_care_(seek_care) {

	// parse the string values into ints for
	// relate, sex and age fields.
	std::string val = vec[RELATE_IDX];
	val = trim(val);
	if (val.length() > 0)
		relate = strToInt(val);

	val = vec[SEX_IDX];
	val = trim(val);
	if (val.length() > 0)
		sex = strToInt(val);

	val = vec[AGE_IDX];
	val = trim(val);
	if (val.length() > 0)
		age_ = strToInt(val);

	if (_household != 0) {
		((Household*)_household)->addMember(this);
	}
}

Person::~Person() {
}

void Person::validate() {
	// remove this person from the model if it has no places
	if (_school == 0 && _household == 0 && _work == 0 && _group_quarters == 0)
		die();
}

bool Person::canStatusChange() {
	return status_.canStatusChange();
}

// initialize the lists of activities for this Person by getting those
// lists from the map.
bool Person::initializeActivities(map<string, vector<Activity> >& map) {

	// find the weekday list in the map, if it doesn't exist
	// then return false
	std::map<string, vector<Activity> >::iterator iter = map.find(tucaseid_weekday);
	if (iter == map.end()) {
		return false;
	} else {
		//std::cout << iter->second << std::endl;
		weekday_acts = iter->second;
	}

	// find the weekend list in the map, if it doesn't
	// exist return false.
	iter = map.find(tucaseid_weekend);
	if (iter == map.end()) {
		return false;
	} else {
		//std::cout << iter->second << std::endl;
		weekend_acts = iter->second;
	}

	return true;
}

// peform the activity for this time and day.
void Person::performActivity(int time, bool isWeekday) {

	// iterate through the activity list and find the activity
	// whose time span contains the specified time.
	const Activity* act(0);
	// use the weekday or weekend list depending.
	// Using a reference here speeds up the model by a lot.
	const ActivityList& list = isWeekday ? weekday_acts : weekend_acts;
	for (ActivityIter iter = list.begin(); iter != list.end(); ++iter) {
		if (iter->contains(time)) {
			act = &(*iter);
			break;
		}
	}

	// if we don't find an activity for this time, go to a household
	// if it exists or to the group_quarters as a household stand-in.
	if (act == 0) {
		//std::cout << "no activity so going to household" << std::endl;
		if (_household != 0)
			changePlace(_household);
		else
			changePlace(_group_quarters);
	} else {
		// get the place type from the activity and go to the place named by
		// that type.
		const std::string& place_type = act->getPlaceType();
		if (place_type == "Household") {
			changePlace(_household);
		} else if (place_type == "Workplace") {
			changePlace(_work);

		} else if (place_type == "Other Household") {
			//std::cout << "going to other household" << std::endl;
			changePlace(other_household);
		} else if (place_type == "School") {
			changePlace(_school);
		} else if (place_type == "Group Quarters") {
			changePlace(_group_quarters);
		} else {
			// unknown place type so go to household if it exists or if not
			// go to group quarters.
			if (_household != 0)
				changePlace(_household);
			else
				changePlace(_group_quarters);
		}
	}
}

void Person::incrementColonizationsCaused(float colonization_caused) {
	status_.incrementColonizationsCaused(colonization_caused);
}

// sets the new disease status for this person.
void Person::updateStatus(DiseaseStatus status) {
	status_.updateStatus(status);
}

// makes the person to go "home" where home
// is in order of priority household, group quarters,
// workplace, or school.
void Person::goToHome() {
	if (_household != 0)
		changePlace(_household);
	else if (_group_quarters != 0)
		changePlace(_group_quarters);
	else if (_work != 0)
		changePlace(_work);
	else if (_school != 0)
		changePlace(_school);
}

// changes the place that this person is at.
void Person::changePlace(Place* place) {
	if (place != 0) {
		current = place;
	}
	// regardless of whether this Person has changed its
	// current place, this method should only be called once
	// per tick. Consequently, the current should have had its
	// status counts etc. cleared prior to people changing place,
	// we need to add this person back to the current place.
	current->addPerson(this);
}

void Person::initSeekAndDestroy() {
	if (_household != 0) {
		((Household*)_household)->initSeekAndDestroy(this);
	}
}

std::ostream& operator<<(std::ostream& os, const Person& person) {
	const std::string& sid = person._school == 0 ? "null" : person._school->placeId();
	const std::string& hid = person._household == 0 ? "null" : person._household->placeId();
	const std::string& wid = person._work == 0 ? "null" : person._work->placeId();
	const std::string& gid =
			person._group_quarters == 0 ? "null" : person._group_quarters->placeId();
	os << "Person[" << person.person_id << ", home: " << hid << ", work: " << wid << ", school: "
			<< sid << ", gq: " << gid << "]";
	return os;
}

}
/* namespace mrsa */
