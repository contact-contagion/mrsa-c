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
	if (instance_ != 0) {
		delete instance_;
	}
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

PersonStatus TransmissionAlgorithm::run(unsigned int infected, unsigned int colonized,
		unsigned int uncolonized, PersonStatus currentStatus, float risk) {
	if (currentStatus == UNCOLONIZED) {
		return runUncolonized(risk, infected, colonized, uncolonized);
	} else if (currentStatus == COLONIZED) {
		return runColonized();
	} else if (currentStatus == INFECTED) {
		return runInfected();
	}

	return currentStatus;
}

PersonStatus TransmissionAlgorithm::runUncolonized(float risk, unsigned int infected,
		unsigned int colonized, unsigned int uncolonized) {

	PersonStatus ret(UNCOLONIZED);
	if (infected > 0) {
		double draw = repast::Random::instance()->nextDouble();
		ret = draw <= (2 * risk * a_) ? COLONIZED : UNCOLONIZED;
	} else if (colonized > 0) {
		double draw = repast::Random::instance()->nextDouble();
		ret = draw <= (risk * a_) ? COLONIZED : UNCOLONIZED;
	}

	return ret;
}

PersonStatus TransmissionAlgorithm::runColonized() {

	PersonStatus ret(COLONIZED);
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= b_)
		ret = INFECTED;
	else if (draw <= e_ + b_)
		ret = UNCOLONIZED;

	return ret;
}

PersonStatus TransmissionAlgorithm::runInfected() {

	PersonStatus ret(INFECTED);
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= d_)
		ret = UNCOLONIZED;
	else if (draw <= d_ + c_)
		ret = COLONIZED;

	return ret;
}

/*
 PersonStatus StatusCalculator::next(PersonStatus start, PersonStatus other,
 double risk, bool faster) {

 PersonStatus ret = UNCOLONIZED;
 if (start == UNCOLONIZED) {
 if (other == UNCOLONIZED)
 ret = chooseOne(1, 0, 0, risk);
 else if (other == COLONIZED)
 ret = chooseOne(1 - a_, a_, 0, risk);
 else if (other == INFECTED)
 ret = chooseOne(1 - 2 * a_, 2 * a_, 0, risk);
 } else if (start == COLONIZED) {
 ret = chooseOne(e_, 1 - b_ - e_, b_, 1);
 } else if (start == INFECTED) {
 if (faster) {
 ret = chooseOne(scaling * d_, scaling * c_, 1 - scaling * (c_ + d_),
 1);
 } else {
 ret = chooseOne(d_, c_, 1 - c_ - d_, 1);
 }
 }

 return ret;
 }

 PersonStatus StatusCalculator::chooseOne(double p1, double p2, double p3,
 double risk) {
 // Draw a new random threshold.
 double draw = Random::instance()->nextDouble();
 PersonStatus ret = UNCOLONIZED;

 // Select the next state.
 // Check for an infected state draw.
 if (draw < (risk * p3)) {
 ret = INFECTED;
 } else if (draw < (risk * (p2 + p3))) {
 ret = COLONIZED;
 }

 return ret;
 }
 */

} /* namespace mrsa */
