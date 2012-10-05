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

Person::Person(repast::AgentId id, repast::relogo::Observer* obs, std::vector<std::string>& vec, Places
		places, float min_infection_duration,
		bool seek_care) :
		Turtle(id, obs), person_id(vec[PERSON_ID_IDX]), places_(places), tucaseid_weekday(
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

	if (places.household != 0) {
		((Household*)places.household)->addMember(this);
	}

	//std::cout << places.current << std::endl;
}

Person::~Person() {
}

void Person::validate() {
	// remove this person from the model if it has no places
	if (places_.school == 0 && places_.household == 0 && places_.work == 0 && places_.group_quarters == 0)
		die();
	//if (places_.household == 0)
	//	die();
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
		if (places_.household != 0)
			changePlace(places_.household, 0);
		else
			changePlace(places_.group_quarters, 0);
	} else {
		changePlace(act->selectPlace(places_),act->activityType());
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
	if (places_.household != 0)
		changePlace(places_.household, 0);
	else if (places_.group_quarters != 0)
		changePlace(places_.group_quarters, 0);
	else if (places_.work != 0)
		changePlace(places_.work, 0);
	else if (places_.school != 0)
		changePlace(places_.school, 0);
}

// changes the place that this person is at.
void Person::changePlace(Place* place, int activity_type) {
	if (place != 0) {
		places_.current = place;
	}
	// regardless of whether this Person has changed its
	// current place, this method should only be called once
	// per tick. Consequently, the current should have had its
	// status counts etc. cleared prior to people changing place,
	// we need to add this person back to the current place.
	if (places_.current != 0)
		places_.current->addPerson(this, activity_type);
}

void Person::initSeekAndDestroy() {
	if (places_.household != 0) {
		((Household*)places_.household)->initSeekAndDestroy(this);
	}
}

std::ostream& operator<<(std::ostream& os, const Person& person) {
	const std::string& sid = person.places_.school == 0 ? "null" : person.places_.school->placeId();
	const std::string& hid = person.places_.household == 0 ? "null" : person.places_.household->placeId();
	const std::string& wid = person.places_.work == 0 ? "null" : person.places_.work->placeId();
	const std::string& gid =
			person.places_.group_quarters == 0 ? "null" : person.places_.group_quarters->placeId();
	os << "Person[" << person.person_id << ", home: " << hid << ", work: " << wid << ", school: "
			<< sid << ", gq: " << gid << "]";
	return os;
}

}
/* namespace mrsa */
