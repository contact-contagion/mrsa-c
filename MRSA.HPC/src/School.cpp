/*
 * School.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "School.h"
#include "DiseaseStatus.h"

namespace mrsa {

using namespace std;

AgeGroup::AgeGroup() :
		infected(0), colonized(0), uncolonized(0), persons() {

}

AgeGroup::~AgeGroup() {
}

void AgeGroup::addPerson(Person* person) {
	// add the person to the vector of Persons.
	persons.push_back(person);
	// increment the disease status counts based
	// on the person's status.
	DiseaseStatus status = person->status();
	if (status == UNCOLONIZED)
		++uncolonized;
	else if (status == COLONIZED)
		++colonized;
	else if (status == INFECTED)
		++infected;
}

void AgeGroup::processPeople(TransmissionAlgorithm* ta, float risk) {
	// run the transmission algorithm for each person, using the
	// status counts for this AgeGroup.
	for (vector<Person*>::iterator iter = persons.begin(); iter != persons.end(); ++iter) {
		Person* person = (*iter);
		if (person->canStatusChange()) {
			DiseaseStatus status = person->status();
			person->updateStatus(ta->run(infected, colonized, uncolonized, status, risk));
		}
	}
}

void AgeGroup::reset() {
	// clear the vector of persons and set the counts to 0
	persons.clear();
	infected = colonized = uncolonized = 0;
}

// low risk = 1f
School::School(std::vector<std::string>& vec) :
		Place(vec, 1.0f), person_map() {
}

School::~School() {
	// delete the AgeGroups.
	for (AgeGroupMapIter iter = person_map.begin(); iter != person_map.end(); ++iter) {
		delete iter->second;
	}

}

// adds the person to the age group for that person.
void School::addPerson(Person* person) {

	int age = person->age();
	// find the AgeGroup for age.
	AgeGroupMapIter iter = person_map.find(age);
	if (iter == person_map.end()) {
		// create a new AgeGroup and
		// add the person to it.
		AgeGroup* grp = new AgeGroup();
		grp->addPerson(person);
		// put the AgeGroup in the map.
		person_map.insert(pair<int, AgeGroup*>(age, grp));
	} else {
		// add the person to found AgeGroup.s
		iter->second->addPerson(person);
	}
}

void School::runTransmission() {
	// run the transmission algorithm for all the AgeGroups.
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	for (AgeGroupMapIter iter = person_map.begin(); iter != person_map.end(); ++iter) {
		iter->second->processPeople(ta, risk_);
	}
}

void School::reset() {
	// reset all the age groups.
	for (AgeGroupMapIter iter = person_map.begin(); iter != person_map.end(); ++iter) {
		iter->second->reset();
	}
}

} /* namespace mrsa */
