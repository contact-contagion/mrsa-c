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
 * Implements the Hospital place. Hospital transmission always
 * has an infected person regardless of whether or not such
 * a person exists in the hospital.
 */
class Hospital: public AbstractPlace {

public:
	Hospital(std::vector<std::string>& vec, Risk risk);
	virtual ~Hospital();

	virtual void runTransmission();

protected:
	/**
	 * Process the specified uncolonized person for transmission. The infected
	 * count in this is always 1.
	 */
	void processUncolonized(Person* person, TransmissionAlgorithm* ta);
};

} /* namespace mrsa */
#endif /* HOSPITAL_H_ */
