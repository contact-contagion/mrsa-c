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
	PersonStatus status = person->getStatus();
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

double InfectionSum::getData() {
	return stats_->totalInfected();
}

UnColonizedSum::UnColonizedSum(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

UnColonizedSum::~UnColonizedSum() {
}

double UnColonizedSum::getData() {
	return stats_->totalUnColonized();
}

ColonizedSum::ColonizedSum(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

ColonizedSum::~ColonizedSum() {
}

double ColonizedSum::getData() {
	return stats_->totalColonized();
}

TotalSum::TotalSum(PersonStats* stats) : repast::TDataSource<double>(),
		stats_(stats) {
}

TotalSum::~TotalSum() {
}

double TotalSum::getData() {
	return stats_->totalUnColonized() + stats_->totalColonized() + stats_->totalInfected();
}

} /* namespace mrsa */
