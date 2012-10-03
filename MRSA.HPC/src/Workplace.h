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
 * Implements the Workplace place. This does randomly
 * selected transmission on 4 persons, using a risk of 0.1
 */
class Workplace: public SelectiveTransmissionPlace {

public:
	Workplace(std::vector<std::string>& vec, Risk risk);
	virtual ~Workplace();
};

} /* namespace mrsa */
#endif /* WORKPLACE_H_ */
