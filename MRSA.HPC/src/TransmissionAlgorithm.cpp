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

//int attempts = 0;

TransmissionAlgorithm::TransmissionAlgorithm(double a, double b, double c, double d, double e) :
		a_(a), b_(b), c_(c), d_(d), e_(e),  newly_colonized(0), newly_infected(0),
		colonized_from_infection(0), colonized_from_colonization(0),
		colonized_per_infected(0), colonized_per_colonization(0) {
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

void TransmissionAlgorithm::resetCounts() {
	//std::cout << "" << attempts << "," << newly_infected << std::endl;
	//attempts = 0;
	newly_infected = newly_colonized = 0;
	colonized_from_infection = colonized_from_colonization = 0;
	colonized_per_infected = colonized_per_colonization = 0;
}

TransmissionAlgorithm* TransmissionAlgorithm::instance() {
	if (instance_ == 0) {
		throw std::domain_error(
				"TransmissionAlgorithm must be initialized before instance() is called");
	}
	return instance_;
}

// the algorithm for uncolonized persons.
DiseaseStatus TransmissionAlgorithm::runUncolonized(float risk, unsigned int infected,
		unsigned int colonized) {

	// default return value.
	DiseaseStatus ret(UNCOLONIZED);
	if (infected > 0) {
		// set the return value based on the results of a random draw
		// vs. the risk and a_ * 2
		double draw = repast::Random::instance()->nextDouble();
		ret = draw <= (2 * risk * a_) ? COLONIZED : UNCOLONIZED;
		if (ret == COLONIZED) {
			++newly_colonized;
			++colonized_from_infection;
		}
	} else if (colonized > 0) {
		// set the return value based on the results of a random draw
		// vs. the risk and a_
		double draw = repast::Random::instance()->nextDouble();
		ret = draw <= (risk * a_) ? COLONIZED : UNCOLONIZED;
		if (ret == COLONIZED) {
			++newly_colonized;
			++colonized_from_colonization;
		}
	}

	//std::cout << "fc: " << from_colonization << std::endl;
	//std::cout << "fi: " << from_infection << std::endl;

	return ret;
}

// the algorithm for colonized persons.
DiseaseStatus TransmissionAlgorithm::runColonized() {
	DiseaseStatus ret(COLONIZED);
	//++attempts;
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= b_) {
		// move from colonized to infected with a probability of b_
		ret = INFECTED;
		++newly_infected;
	} else if (draw <= e_ + b_)
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
	else if (draw <= d_ + c_) {
		// move from infected to uncolonized with a probability of c_
		// we already tested for <= d_ so only get here if > d_ but
		// <= c_.
		ret = COLONIZED;
		++newly_colonized;
	}

	return ret;
}

} /* namespace mrsa */
