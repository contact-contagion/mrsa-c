/*
 * TransmissionAlgorithm.h
 *
 *  Created on: Apr 26, 2012
 *      Author: nick
 */

#ifndef TRANSMISSIONALGORITHM_H_
#define TRANSMISSIONALGORITHM_H_

#include "DiseaseStatus.h"

namespace mrsa {

/**
 * Implements the algorithm for transitioning
 * from one disease state to another. This is implemented
 * as a Singleton that must be explicitly initialized before use.
 */
class TransmissionAlgorithm {

public:
	TransmissionAlgorithm(double a, double b, double c, double d, double e);
	virtual ~TransmissionAlgorithm();

	/**
	 * Initialize the TransmissionAlgorithm singleton with the
	 * a, b, c, d, and e parameters.
	 */
	static void initialize(double a, double b, double c, double d, double e);

	/**
	 * Gets singleton instance. If it has not been initialized, an exception
	 * is thrown.
	 */
	static TransmissionAlgorithm* instance();

	/**
	 * Gets the number of persons who transitioned
	 * from uncolonized to colonized as the result
	 * of colocation with infected person(s).
	 */
	const long fromInfectionCount() const {
		return from_infection;
	}

	/**
	 * Gets the number of persons who transitioned
	 * from uncolonized to colonized as the result
	 * of colocation with colonized person(s).
	 */
	const long fromColonizationCount() const {
		return from_colonization;
	}

	/**
	 * Runs the transmission algorithm given the current status and the
	 * infected, colonized and uncolonized counts.
	 *
	 * @param infected
	 * @param colonized
	 * @param uncolonized
	 * @param currentStatus
	 */
	DiseaseStatus run(unsigned int infected, unsigned int colonized, DiseaseStatus currentStatus,
			float risk);

private:
	static TransmissionAlgorithm* instance_;
	double a_, b_, c_, d_, e_;
	// counts the number of persons who were
	// colonized from contact with infected persons,
	// and from colonized persons.
	long from_infection, from_colonization;

	/**
	 * Run the transmission algorithm for an uncolonized person, returning the result as a
	 * DiseaseStatus.
	 */
	DiseaseStatus runUncolonized(float risk, unsigned int infected, unsigned int colonized);
	/**
	 * Run the transmission algorithm for a colonized person, returning the result as a
	 * DiseaseStatus.
	 */
	DiseaseStatus runColonized();

	/**
	 * Run the transmission algorithm for an infected person, returning the result as a
	 * DiseaseStatus.
	 */
	DiseaseStatus runInfected();
};

} /* namespace mrsa */
#endif /* TRANSMISSIONALGORITHM_H_ */

