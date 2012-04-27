/*
 * School.h
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#ifndef SCHOOL_H_
#define SCHOOL_H_

#include <map>

#include "Person.h"
#include "Place.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

class AgeGroup {

public:
	AgeGroup();
	virtual ~AgeGroup();

	/**
	 * Resets this AgeGroup for another iteration.
	 *
	 */
	void reset();

	/**
	 * Adds a person to this AgeGroup.
	 *
	 * @param person the person to add
	 */
	void addPerson(Person* person);
	/**
	 * Processes all the persons in this age group for transmission.
	 */
	void processPeople(TransmissionAlgorithm* ta, float risk);

private:
	AgeGroup(const AgeGroup& other);
	unsigned int infected, colonized, uncolonized;
	std::vector<Person*> persons;
};


/**
 * A School place.
 */
class School: public Place {
public:
	School(std::vector<std::string>& vec);
	virtual ~School();

	/**
	 * Adds a person to this school.
	 *
	 * @param person the person to add.
	 */
	virtual void addPerson(Person* person);

	/**
	 * Runs school mrsa transmission algorithm.
	 */
	virtual void runTransmission();

	/**
	 * Resets this school for the next iteration of the model.
	 */
	virtual void reset();

private:

	typedef std::map<int, AgeGroup* >::iterator PersonMapIter;
	std::map<int, AgeGroup*> person_map;

};

} /* namespace mrsa */
#endif /* SCHOOL_H_ */
