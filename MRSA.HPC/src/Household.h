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

};

} /* namespace mrsa */
#endif /* HOUSEHOLD_H_ */
