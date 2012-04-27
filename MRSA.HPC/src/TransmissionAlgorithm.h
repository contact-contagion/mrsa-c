/*
 * TransmissionAlgorithm.h
 *
 *  Created on: Apr 26, 2012
 *      Author: nick
 */

#ifndef TRANSMISSIONALGORITHM_H_
#define TRANSMISSIONALGORITHM_H_


#include "PersonStatus.h"

namespace mrsa {

/**
 * Implements the algorithm for transitioning
 * from one disease state to another.
 */
class TransmissionAlgorithm {

public:
	TransmissionAlgorithm(double a, double b, double c, double d, double e);
	virtual ~TransmissionAlgorithm();

	static void initialize(double a, double b, double c, double d, double e);
	static TransmissionAlgorithm* instance();

	/**
	 * Runs the transmission algorithm given the current status and the
	 * infected, colonized and uncolonized counts.
	 *
	 * @param infected
	 * @param colonized
	 * @param uncolonized
	 * @param currentStatus
	 */
	PersonStatus run(unsigned int infected, unsigned int colonized, unsigned int uncolonized,
			PersonStatus currentStatus, float risk);

private:
	static TransmissionAlgorithm* instance_;
	double a_, b_, c_, d_, e_;

	PersonStatus runUncolonized(float risk, unsigned int infected, unsigned int colonized, unsigned int uncolonized);
	PersonStatus runColonized();
	PersonStatus runInfected();
};


} /* namespace mrsa */
#endif /* TRANSMISSIONALGORITHM_H_ */


