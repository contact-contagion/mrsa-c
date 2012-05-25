#ifndef Statistics_H_
#define Statistics_H_

#include "relogo/AgentSet.h"=
#include "repast_hpc/TDataSource.h"

#include "Person.h"

namespace mrsa {

/**
 * Gather, calculates and writes the statistics for
 * the MRSA model.
 */
class Statistics {

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
	friend class SummaryInfectionStats;

public:
	Statistics();
	virtual ~Statistics();

	/**
	 * Clears the counts.
	 */
	void clearHourlyStats() {
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

	/**
	 * Calculates the hourly stats.
	 */
	void calculateHourlyStats();

	/**
	 * Calculates the summary stats and writes them to the specified file.
	 *
	 * @param people an AgentSet of the persons to use to calculate the summary stats
	 * @param file the path of the file to write to
	 */
	void calculateSummaryStats(repast::relogo::AgentSet<Person>& people, const std::string& file);

private:
	typedef std::map<unsigned int, unsigned long>::const_iterator ConstHistIter;
	typedef std::map<unsigned int, unsigned long>::iterator HistIter;

	void addToHistogram(unsigned int count, std::map<unsigned int, unsigned long>& hist);

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
	TIOverPTI(Statistics* stats);
	virtual ~TIOverPTI();

	double getData();

private:
	Statistics* stats_;
};

class TCOverPTC: public repast::TDataSource<double> {

public:
	TCOverPTC(Statistics* stats);
	virtual ~TCOverPTC();

	double getData();

private:
	Statistics* stats_;
};

class IOverIR0: public repast::TDataSource<double> {

public:
	IOverIR0(Statistics* stats);
	virtual ~IOverIR0();

	double getData();

private:
	Statistics* stats_;
};

class IOverCR0: public repast::TDataSource<double> {

public:
	IOverCR0(Statistics* stats);
	virtual ~IOverCR0();

	double getData();

private:
	Statistics* stats_;
};

class COverCR0: public repast::TDataSource<double> {

public:
	COverCR0(Statistics* stats);
	virtual ~COverCR0();

	double getData();

private:
	Statistics* stats_;
};

class COverIR0: public repast::TDataSource<double> {

public:
	COverIR0(Statistics* stats);
	virtual ~COverIR0();

	double getData();

private:
	Statistics* stats_;
};

/**
 * Repast HPC Data source implementation for
 * returning the total number of infected persons.
 */
class InfectionSum: public repast::TDataSource<double> {

public:
	InfectionSum(Statistics* stats);
	virtual ~InfectionSum();

	double getData();

private:
	Statistics* stats_;
};

/**
 * Repast HPC Data source implementation for
 * returning the total number of uncolonized persons.
 */
class UnColonizedSum: public repast::TDataSource<double> {

public:
	UnColonizedSum(Statistics* stats);
	virtual ~UnColonizedSum();

	double getData();

private:
	Statistics* stats_;
};

/**
 * Repast HPC Data source implementation for
 * returning the total number of colonized persons.
 */
class ColonizedSum: public repast::TDataSource<double> {

public:
	ColonizedSum(Statistics* stats);
	virtual ~ColonizedSum();

	double getData();

private:
	Statistics* stats_;
};

/**
 * Repast HPC Data source implementation for
 * returning the total number of persons.
 */
class TotalSum: public repast::TDataSource<double> {

public:
	TotalSum(Statistics* stats);
	virtual ~TotalSum();

	double getData();

private:
	Statistics* stats_;
};

} /* namespace mrsa */
#endif /* Statistics_H_ */
