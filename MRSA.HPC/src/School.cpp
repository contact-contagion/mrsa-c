/*
 * School.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "School.h"
#include "PersonStatus.h"

namespace mrsa {

using namespace std;

AgeGroup::AgeGroup() :
		infected(0), colonized(0), uncolonized(0), persons() {

}

AgeGroup::~AgeGroup() {
}

void AgeGroup::addPerson(Person* person) {
	persons.push_back(person);
	PersonStatus status = person->status();
	if (status == UNCOLONIZED)
		++uncolonized;
	else if (status == COLONIZED)
		++colonized;
	else if (status == INFECTED)
		++infected;
}

void AgeGroup::processPeople(TransmissionAlgorithm* ta, float risk) {
	for (vector<Person*>::iterator iter = persons.begin(); iter != persons.end(); ++iter) {
		Person* person = (*iter);
		PersonStatus status = person->status();
		person->updateStatus(ta->run(infected, colonized, uncolonized, status, risk));
	}
}

void AgeGroup::reset() {
	persons.clear();
	infected = colonized = uncolonized = 0;
}

// low risk = 1f
School::School(std::vector<std::string>& vec) :
		Place(vec, 1.0f), person_map() {
}

School::~School() {
	for (PersonMapIter iter = person_map.begin(); iter != person_map.end(); ++iter) {
		delete iter->second;
	}

}

void School::addPerson(Person* person) {
	int age = person->age();
	PersonMapIter iter = person_map.find(age);
	if (iter == person_map.end()) {
		// create a new AgeGroup
		AgeGroup* grp = new AgeGroup();
		grp->addPerson(person);
		person_map.insert(pair<int, AgeGroup*>(age, grp));

	} else {
		iter->second->addPerson(person);
	}
}

void School::runTransmission() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	for (PersonMapIter iter = person_map.begin(); iter != person_map.end(); ++iter) {
		iter->second->processPeople(ta, risk_);
	}
}

void School::reset() {
	for (PersonMapIter iter = person_map.begin(); iter != person_map.end(); ++iter) {
		iter->second->reset();
	}
}

} /* namespace mrsa */
