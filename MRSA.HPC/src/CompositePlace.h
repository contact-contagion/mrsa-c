/*
 * CompositePlace.h
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#ifndef COMPOSITEPLACE_H_
#define COMPOSITEPLACE_H_

#include "Place.h"
#include "AbstractPlace.h"

namespace mrsa {

/**
 * A group of persons of the same age in a school. Transmission
 * occurs between members of an AgeGroup.
 */
class ComponentPlace : public AbstractPlace {

public:
	ComponentPlace(std::string id, std::string type, Risk risk);
	virtual ~ComponentPlace();

	/**
	 * Runs the transmission algorithm on the members of this age group.
	 */
	virtual void runTransmission();

	/**
	 * Gets the number of persons in this ComponentPlace.
	 */
	size_t size();

private:
	ComponentPlace(const ComponentPlace& other);
};



/**
 * A CompositePlace contains sub component places in which
 * the transmission actually occurs.
 */
class CompositePlace: public Place {

public:
	CompositePlace(std::vector<std::string>& vec, Risk risk, unsigned int component_max_size);
	virtual ~CompositePlace();

	/**
	 * Adds a person to a component place in this CompositePlace.
	 */
	int addPersonToComponent(Person* person, int activity_type, int compartment_index);

	/**
	 * Runs the mrsa transmission algorithm.
	 */
	void runTransmission();

	/**
	 * Resets this CompositePlace for the next iteration of the model.
	 */
	void reset();

	/**
	 * Gets the number of component places in this CompositePlace.
	 */
	size_t componentCount() {
		return components.size();
	}

	/**
	 * Gets the number of persons in the component place at the specified index.
	 *
	 * @return the number of persons in the component place at the specified index.
	 */
	size_t componentSize(size_t index);




private:
	std::vector<ComponentPlace*> components;
	unsigned int component_max_size_;
};

} /* namespace mrsa */
#endif /* COMPOSITEPLACE_H_ */
