/*
 * Household.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */
#include <algorithm>

#include "Household.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

// 14 x 24
const int FOURTEEN_DAYS = 336;
const double CURE_PROB = 0.83;

// sets the risk 2.0
Household::Household(std::vector<std::string>& vec) :
		AbstractPlace(vec, 2.0f), members(0), source_infectee(0), sd_timestamp(0) {
}

Household::~Household() {
}

void Household::addMember(Person* person) {
	members.push_back(person);
}

void Household::runTransmission() {
	// run the TA for each person.
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	// uncolonized must always go first so that the infected and colonized persons
	// if they cause a colonization of the uncolonized are still in that infected or
	// colonized state.
	for (PersonIter iter = uncolonized.begin(); iter != uncolonized.end(); ++iter) {
		processUncolonized(*iter, ta);
	}

	for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
		processInfected(*iter, ta);
	}

	for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
		processColonized(*iter, ta);
	}

	if (source_infectee != 0
			&& repast::RepastProcess::instance()->getScheduleRunner().currentTick() - sd_timestamp >= FOURTEEN_DAYS) {
		seekAndDestroy();
	}
}

void Household::seekAndDestroy() {
	std::cout << placeId() << " seek and destroy " << std::endl;
	std::random_shuffle(members.begin(), members.end(), repast::uni_random);
	for (PersonIter iter = members.begin(); iter != members.end(); ++iter) {
		Person* p = *iter;
		if (p->status() != UNCOLONIZED && p != source_infectee && repast::Random::instance()->nextDouble() <= CURE_PROB) {
			p->updateStatus(UNCOLONIZED);
		}
	}
	sd_timestamp = 0;
	source_infectee = 0;
}

void Household::initSeekAndDestroy(Person* person) {
	if (source_infectee == 0) {
		source_infectee = person;
		sd_timestamp = repast::RepastProcess::instance()->getScheduleRunner().currentTick();
	}
}

} /* namespace mrsa */
