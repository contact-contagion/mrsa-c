/*
 * Gym.h
 *
 *  Created on: Mar 11, 2013
 *      Author: nick
 */

#ifndef GYM_H_
#define GYM_H_

#include "CompositePlace.h"

namespace mrsa {

class Gym : public CompositePlace {
public:
	Gym(std::vector<std::string>& vec, Risk risk);
	virtual ~Gym();
};

} /* namespace mrsa */
#endif /* GYM_H_ */
