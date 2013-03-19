/*
 * GeneralQuarters.h
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#ifndef PRISON_H_
#define PRISON_H_

#include "CompositePlace.h"

namespace mrsa {

class Prison: public CompositePlace {

public:
	Prison(std::vector<std::string>& vec, Risk risk);
	virtual ~Prison();

};

} /* namespace mrsa */
#endif /* PRISON_H_ */
