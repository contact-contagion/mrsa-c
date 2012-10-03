/*
 * Places.h
 *
 *  Created on: Oct 3, 2012
 *      Author: nick
 */

#ifndef PLACES_H_
#define PLACES_H_

#include "Place.h"

namespace mrsa {

struct Places {

	Place* household, *group_quarters, *work, *school, *current,
	*daycare, *gym, *hospital;
	std::vector<Place*> other_households;

	Places() : household(0), group_quarters(0), work(0), school(0), current(0),
			daycare(0), gym(0), hospital(0), other_households() {}

	Places(const Places& p) : household(p.household), group_quarters(p.group_quarters), work(p.work),
			school(p.school), current(p.current),
			daycare(p.daycare), gym(p.gym), hospital(p.hospital), other_households(p.other_households) {}
};

}




#endif /* PLACES_H_ */
