/*
 * Person.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

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
				vec[TUCASE_ID_WEEKEND_IDX]), relate(0), sex(0), age(0), weekday_acts(
				0), weekend_acts(0), current_activity(0), hourOfInfection(0) {

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
		age = strToInt(val);
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

void Person::updatePlaceCounters() {
	if (current != 0) {
		if (_status == INFECTED) {
			current->place_infected++;
		} else if (_status == UNCOLONIZED) {
			current->place_uncolonized++;
		} else if (_status == COLONIZED) {
			current->place_colonized++;
		}
	}
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
		changePlace(_household);
	} else {
		const std::string& place_type = current_activity->getPlaceType();
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

void Person::detailedPlaceTransition(float infection_period) {
	double tick = RepastProcess::instance()->getScheduleRunner().currentTick();
	if (_status == INFECTED && (tick - hourOfInfection) <= infection_period) {
		//std::cout << id() << " infected through tick " << tick << std::endl;
		return;
	}

	PersonStatus place_status = UNCOLONIZED;
	if (current != 0) {
		if (current->place_infected > 0)
			place_status = INFECTED;
		else if (current->place_colonized > 0)
			place_status = COLONIZED;
	}

	bool faster_response = (_household != 0 && _household->faster_response);
	int current_risk = 2;
	if (current_activity != 0)
		current_risk = current_activity->getPlaceRisk();
	PersonStatus nextStatus = StatusCalculator::instance()->next(_status,
			place_status, current_risk, faster_response);
	updateStatus(nextStatus);
}

void Person::updateStatus(PersonStatus status) {
	if (_status != INFECTED && status == INFECTED) {
		hourOfInfection =
				RepastProcess::instance()->getScheduleRunner().currentTick();
	}
	_status = status;
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
	if (place != 0 && current != place) {
		current = place;
	}
}

std::ostream& operator<<(std::ostream& os, const Person& person) {
	const std::string& sid =
			person._school == 0 ? "null" : person._school->place_id;
	const std::string& hid =
			person._household == 0 ? "null" : person._household->place_id;
	const std::string& wid =
			person._work == 0 ? "null" : person._work->place_id;
	const std::string& gid =
			person._group_quarters == 0 ?
					"null" : person._group_quarters->place_id;
	os << "Person[" << person.person_id << ", home: " << hid << ", work: "
			<< wid << ", school: " << sid << ", gq: " << gid << "]";
	return os;
}

StatusCalculator* StatusCalculator::instance_ = 0;

StatusCalculator::StatusCalculator(double a, double b, double c, double d,
		double e, double scalingFactor) :
		a_(a), b_(b), c_(c), d_(d), e_(e) {
}

void StatusCalculator::initialize(double a, double b, double c, double d,
		double e, double scalingFactor) {
	instance_ = new StatusCalculator(a, b, c, d, e, scalingFactor);
}

StatusCalculator* StatusCalculator::instance() {
	if (instance_ == 0) {
		throw domain_error(
				"StatusCalculator must initialized before instance() is called");
	}
	return instance_;
}

StatusCalculator::~StatusCalculator() {
}

PersonStatus StatusCalculator::next(PersonStatus start, PersonStatus other,
		double risk, bool faster) {

	PersonStatus ret = UNCOLONIZED;
	if (start == UNCOLONIZED) {
		if (other == UNCOLONIZED)
			ret = chooseOne(1, 0, 0, risk);
		else if (other == COLONIZED)
			ret = chooseOne(1 - a_, a_, 0, risk);
		else if (other == INFECTED)
			ret = chooseOne(1 - 2 * a_, 2 * a_, 0, risk);
	} else if (start == COLONIZED) {
		ret = chooseOne(e_, 1 - b_ - e_, b_, 1);
	} else if (start == INFECTED) {
		if (faster) {
			ret = chooseOne(scaling * d_, scaling * c_, 1 - scaling * (c_ + d_),
					1);
		} else {
			ret = chooseOne(d_, c_, 1 - c_ - d_, 1);
		}
	}

	return ret;

}

PersonStatus StatusCalculator::chooseOne(double p1, double p2, double p3,
		double risk) {
	// Draw a new random threshold.
	double draw = Random::instance()->nextDouble();
	PersonStatus ret = UNCOLONIZED;

	// Select the next state.
	// Check for an infected state draw.
	if (draw < (risk * p3)) {
		ret = INFECTED;
	} else if (draw < (risk * (p2 + p3))) {
		ret = COLONIZED;
	}

	return ret;
}

}
/* namespace mrsa */
