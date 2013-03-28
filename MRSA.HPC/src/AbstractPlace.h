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
	AbstractPlace(std::vector<std::string>& vec, Risk risk);
	AbstractPlace(std::string id, std::string type, Risk risk, unsigned int zip);
	virtual ~AbstractPlace();

	/**
	 * Adds a person to this place.
	 *
	 * @param person the person to add.
	 */
	virtual void addPerson(Person* person, int activity_type);

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
	// number of colonized, and infected persons in this
	// place.
	int activity_type_;
	std::vector<Person*> infected, colonized, uncolonized;

	/**
	 * Process the specifed infected person for transmission.
	 */
	void processInfected(Person* person, TransmissionAlgorithm* ta);

	/**
	 * Process the specified colonized person for transmission.
	 */
	void processColonized(Person* person, TransmissionAlgorithm* ta);

	/**
	 * Process the specified uncolonized person for transmission.
	 */
	void processUncolonized(Person* person, TransmissionAlgorithm* ta);
};

} /* namespace mrsa */
#endif /* ABSTRACTPLACE_H_ */
