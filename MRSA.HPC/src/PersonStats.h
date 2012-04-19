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

class PersonStats {
public:
	PersonStats();
	virtual ~PersonStats();

	void clear() {
		total_infected = total_colonized = total_uncolonized = 0;
	}

	void countPerson(Person* person);

	const long totalInfected() const {
		return total_infected;
	}

	const long totalColonized() const {
		return total_colonized;
	}

	const long totalUnColonized() const {
		return total_uncolonized;
	}

private:
	long total_infected, total_colonized, total_uncolonized;
};

class InfectionSum : public repast::TDataSource<double> {

public:
	InfectionSum(PersonStats* stats);
	virtual ~InfectionSum();

	double getData();

private:
	PersonStats* stats_;
};

class UnColonizedSum : public repast::TDataSource<double> {

public:
	UnColonizedSum(PersonStats* stats);
	virtual ~UnColonizedSum();

	double getData();

private:
	PersonStats* stats_;
};

class ColonizedSum : public repast::TDataSource<double> {

public:
	ColonizedSum(PersonStats* stats);
	virtual ~ColonizedSum();

	double getData();

private:
	PersonStats* stats_;
};

class TotalSum : public repast::TDataSource<double> {

public:
	TotalSum(PersonStats* stats);
	virtual ~TotalSum();

	double getData();

private:
	PersonStats* stats_;
};

} /* namespace mrsa */
#endif /* PersonStats_H_ */
