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

/**
 * A household place in the mrsa model. The risk is set
 * to 2.0 and each uncolonized person currently in the household
 * undergoes the transmission algorithm.
 */
class Household : public mrsa::AbstractPlace {

public:
	Household(std::vector<std::string>& vec);
	virtual ~Household();

	/**
	 * Runs the household transmission algorithm.
	 */
	virtual void runTransmission();

	/**
	 * Adds the specified Person as a member of this Household.
	 */
	void addMember(Person* person);

	/**
	 * Initialize seek and destroy behavior with the
	 * specified person as the source infectee.
	 */
	void initSeekAndDestroy(Person* person);

private:
	std::vector<Person*> members;
	// first person infected in a house of
	// uninfected members
	Person* source_infectee;
	double sd_timestamp;

	void seekAndDestroy();

};

} /* namespace mrsa */
#endif /* HOUSEHOLD_H_ */
