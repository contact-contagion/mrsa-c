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

	const long newlyColonizedCount() const {
		return newly_colonized;
	}

	const long newlyInfectedCount() const {
		return newly_infected;
	}

	const long colonizedFromInfectionCount() const {
		return colonized_from_infection;
	}

	const long colonizedFromColonizationCount() const {
		return colonized_from_colonization;
	}

	const double colonizedPerInfectedCount() const {
		return colonized_per_infected;
	}

	const double colonizedPerColonizedCount() const {
		return colonized_per_colonization;
	}

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
	DiseaseStatus runInfected(float seek_care_modifier);

	/**
	 * Resets the transmission counts.
	 */
	void resetCounts();

private:

	static TransmissionAlgorithm* instance_;
	double a_, b_, c_, d_, e_;

	// counts the number of persons who were
	// are newly_colonized and newly_infected
	long newly_colonized, newly_infected;
	long colonized_from_infection, colonized_from_colonization;
	double colonized_per_infected, colonized_per_colonization;

	/**
	 * Private constructor.
	 */
	TransmissionAlgorithm(double a, double b, double c, double d, double e);

};

} /* namespace mrsa */
#endif /* TRANSMISSIONALGORITHM_H_ */

