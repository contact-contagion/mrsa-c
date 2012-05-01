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

// adds the person to this place
void SelectiveTransmissionPlace::addPerson(Person* person) {
	// increments the disease status counts
	AbstractPlace::addPerson(person);
	// if uncolonized add to the uncolonized list
	// otherwise add to the "other persons" list.
	if (person->status() == UNCOLONIZED)
		unc_persons.push_back(person);
	else
		other_persons.push_back(person);
}

// runs the transmission algorithm on the persons in this place.
void SelectiveTransmissionPlace::runTransmission() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	// run the algorithm on the non-uncolonized persons, so they can transition from
	// colonized to infected etc.
	for (PersonIter iter = other_persons.begin(); iter != other_persons.end(); ++iter) {
		processPerson(*iter, ta);
	}

	// if the number of uncolonized persons is less than the number of persons to randomly
	// select for transmision, then run the transmission algorithm on all of them.
	if (unc_persons.size() <= uncp_count) {
		for (PersonIter iter = unc_persons.begin(); iter != unc_persons.end(); ++iter) {
			processPerson(*iter, ta);
		}

	} else {
		// choose "uncp_count" persons at random and run the transmission algorithm
		// on them.
		// vector of persons already processed. This is likely to be a small number
		// so a std::set would be overkill.
		std::vector<Person*> processed;
		// create a random number generator to generate an int between 0 and the number of persons
		// - 1. This will be used an index into the uncolonized persons vector.
		repast::IntUniformGenerator gen = repast::Random::instance()->createUniIntGenerator(0,
				(int) unc_persons.size() - 1);
		// for "uncp_count" times:
		for (unsigned int i = 0; i < uncp_count; ++i) {
			// get a person at random
			Person* person = unc_persons[(int) gen.next()];
			// see if that person has already been processed, if so then
			// do the draw again, until we get an unprocessed person.
			PersonIter iter = std::find(processed.begin(), processed.end(), person);
			while (iter != processed.end()) {
				person = unc_persons[(int) gen.next()];
				iter = std::find(processed.begin(), processed.end(), person);
			}

			// process (run the transmission algorithm on) the person.
			processPerson(person, ta);
			// add the person to the vector of persons who have been processed.
			processed.push_back(person);
		}
	}
}

void SelectiveTransmissionPlace::reset() {
	// reset the counts
	AbstractPlace::reset();
	// clear the vector of colonized and infected persons
	other_persons.clear();
	// clear the vector of uncolonized persons.
	unc_persons.clear();
}

} /* namespace mrsa */
