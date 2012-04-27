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
 * on a specified number of randomly selected uncolonized persons.
 * The transmision state change for non-uncolonized persons will happen
 * for all of those persons.
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
	 * Adds a person to this SelectiveTransmissionPlace
	 *
	 * @param person the person to add
	 */
	virtual void addPerson(Person* person);

	/**
	 * Runs the SelectiveTransmissionPlace transmission algorithm.
	 */
	virtual void runTransmission();

	/**
	 * Resets this SelectiveTransmissionPlace to an unoccupied state.
	 */
	virtual void reset();

private:
	unsigned int uncp_count;
	std::vector<Person*> unc_persons, other_persons;
};

} /* namespace mrsa */
#endif /* SELECTIVETRANSMISSIONPLACE_H_ */
