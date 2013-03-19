/*
 * School.h
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#ifndef SCHOOL_H_
#define SCHOOL_H_

#include "Person.h"
#include "CompositePlace.h"

namespace mrsa {

/**
 * A School place. Transmission in a School is via
 * persons in the same grade (i.e. of the same age here),
 * with a risk of 1.0.
 */
class School: public CompositePlace {
public:
	School(std::vector<std::string>& vec, Risk risk);

	virtual ~School();

};

} /* namespace mrsa */
#endif /* SCHOOL_H_ */
