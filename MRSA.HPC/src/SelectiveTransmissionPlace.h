/*
 * SelectiveTransmissionPlace.h
 *
 *  Created on: Apr 27, 2012
 *      Author: nick
 */

#ifndef SELECTIVETRANSMISSIONPLACE_H_
#define SELECTIVETRANSMISSIONPLACE_H_

#include "AbstractPlace.h"

namespace mrsa {

/**
 * Place implementation that will run the transmision algorithm
 * on only a specified number of randomly selected uncolonized persons, rather
 * than all uncolonized persons.  The transmision state change for non-uncolonized
 * persons will happen for all of the non-uncolonized persons in this place
 */
class SelectiveTransmissionPlace: public AbstractPlace {

public:
	/**
	 * Creates a SelectiveTransmissionPlace from the specified vector of parameters with
	 * the specified risk and count of uncolonized persons to the algorithm on.
	 */
	SelectiveTransmissionPlace(std::vector<std::string>& vec, float risk, int uncp_count);
	virtual ~SelectiveTransmissionPlace();


	/**
	 * Runs the SelectiveTransmissionPlace transmission algorithm.
	 */
	virtual void runTransmission();


private:
	unsigned int uncp_count;
};

} /* namespace mrsa */
#endif /* SELECTIVETRANSMISSIONPLACE_H_ */
