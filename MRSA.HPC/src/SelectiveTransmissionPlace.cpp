/*
 * SelectiveTransmissionPlace.cpp
 *
 *  Created on: Apr 27, 2012
 *      Author: nick
 */

#include "SelectiveTransmissionPlace.h"

namespace mrsa {

SelectiveTransmissionPlace::SelectiveTransmissionPlace(std::vector<std::string>& vec, float risk,
		int uncolonized_count) :
		AbstractPlace(vec, risk), uncp_count(uncolonized_count), unc_persons(), other_persons() {
}

SelectiveTransmissionPlace::~SelectiveTransmissionPlace() {
}

void SelectiveTransmissionPlace::addPerson(Person* person) {
	AbstractPlace::addPerson(person);
	if (person->status() == UNCOLONIZED)
		unc_persons.push_back(person);
	else
		other_persons.push_back(person);
}

void SelectiveTransmissionPlace::runTransmission() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();
	// run the algorithm on the non-uncolonized persons.
	for (PersonIter iter = other_persons.begin(); iter != other_persons.end(); ++iter) {
		processPerson(*iter, ta);
	}

	if (unc_persons.size() <= uncp_count) {
		for (PersonIter iter = unc_persons.begin(); iter != unc_persons.end(); ++iter) {
			processPerson(*iter, ta);
		}

	} else {
		std::vector<Person*> processed;
		repast::IntUniformGenerator gen = repast::Random::instance()->createUniIntGenerator(0,
				(int) unc_persons.size() - 1);
		for (unsigned int i = 0; i < uncp_count; ++i) {
			Person* person = unc_persons[(int) gen.next()];
			PersonIter iter = std::find(processed.begin(), processed.end(), person);
			while (iter != processed.end()) {
				person = unc_persons[(int) gen.next()];
				iter = std::find(processed.begin(), processed.end(), person);
			}

			processPerson(person, ta);
			processed.push_back(person);
		}
	}
}

void SelectiveTransmissionPlace::reset() {
	AbstractPlace::reset();
	other_persons.clear();
	unc_persons.clear();
}

} /* namespace mrsa */
