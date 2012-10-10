/*
 * Hospital.h
 *
 *      Author: nick
 */

#ifndef HOSPITAL_H_
#define HOSPITAL_H_

#include "SelectiveTransmissionPlace.h"

namespace mrsa {

/**
 * Implements the Hospital place. This does randomly
 * selected transmission on 4 persons.
 */
class Hospital: public SelectiveTransmissionPlace {

public:
	Hospital(std::vector<std::string>& vec, Risk risk);
	virtual ~Hospital();
};

} /* namespace mrsa */
#endif /* HOSPITAL_H_ */
