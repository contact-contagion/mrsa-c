/*
 * Workplace.h
 *
 *  Created on: Apr 26, 2012
 *      Author: nick
 */

#ifndef WORKPLACE_H_
#define WORKPLACE_H_

#include "SelectiveTransmissionPlace.h"

namespace mrsa {

/**
 * Implements the Worksplace place.
 */
class Workplace: public SelectiveTransmissionPlace {

public:
	Workplace(std::vector<std::string>& vec);
	virtual ~Workplace();
};

} /* namespace mrsa */
#endif /* WORKPLACE_H_ */
