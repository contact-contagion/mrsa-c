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
		AbstractPlace(vec, risk), uncp_count(uncolonized_count) {
}

SelectiveTransmissionPlace::~SelectiveTransmissionPlace() {
}

// runs the transmission algorithm on the persons in this place.
void SelectiveTransmissionPlace::runTransmission() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	// uncolonized should always go first so that if colonized or infected
	// cause a colonization they are still in that state when the uncolonized
	// to colonized transmission is attributed them.

	// if the number of uncolonized persons is less than the number of persons to randomly
	// select for transmision, then run the transmission algorithm on all of them.
	if (uncolonized.size() <= uncp_count) {
		for (PersonIter iter = uncolonized.begin(); iter != uncolonized.end(); ++iter) {
			processUncolonized(*iter, ta);
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
				(int) uncolonized.size() - 1);
		// for "uncp_count" times:
		for (unsigned int i = 0; i < uncp_count; ++i) {
			// get a person at random
			Person* person = uncolonized[(int) gen.next()];
			// see if that person has already been processed, if so then
			// do the draw again, until we get an unprocessed person.
			PersonIter iter = std::find(processed.begin(), processed.end(), person);
			while (iter != processed.end()) {
				person = uncolonized[(int) gen.next()];
				iter = std::find(processed.begin(), processed.end(), person);
			}

			// process (run the transmission algorithm on) the person.
			processUncolonized(person, ta);
			// add the person to the vector of persons who have been processed.
			processed.push_back(person);
		}
	}

	// run the algorithm on the non-uncolonized persons, so they can transition from
	// colonized to infected etc.
	for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
		processColonized(*iter, ta);
	}

	for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
		processInfected(*iter, ta);
	}

}

} /* namespace mrsa */
