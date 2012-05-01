/*
 * AbstractPlace.h
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#ifndef ABSTRACTPLACE_H_
#define ABSTRACTPLACE_H_

#include "Place.h"
#include "Person.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

/**
 * Abstract base implementation of a Place that
 * accumulates the disease status counts for Persons
 * in this place using the addPerson method.
 */
class AbstractPlace: public Place {

public:
	AbstractPlace(std::vector<std::string>& vec, float risk);
	virtual ~AbstractPlace();

	/**
	 * Adds a person to this place.
	 *
	 * @param person the person to add.
	 */
	virtual void addPerson(Person* person);

	/**
	 * Runs the transmission algorithm appropriate to this place.
	 */
	virtual void runTransmission() = 0;

	/**
	 * Resets this place for the next iteration of the model. Typically
	 * this will set the counter to 0 and remove all the persons
	 * from the this place.
	 */
	virtual void reset();

protected:
	typedef std::vector<Person*>::iterator PersonIter;
	// number of colonized, uncolonized, and infected persons in this
	// place.
	int colonized, uncolonized, infected;

	/**
	 * Process the specifed person for transmission.
	 */
	void processPerson(Person* person, TransmissionAlgorithm* ta);
};

} /* namespace mrsa */
#endif /* ABSTRACTPLACE_H_ */
