/*
 * PersonStats.h
 *
 *  Created on: Apr 19, 2012
 *      Author: nick
 */

#ifndef PersonStats_H_
#define PersonStats_H_

#include "relogo/AgentSet.h"=
#include "repast_hpc/TDataSource.h"

#include "Person.h"

namespace mrsa {

/**
 * Counts the number of colonized, infected, etc. persons
 * each tick of the simulation. This should be reset prior
 * to each iteration
 */
class PersonStats {

	friend class ColonizedSum;
	friend class InfectionSum;
	friend class TotalSum;
	friend class UnColonizedSum;
	friend class COverCR0;
	friend class COverIR0;
	friend class IOverCR0;
	friend class IOverIR0;
	friend class TIOverPTI;
	friend class TCOverPTC;

public:
	PersonStats();
	virtual ~PersonStats();

	/**
	 * Clears the counts.
	 */
	void clear() {
		prev_total_colonized = total_colonized;
		prev_total_infected = total_infected;
		total_infected = total_colonized = total_uncolonized = 0;
		ni_over_ti = ni_over_tc = nc_over_ti = nc_over_tc = 0;
		ti_over_pti = tc_over_ptc = 0;
	}

	/**
	 * Counts the specified Person, incrementing
	 * the appropriate counter depending on the person's
	 * disease status.
	 */
	void countPerson(Person* person);

	void calculateR0Values();

	void avg() {
		std::cout << "newly_infected_over_total_infected_r0: " << (ni_over_ti_sum / count) << std::endl
				<< "newly_infected_over_total_colonized_r0: " << (ni_over_tc_sum / count) << std::endl
				<< "newly_colonized_over_total_infected_r0: " << (nc_over_ti_sum / count) << std::endl
				<< "newly_colonized_over_total_colonized_r0: " << (nc_over_tc_sum / count) << std::endl

				<< "avg. total infected over prev total infected: " << (ti_over_pti_sum / count) << std::endl
				<< "avg. total colonized over prev total colonized: " << (tc_over_ptc_sum / count) << std::endl;

	}

private:
	long total_infected, total_colonized, total_uncolonized;
	long prev_total_infected, prev_total_colonized;
	double ni_over_ti, ni_over_tc, nc_over_ti, nc_over_tc;
	double ti_over_pti, tc_over_ptc;
	long count;
	double ni_over_tc_sum, nc_over_tc_sum, nc_over_ti_sum, ni_over_ti_sum;
	double ti_over_pti_sum, tc_over_ptc_sum;
};

class TIOverPTI: public repast::TDataSource<double> {

public:
	TIOverPTI(PersonStats* stats);
	virtual ~TIOverPTI();

	double getData();

private:
	PersonStats* stats_;
};

class TCOverPTC: public repast::TDataSource<double> {

public:
	TCOverPTC(PersonStats* stats);
	virtual ~TCOverPTC();

	double getData();

private:
	PersonStats* stats_;
};

class IOverIR0: public repast::TDataSource<double> {

public:
	IOverIR0(PersonStats* stats);
	virtual ~IOverIR0();

	double getData();

private:
	PersonStats* stats_;
};

class IOverCR0: public repast::TDataSource<double> {

public:
	IOverCR0(PersonStats* stats);
	virtual ~IOverCR0();

	double getData();

private:
	PersonStats* stats_;
};

class COverCR0: public repast::TDataSource<double> {

public:
	COverCR0(PersonStats* stats);
	virtual ~COverCR0();

	double getData();

private:
	PersonStats* stats_;
};

class COverIR0: public repast::TDataSource<double> {

public:
	COverIR0(PersonStats* stats);
	virtual ~COverIR0();

	double getData();

private:
	PersonStats* stats_;
};



/**
 * Repast HPC Data source implementation for
 * returning the total number of infected persons.
 */
class InfectionSum: public repast::TDataSource<double> {

public:
	InfectionSum(PersonStats* stats);
	virtual ~InfectionSum();

	double getData();

private:
	PersonStats* stats_;
};

/**
 * Repast HPC Data source implementation for
 * returning the total number of uncolonized persons.
 */
class UnColonizedSum: public repast::TDataSource<double> {

public:
	UnColonizedSum(PersonStats* stats);
	virtual ~UnColonizedSum();

	double getData();

private:
	PersonStats* stats_;
};

/**
 * Repast HPC Data source implementation for
 * returning the total number of colonized persons.
 */
class ColonizedSum: public repast::TDataSource<double> {

public:
	ColonizedSum(PersonStats* stats);
	virtual ~ColonizedSum();

	double getData();

private:
	PersonStats* stats_;
};

/**
 * Repast HPC Data source implementation for
 * returning the total number of persons.
 */
class TotalSum: public repast::TDataSource<double> {

public:
	TotalSum(PersonStats* stats);
	virtual ~TotalSum();

	double getData();

private:
	PersonStats* stats_;
};

} /* namespace mrsa */
#endif /* PersonStats_H_ */
