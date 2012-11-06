#ifndef Statistics_H_
#define Statistics_H_

#include <map>

#include "relogo/AgentSet.h"=
#include "repast_hpc/TDataSource.h"

#include "Person.h"

namespace mrsa {

// captures yearly averages, used
// to compute the total averages.
struct YearlyAvg {
	double yearly_infected_r0, yearly_colonized_r0;
	double yearly_infection_duration, yearly_colonization_duration;
	// infection duration for those don't and do seek care
	double yearly_no_seek_infection_duration, yearly_seek_infection_duration;
	int count;
};

// struct used to aggregate inidividual stats
// over all persons.
struct PersonStats {
	// the infection / colonized counts
	long infection_count, colonized_count;
	long seek_infection_count, no_seek_infection_count;
	// durations
	double infection_duration, colonization_duration;
	// infection duration for those don't and do seek care
	double no_seek_infection_duration, seek_infection_duration;
	// number of colonizations attributable colonization and infection
	double from_colonization, from_infection;
	// avergage r0 values
	double avg_infection_r0, avg_colonization_r0, avg_r0;
	// infected / colonized counts
	long infected_person_count, colonized_person_count, i_or_c_count;
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
	 * Clears the hourly counts.
	 */
	void resetHourlyStats() {
		//prev_total_colonized = total_colonized;
		//prev_total_infected = total_infected;
		hourly_infected = hourly_colonized = hourly_uncolonized = 0;
		//ni_over_ti = ni_over_tc = nc_over_ti = nc_over_tc = 0;
		//ti_over_pti = tc_over_ptc = 0;
	}

	/**
	 * Clears yearly counts. This should be called before
	 * starting yearly counts.
	 */
	void resetYearlyStats() {
		yearly_infected = yearly_colonized = 0;
		eoy_prevalence_infected = eoy_prevalence_colonized = 0;
		yearly_c_from_c = yearly_c_from_i = 0;
		yearly_colonization_duration = yearly_infection_duration = 0;
		colonization_count_map.clear();
		infection_count_map.clear();
		yearly_infected_r0 = yearly_colonized_r0 = 0;
		hospital_stays = 0;
		hospital_stay_duration = 0;
		hospital_infections = hospital_colonizations = 0;
	}

	/**
	 * Set the initial counts using the specified AgentSet of persons.
	 * This counts people whose disease status is set as part of setup
	 * rather than through the TA algorithm.
	 */
	void setInitialCounts(repast::relogo::AgentSet<Person>& people);

	/**
	 * Increments the colonization count for the specified place type.
	 */
	void incrementColonizationCount(const std::string& type);

	/**
	 * Increments the infection count for the specified place type.
	 */
	void incrementInfectionCount(const std::string& type);

	/**
	 * Increments the hospital stay count.
	 */
	void incrementHospitalStayCount() {
		++hospital_stays;
	}

	/**
	 * Increments the hospital stay duration count by the specified amount.
	 */
	void incrementHospitalDurationCount(double hours) {
		hospital_stay_duration += hours;
	}

	/**
	 * Counts the specified Person, incrementing
	 * the appropriate counter depending on the person's
	 * disease status.
	 */
	void countPerson(Person* person);

	/**
	 * Gathers / calculates any stats at the end of an hour.
	 */
	void hourEnded();

	/**
	 * Calculate stats at the end of a year using the set of
	 * persons.
	 */
	void yearEnded(repast::relogo::AgentSet<Person>& people, int year, repast::Properties& props);

	/**
	 * Calculates the summary stats and writes them to the specified file.
	 *
	 * @param people an AgentSet of the persons to use to calculate the summary stats
	 * @param file the path of the file to write to
	 */
	void calculateSummaryStats(repast::relogo::AgentSet<Person>& people, const std::string& file,
			repast::Properties& props);

private:
	typedef std::map<unsigned int, unsigned long>::const_iterator ConstHistIter;
	typedef std::map<unsigned int, unsigned long>::iterator HistIter;
	typedef std::map<std::string, double>::iterator ColMapIter;

	static Statistics* instance_;

	// private constructor
	Statistics();

	// increments the histogram hist for the specified key 'count'
	void addToHistogram(unsigned int count, std::map<unsigned int, unsigned long>& hist);
	// updates the PersonStats struct using data from the person
	void updateCountsFromStatsVector(Person* p, PersonStats& p_stats);

	// hourly infected etc. totals
	long hourly_infected, hourly_colonized, hourly_uncolonized;
	long newly_infected, newly_colonized;

	// these vars are calculated then exposed via the TDataSource API
	// in order to log them using RepastHPC data collection.
	double yearly_infected_r0, yearly_colonized_r0, yearly_r0;
	long yearly_infected, yearly_colonized;
	long eoy_prevalence_infected, eoy_prevalence_colonized;
	double yearly_no_seek_infection_duration, yearly_seek_infection_duration;
	double yearly_infection_duration, yearly_colonization_duration;
	long yearly_c_from_i, yearly_c_from_c;

	long total_c_from_i, total_c_from_c;
	double total_infected, total_colonized;

	long hospital_stays;
	double hospital_stay_duration;
	double hospital_colonizations, hospital_infections;

	std::map<std::string, double> colonization_count_map;
	std::map<std::string, double> infection_count_map;
	YearlyAvg averages;
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

/**
 * Adapts map double values to the TDataSource API. The constructor
 * takes a map key and a map. getData returns the current value
 * for that key.
 */
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
