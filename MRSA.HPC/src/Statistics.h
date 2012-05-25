#ifndef Statistics_H_
#define Statistics_H_

#include <map>

#include "relogo/AgentSet.h"=
#include "repast_hpc/TDataSource.h"

#include "Person.h"

namespace mrsa {

// Used to aggregate Stats from individual persons
struct PersonStats {
	long infection_count, colonized_count;
	double infection_duration, colonization_duration;
	double from_colonization, from_infection;
	double avg_infection_r0, avg_colonization_r0;
	long infected_person_count, colonized_person_count;
};

/**
 * Gather, calculates and writes the statistics for
 * the MRSA model.
 */
class Statistics {

	friend class MRSAObserver;
	friend class TotalSum;

public:
	virtual ~Statistics();

	/**
	 * Gets the singleton instance of the Statistics object.
	 */
	static Statistics* getInstance();

	/**
	 * Clears the counts.
	 */
	void resetHourlyStats() {
		//prev_total_colonized = total_colonized;
		//prev_total_infected = total_infected;
		hourly_infected = hourly_colonized = hourly_uncolonized = 0;
		//ni_over_ti = ni_over_tc = nc_over_ti = nc_over_tc = 0;
		//ti_over_pti = tc_over_ptc = 0;
	}

	void resetYearlyStats() {
		yearly_infected = yearly_colonized = 0;
		yearly_c_from_c = yearly_c_from_i = 0;
		yearly_colonization_duration = yearly_infection_duration = 0;
		colonization_count_map.clear();
		yearly_infected_r0 = yearly_colonized_r0 = 0;
	}

	/**
	 * Increments the colonization count for the specified place type.
	 */
	void incrementColonizationCount(const std::string& type);

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
	 * Calculate the yearly stats from the set of Persons.
	 */
	void calculateYearlyStats(repast::relogo::AgentSet<Person>& people);

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
	typedef std::map<std::string, double>::iterator ColMapIter;

	static Statistics* instance_;

	// private constructor
	Statistics();

	void addToHistogram(unsigned int count, std::map<unsigned int, unsigned long>& hist);
	void updateCountsFromStatsVector(std::list<StatusStats> vec, PersonStats& p_stats);

	// hourly infected etc. totals
	long hourly_infected, hourly_colonized, hourly_uncolonized;

	/*
	 long prev_total_infected, prev_total_colonized;
	 double ni_over_ti, ni_over_tc, nc_over_ti, nc_over_tc;
	 double ti_over_pti, tc_over_ptc;

	 long count;
	 double ni_over_tc_sum, nc_over_tc_sum, nc_over_ti_sum, ni_over_ti_sum;
	 double ti_over_pti_sum, tc_over_ptc_sum;
	 */

	// exposes yearly r0 for data collection
	double yearly_infected_r0, yearly_colonized_r0;
	long yearly_infected, yearly_colonized;
	double yearly_infection_duration, yearly_colonization_duration;
	long yearly_c_from_i, yearly_c_from_c;

	std::map<std::string, double> colonization_count_map;
};

/**
 * Adapts long stat vars to the TDataSource interface.
 */
class LDataSourceAdapter: public repast::TDataSource<double> {

public:
	LDataSourceAdapter(long* stat);
	virtual ~LDataSourceAdapter();

	double getData();

private:
	long* stat_;
};

/**
 * Adapts double stat vars to the TDataSource interface.
 */
class DDataSourceAdapter: public repast::TDataSource<double> {

public:
	DDataSourceAdapter(double* stat);
	virtual ~DDataSourceAdapter();

	double getData();

private:
	double* stat_;
};

/**
 * Repast HPC Data source implementation for
 * returning the total number of persons.
 */
// this adds variables so we can't use the Adapter classes
// defined above.
class TotalSum: public repast::TDataSource<double> {

public:
	TotalSum(Statistics* stats);
	virtual ~TotalSum();

	double getData();

private:
	Statistics* stats_;
};

class PlaceCount: public repast::TDataSource<double> {

public:
	PlaceCount(std::map<std::string, double>* place_map, const std::string& place_name);
	virtual ~PlaceCount();

	double getData();

private:

	std::map<std::string, double>* place_map_;
	std::string place_name_;
};

} /* namespace mrsa */
#endif /* Statistics_H_ */
