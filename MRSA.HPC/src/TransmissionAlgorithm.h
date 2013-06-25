/*
*MRSA Model
*
*Copyright (c) 2013 University of Chicago and Argonne National Laboratory
*   All rights reserved.
*  
*   Redistribution and use in source and binary forms, with 
*   or without modification, are permitted provided that the following 
*   conditions are met:
*  
*  	 Redistributions of source code must retain the above copyright notice,
*  	 this list of conditions and the following disclaimer.
*  
*  	 Redistributions in binary form must reproduce the above copyright notice,
*  	 this list of conditions and the following disclaimer in the documentation
*  	 and/or other materials provided with the distribution.
*  
*  	 Neither the name of the Argonne National Laboratory nor the names of its
*     contributors may be used to endorse or promote products derived from
*     this software without specific prior written permission.
*  
*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
*   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
*   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
*   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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

struct TAParameters {
	double a, b, e, alpha, beta, gamma, rho;
};


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
	static void initialize(TAParameters& params);

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
	DiseaseStatus runColonized(float b_risk_multiplier, float e_risk_multiplier);

	/**
	 * Run the transmission algorithm for an initially infected person.
	 * This determines whether or not they seek care.
	 */
	InfectionStatus runInfected();

	/**
	 * Runs the transmission algorithm for an infected person
	 * who is under going "self care".
	 */
	DiseaseStatus runInfectedSelfCare();

	/**
	 * Runs the transmission algorithm for an infected person
	 * who is seeking care.
	 */
	DiseaseStatus runInfectedSeekCare();

	/**
	 * Resets the transmission counts.
	 */
	void resetCounts();

private:

	static TransmissionAlgorithm* instance_;
	double a_, b_, e_, alpha_, beta_, gamma_, rho_;

	// counts the number of persons who were
	// are newly_colonized and newly_infected
	long newly_colonized, newly_infected;
	long colonized_from_infection, colonized_from_colonization;
	double colonized_per_infected, colonized_per_colonization;

	/**
	 * Private constructor.
	 */
	TransmissionAlgorithm(TAParameters& params);

};

} /* namespace mrsa */
#endif /* TRANSMISSIONALGORITHM_H_ */

