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
public:
	PersonStats();
	virtual ~PersonStats();

	/**
	 * Clears the counts.
	 */
	void clear() {
		total_infected = total_colonized = total_uncolonized = 0;
	}

	/**
	 * Counts the specified Person, incrementing
	 * the appropriate counter depending on the person's
	 * disease status.
	 */
	void countPerson(Person* person);

	/**
	 * Gets the total number of infected persons.
	 */
	const long totalInfected() const {
		return total_infected;
	}

	/**
	 * Gets the total number of colonized persons.
	 */
	const long totalColonized() const {
		return total_colonized;
	}

	/**
	 * Gets the total number of uncolonized persons.
	 */
	const long totalUnColonized() const {
		return total_uncolonized;
	}

private:
	long total_infected, total_colonized, total_uncolonized;
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
