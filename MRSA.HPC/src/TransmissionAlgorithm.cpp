/*
 * TransmissionAlgorithm.cpp
 *
 *  Created on: Apr 26, 2012
 *      Author: nick
 */

#include "repast_hpc/Random.h"

#include "TransmissionAlgorithm.h"

namespace mrsa {

TransmissionAlgorithm* TransmissionAlgorithm::instance_ = 0;

TransmissionAlgorithm::TransmissionAlgorithm(double a, double b, double c, double d, double e) :
		a_(a), b_(b), c_(c), d_(d), e_(e) {
}

void TransmissionAlgorithm::initialize(double a, double b, double c, double d, double e) {
	// if the instance exists, delete the old one.
	if (instance_ != 0) {
		delete instance_;
	}
	// create the one.
	instance_ = new TransmissionAlgorithm(a, b, c, d, e);
}

TransmissionAlgorithm::~TransmissionAlgorithm() {
}

TransmissionAlgorithm* TransmissionAlgorithm::instance() {
	if (instance_ == 0) {
		throw std::domain_error(
				"TransmissionAlgorithm must be initialized before instance() is called");
	}
	return instance_;
}

// run the algorithm
DiseaseStatus TransmissionAlgorithm::run(unsigned int infected, unsigned int colonized,
		unsigned int uncolonized, DiseaseStatus currentStatus, float risk) {
	// depending on the status, run the algorithm for uncolonized, colonized or infected persons.
	if (currentStatus == UNCOLONIZED) {
		return runUncolonized(risk, infected, colonized, uncolonized);
	} else if (currentStatus == COLONIZED) {
		return runColonized();
	} else if (currentStatus == INFECTED) {
		return runInfected();
	}

	return currentStatus;
}

// the algorithm for uncolonized persons.
DiseaseStatus TransmissionAlgorithm::runUncolonized(float risk, unsigned int infected,
		unsigned int colonized, unsigned int uncolonized) {

	// default return value.
	DiseaseStatus ret(UNCOLONIZED);
	if (infected > 0) {
		// set the return value based on the results of a random draw
		// vs. the risk and a_ * 2
		double draw = repast::Random::instance()->nextDouble();
		ret = draw <= (2 * risk * a_) ? COLONIZED : UNCOLONIZED;
	} else if (colonized > 0) {
		// set the return value based on the results of a random draw
		// vs. the risk and a_
		double draw = repast::Random::instance()->nextDouble();
		ret = draw <= (risk * a_) ? COLONIZED : UNCOLONIZED;
	}

	return ret;
}

// the algorithm for colonized persons.
DiseaseStatus TransmissionAlgorithm::runColonized() {
	DiseaseStatus ret(COLONIZED);
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= b_)
		// move from colonized to infected with a probability of b_
		ret = INFECTED;
	else if (draw <= e_ + b_)
		// move from colonized to infected with a probability of e_
		// we already tested for <= b_ so only get here if > b_ but
		// <= e_.
		ret = UNCOLONIZED;

	return ret;
}

DiseaseStatus TransmissionAlgorithm::runInfected() {

	DiseaseStatus ret(INFECTED);
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= d_)
		// move from infected to uncolonized with a probability of d_
		ret = UNCOLONIZED;
	else if (draw <= d_ + c_)
		// move from infected to uncolonized with a probability of c_
		// we already tested for <= d_ so only get here if > d_ but
		// <= c_.
		ret = COLONIZED;

	return ret;
}

} /* namespace mrsa */
