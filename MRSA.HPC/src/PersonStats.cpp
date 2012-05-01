/*
 * PersonStats.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: nick
 */

#include "PersonStats.h"

namespace mrsa {

using namespace repast::relogo;

PersonStats::PersonStats() :
		total_infected(0), total_colonized(0), total_uncolonized(0) {
}

PersonStats::~PersonStats() {
}

void PersonStats::countPerson(Person* person) {
	// get the person's status and increment the appropriate count.
	DiseaseStatus status = person->status();
	if (status == UNCOLONIZED)
		total_uncolonized++;
	else if (status == COLONIZED)
		total_colonized++;
	else if (status == INFECTED)
		total_infected++;
}

InfectionSum::InfectionSum(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

InfectionSum::~InfectionSum() {
}

/**
 * Gets the total number of infected persons using
 * the wrapped PersonStats object.
 */
double InfectionSum::getData() {
	return stats_->totalInfected();
}

UnColonizedSum::UnColonizedSum(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

UnColonizedSum::~UnColonizedSum() {
}

/**
 * Gets the total number of uncolonized persons using
 * the wrapped PersonStats object.
 */
double UnColonizedSum::getData() {
	return stats_->totalUnColonized();
}

ColonizedSum::ColonizedSum(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

ColonizedSum::~ColonizedSum() {
}

/**
 * Gets the total number of colonized persons using
 * the wrapped PersonStats object.
 */
double ColonizedSum::getData() {
	return stats_->totalColonized();
}

TotalSum::TotalSum(PersonStats* stats) : repast::TDataSource<double>(),
		stats_(stats) {
}

TotalSum::~TotalSum() {
}

/**
 * Gets the total number of persons using
 * the wrapped PersonStats object.
 */
double TotalSum::getData() {
	return stats_->totalUnColonized() + stats_->totalColonized() + stats_->totalInfected();
}

} /* namespace mrsa */
