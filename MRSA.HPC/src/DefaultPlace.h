/*
 * DefaultPlace.h
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#ifndef DEFAULTPLACE_H_
#define DEFAULTPLACE_H_

#include "SelectiveTransmissionPlace.h"

namespace mrsa {

/**
 * Default place implementation for those places that do not
 * have a specific implementation. The risk level is set to
 * 1.0. 4 uncolonized persons in this place will be randomly selected
 * to be processed for transmission.
 */
class DefaultPlace : public SelectiveTransmissionPlace {

public:
	/**
	 * Creates this DefaultPlace from the data in the vector.
	 */
	DefaultPlace(std::vector<std::string>& vec, Risk risk);
	virtual ~DefaultPlace();
};

} /* namespace mrsa */
#endif /* DEFAULTPLACE_H_ */
