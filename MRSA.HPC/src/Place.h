/*
 * Place.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef PLACE_H_
#define PLACE_H_


#include <vector>
#include <string>
#include <ostream>

namespace mrsa {

// forward declaration
class Person;

/**
 * Base class that encapsulates a Place (school, home, etc.) in the
 * MRSA model.
 */
class Place {

	friend std::ostream& operator<<(std::ostream& os, const Place& place);

public:

	Place(std::vector<std::string>& vec, float risk);
	virtual ~Place();

	/**
	 * Adds a person to this place.
	 *
	 * @param person the person to add.
	 */
	virtual void addPerson(Person* person) = 0;

	/**
	 * Runs the transmission algorithm appropriate to this place.
	 */
	virtual void runTransmission() = 0;


	/**
	 * Resets this place for the next iteration of the model. Typically
	 * this will set the counter to 0 and remove all the persons
	 * from the this place.
	 */
	virtual void reset() = 0;

	const std::string& placeId() const {
		return id;
	}

	const std::string& placeType() const {
		return type;
	}

protected:

	// id and type of place
	std::string id, type;
	// place location
	double longitude, latitude;
	float risk_;
};

std::ostream& operator<<(std::ostream& os, const Place& place);


} /* namespace mrsa */
#endif /* PLACE_H_ */
