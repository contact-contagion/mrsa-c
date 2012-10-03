/*
 * School.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "School.h"
#include "DiseaseStatus.h"
#include "Constants.h"
#include "Statistics.h"

namespace mrsa {

using namespace std;

AgeGroup::AgeGroup(std::string id, std::string type, Risk risk) :
		AbstractPlace(id, type, risk) {

}

AgeGroup::~AgeGroup() {
}

void AgeGroup::runTransmission() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	// uncolonized must always go first so that the infected and colonized persons
	// if they cause a colonization of the uncolonized are still in that infected or
	// colonized state.
	for (PersonIter iter = uncolonized.begin(); iter != uncolonized.end(); ++iter) {
		processUncolonized(*iter, ta);
	}
	for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
		processInfected(*iter, ta);
	}

	for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
		processColonized(*iter, ta);
	}

}

// low risk = 1f
School::School(std::vector<std::string>& vec, Risk risk) :
		Place(vec,  risk), person_map() {
}

School::~School() {
	// delete the AgeGroups.
	for (AgeGroupMapIter iter = person_map.begin(); iter != person_map.end(); ++iter) {
		delete iter->second;
	}
}

// adds the person to the age group for that person.
void School::addPerson(Person* person, int activity_type) {

	int age = person->age();
	// find the AgeGroup for age.
	AgeGroupMapIter iter = person_map.find(age);
	if (iter == person_map.end()) {
		// create a new AgeGroup and
		// add the person to it.
		AgeGroup* grp = new AgeGroup(id_, type_, risk_);
		grp->addPerson(person, activity_type);
		// put the AgeGroup in the map.
		person_map.insert(pair<int, AgeGroup*>(age, grp));
	} else {
		// add the person to found AgeGroup.s
		iter->second->addPerson(person, activity_type);
	}
}

void School::runTransmission() {
	// run the transmission algorithm for all the AgeGroups.
	for (AgeGroupMapIter iter = person_map.begin(); iter != person_map.end(); ++iter) {
		iter->second->runTransmission();
	}
}

void School::reset() {
	// reset all the age groups.
	for (AgeGroupMapIter iter = person_map.begin(); iter != person_map.end(); ++iter) {
		iter->second->reset();
	}
}

} /* namespace mrsa */
