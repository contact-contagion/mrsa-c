/*
 * Household.h
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#ifndef HOUSEHOLD_H_
#define HOUSEHOLD_H_

#include "AbstractPlace.h"

namespace mrsa {

class Household : public mrsa::AbstractPlace {

public:
	Household(std::vector<std::string>& vec);
	virtual ~Household();

	/**
	 * Adds a person to this household place.
	 *
	 * @param person the person to add
	 */
	virtual void addPerson(Person* person);

	/**
	 * Runs the household transmission algorithm.
	 */
	virtual void runTransmission();

	/**
	 * Resets this household to an unoccupied state.
	 */
	virtual void reset();

private:
	// vector of persons currently in this place
	std::vector<Person*> persons;
};

} /* namespace mrsa */
#endif /* HOUSEHOLD_H_ */
