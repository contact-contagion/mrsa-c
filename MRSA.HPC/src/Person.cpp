/*
 * Person.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <cassert>

#include "repast_hpc/Utilities.h"

#include "Person.h"

namespace mrsa {

using namespace repast::relogo;
using namespace repast;
using namespace std;

Person::Person(repast::AgentId id, repast::relogo::Observer* obs,
		std::vector<std::string>& vec, Place* home, Place* group_quarters,
		Place* work, Place* school) :
		Turtle(id, obs), person_id(vec[PERSON_ID_IDX]), _household(home), _group_quarters(
				group_quarters), _work(work), _school(school), current(0), tucaseid_weekday(
				vec[TUCASE_ID_WEEKDAY_IDX]), tucaseid_weekend(
				vec[TUCASE_ID_WEEKEND_IDX]), relate(0), sex(0), age_(0), weekday_acts(
				0), weekend_acts(0), current_activity(0), hourOfInfection(0), status_(UNCOLONIZED){

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
}

Person::~Person() {
}

void Person::validate() {
	if (_school == 0 && _household == 0 && _work == 0 && _group_quarters == 0)
		die();
}

bool Person::initializeActivities(map<string, vector<Activity*>*>& map) {

	std::map<string, vector<Activity*>*>::iterator iter = map.find(
			tucaseid_weekday);
	if (iter == map.end()) {
		weekday_acts = 0;
		return false;
	} else {
		//std::cout << iter->second << std::endl;
		weekday_acts = iter->second;
	}

	iter = map.find(tucaseid_weekend);
	if (iter == map.end()) {
		weekend_acts = 0;
		return false;
	} else {
		//std::cout << iter->second << std::endl;
		weekend_acts = iter->second;
	}

	return true;
}

void Person::performActivity(int time, bool isWeekday) {
	// find the current activity
	current_activity = 0;
	ActivityList list = isWeekday ? weekday_acts : weekend_acts;
	for (ActivityIter iter = list->begin(); iter != list->end(); ++iter) {
		if ((*iter)->within(time)) {
			current_activity = (*iter);
			break;
		}
	}

	if (current_activity == 0) {
		//std::cout << "no activity so going to household" << std::endl;
		changePlace(_household);
	} else {
		const std::string& place_type = current_activity->getPlaceType();
		//std::cout << (*this) << " going to: " << place_type << std::endl;
		if (place_type == "Household") {
			changePlace(_household);
		} else if (place_type == "Workplace") {
			changePlace(_work);
		} else if (place_type == "School") {
			changePlace(_school);
		} else if (place_type == "Group Quarters") {
			changePlace(_group_quarters);
		} else {
			// otherwise go home
			changePlace(_household);
		}
	}
}

void Person::updateStatus(PersonStatus status) {
	if (status_ != INFECTED && status == INFECTED) {
		hourOfInfection =
				RepastProcess::instance()->getScheduleRunner().currentTick();
	}
	status_ = status;
}

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

void Person::changePlace(Place* place) {
	if (place != 0) {
		current = place;
	}
	// regardless of whether current is new or not
	// it should have had its status counts etc. cleared
	// prior to people changing place.
	current->addPerson(this);
}

std::ostream& operator<<(std::ostream& os, const Person& person) {
	const std::string& sid =
			person._school == 0 ? "null" : person._school->placeId();
	const std::string& hid =
			person._household == 0 ? "null" : person._household->placeId();
	const std::string& wid =
			person._work == 0 ? "null" : person._work->placeId();
	const std::string& gid =
			person._group_quarters == 0 ?
					"null" : person._group_quarters->placeId();
	os << "Person[" << person.person_id << ", home: " << hid << ", work: "
			<< wid << ", school: " << sid << ", gq: " << gid << "]";
	return os;
}

}
/* namespace mrsa */
