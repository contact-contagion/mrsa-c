/*
 * PersonStats.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: nick
 */

#include "PersonStats.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

using namespace repast::relogo;

PersonStats::PersonStats() :
		total_infected(0), total_colonized(0), total_uncolonized(0), prev_total_infected(0), prev_total_colonized(
				0), ni_over_ti(0), ni_over_tc(0), nc_over_ti(0), nc_over_tc(0), ti_over_pti(0), tc_over_ptc(
				0), count(0), ni_over_tc_sum(0), nc_over_tc_sum(0), nc_over_ti_sum(0), ni_over_ti_sum(0),
				ti_over_pti_sum(0), tc_over_ptc_sum(0) {
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

void PersonStats::calculateR0Values() {
	double newly_colonized = TransmissionAlgorithm::instance()->newlyColonizedCount();
	double newly_infected = TransmissionAlgorithm::instance()->newlyInfectedCount();

	ni_over_tc = newly_infected / total_colonized;
	nc_over_tc = newly_colonized / total_colonized;
	nc_over_ti = newly_colonized / total_infected;
	ni_over_ti = newly_infected / total_infected;

	ti_over_pti = total_infected / ((double) prev_total_infected);
	tc_over_ptc = total_colonized / ((double) prev_total_colonized);

	++count;
	ni_over_tc_sum += ni_over_tc;
	nc_over_tc_sum += nc_over_tc;
	nc_over_ti_sum += nc_over_ti;
	ni_over_ti_sum += ni_over_ti;

	ti_over_pti_sum += ti_over_pti;
	tc_over_ptc_sum += tc_over_ptc;
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
	return stats_->total_infected;
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
	return stats_->total_uncolonized;
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
	return stats_->total_colonized;
}

TotalSum::TotalSum(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

TotalSum::~TotalSum() {
}

/**
 * Gets the total number of persons using
 * the wrapped PersonStats object.
 */
double TotalSum::getData() {
	return stats_->total_uncolonized + stats_->total_colonized + stats_->total_infected;
}

IOverIR0::IOverIR0(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

IOverIR0::~IOverIR0() {
}

double IOverIR0::getData() {
	return stats_->ni_over_ti;
}

IOverCR0::IOverCR0(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

IOverCR0::~IOverCR0() {
}

double IOverCR0::getData() {
	return stats_->ni_over_tc;
}

COverIR0::COverIR0(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

COverIR0::~COverIR0() {
}

double COverIR0::getData() {
	return stats_->nc_over_ti;
}

COverCR0::COverCR0(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

COverCR0::~COverCR0() {
}

double COverCR0::getData() {
	return stats_->nc_over_tc;
}

TCOverPTC::TCOverPTC(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

TCOverPTC::~TCOverPTC() {}

double TCOverPTC::getData() {
	return stats_->tc_over_ptc;
}

TIOverPTI::TIOverPTI(PersonStats* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

TIOverPTI::~TIOverPTI() {}

double TIOverPTI::getData() {
	return stats_->ti_over_pti;
}

} /* namespace mrsa */
