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
 * have a specific implementation.
 *
 */
class DefaultPlace : public SelectiveTransmissionPlace {

public:
	DefaultPlace(std::vector<std::string>& vec);
	virtual ~DefaultPlace();
};

} /* namespace mrsa */
#endif /* DEFAULTPLACE_H_ */
