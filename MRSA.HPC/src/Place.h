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

struct Risk {

	// a risk multiplier for activity type 0 and activity type 1
	float a0_, a1_;
	float b0_, b1_;
	float x0_, x1_;

	Risk() : a0_(1), a1_(1),
			b0_(1), b1_(1), x0_(1), x1_(1) {}


	Risk(float a0, float a1, float b0, float b1, float x0, float x1) : a0_(a0), a1_(a1),
			b0_(b0), b1_(b1), x0_(x0), x1_(x1) {}

	Risk(const Risk& r) : a0_(r.a0_), a1_(r.a1_),
			b0_(r.b0_), b1_(r.b1_), x0_(r.x0_), x1_(r.x1_) {}

};


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
	Place(std::vector<std::string>& vec, Risk risk);

	/**
	 * Creates a place with just a risk and an id.
	 */
	Place(std::string id, std::string type, Risk risk);

	virtual ~Place();

	/**
	 * Adds a person to this place.
	 *
	 * @param person the person to add.
	 */
	virtual void addPerson(Person* person, int activity_type) = 0;

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
	Risk risk_;
};

std::ostream& operator<<(std::ostream& os, const Place& place);

} /* namespace mrsa */
#endif /* PLACE_H_ */
