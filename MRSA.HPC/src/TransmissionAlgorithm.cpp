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

TransmissionAlgorithm::TransmissionAlgorithm(TAParameters& params) :
		a_(params.a), b_(params.b), e_(params.e), alpha_(params.alpha), beta_(params.beta),
		gamma_(params.gamma), rho_(params.rho), newly_colonized(0), newly_infected(0),
		colonized_from_infection(0), colonized_from_colonization(0),
		colonized_per_infected(0), colonized_per_colonization(0) {
}

void TransmissionAlgorithm::initialize(TAParameters& params) {
	// if the instance exists, delete the old one.
	if (instance_ != 0) {
		delete instance_;
	}
	// create the one.
	instance_ = new TransmissionAlgorithm(params);
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
DiseaseStatus TransmissionAlgorithm::runUncolonized(float risk_multiplier, unsigned int infected,
		unsigned int colonized) {

	// default return value.
	DiseaseStatus ret(UNCOLONIZED);
	if (infected > 0) {
		// set the return value based on the results of a random draw
		// vs. the risk and a_ * 2
		double draw = repast::Random::instance()->nextDouble();
		ret = draw <= (2 * risk_multiplier * a_) ? COLONIZED : UNCOLONIZED;
		if (ret == COLONIZED) {
			++newly_colonized;
			++colonized_from_infection;
		}
	} else if (colonized > 0) {
		// set the return value based on the results of a random draw
		// vs. the risk and a_
		double draw = repast::Random::instance()->nextDouble();
		ret = draw <= (risk_multiplier * a_) ? COLONIZED : UNCOLONIZED;
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
DiseaseStatus TransmissionAlgorithm::runColonized(float risk_multiplier) {
	DiseaseStatus ret(COLONIZED);
	//++attempts;
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= (b_ * risk_multiplier)) {
		// move from colonized to infected with a probability of b_
		ret = INFECTED;
		++newly_infected;
	} else if (draw <= e_ + (b_ * risk_multiplier))
		// move from colonized to infected with a probability of e_
		// we already tested for <= b_ so only get here if > b_ but
		// <= e_.
		ret = UNCOLONIZED;

	return ret;
}

InfectionStatus TransmissionAlgorithm::runInfected() {
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= alpha_) return SEEK_CARE;
	return SELF_CARE;
}

DiseaseStatus TransmissionAlgorithm::runInfectedSelfCare() {
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= rho_) return UNCOLONIZED;
	return COLONIZED;
}

DiseaseStatus TransmissionAlgorithm::runInfectedSeekCare() {
	double draw = repast::Random::instance()->nextDouble();
	if (draw <= beta_) return UNCOLONIZED;
	return COLONIZED;
}


} /* namespace mrsa */
