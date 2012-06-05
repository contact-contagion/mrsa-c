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
 * MRSA model. Disease transmission is dependent on place, and
 * so the runTransmission virtual method should be implemented based
 * on the semantics of the Place sub type.
 */
class Place {

	friend std::ostream& operator<<(std::ostream& os, const Place& place);

public:

	/**
	 * Creates the place based on the values in the vector, and with the specified risk.
	 */
	Place(std::vector<std::string>& vec, float risk);

	/**
	 * Creates a place with just a risk and an id.
	 */
	Place(std::string id, std::string type, float risk_);

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
	 * Resets this place for the next iteration of the model.
	 */
	virtual void reset() = 0;

	/**
	 * Gets the place id of this place.
	 */
	const std::string& placeId() const {
		return id_;
	}

	/**
	 * Gets the type of this place (Household etc.)
	 */
	const std::string& placeType() const {
		return type_;
	}

protected:

	// id and type of place
	std::string id_, type_;
	// place location
	double longitude, latitude;
	float risk_;
};

std::ostream& operator<<(std::ostream& os, const Place& place);

} /* namespace mrsa */
#endif /* PLACE_H_ */
