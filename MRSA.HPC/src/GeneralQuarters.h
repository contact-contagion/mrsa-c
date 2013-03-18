/*
 * GeneralQuarters.h
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#ifndef GENERALQUARTERS_H_
#define GENERALQUARTERS_H_

#include "CompositePlace.h"

namespace mrsa {

class GeneralQuarters: public CompositePlace {

public:
	GeneralQuarters(std::vector<std::string>& vec, Risk risk);

	virtual ~GeneralQuarters();

	/**
	 * Adds a person to this GeneralQuarters.
	 *
	 * @param person the person to add.
	 */
	void addPerson(Person* person, int activity_type);

};

} /* namespace mrsa */
#endif /* GENERALQUARTERS_H_ */
