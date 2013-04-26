#ifndef Statistics_H_
#define Statistics_H_

#include <map>

#include "relogo/AgentSet.h"
#include "repast_hpc/TDataSource.h"

#include "Person.h"
#include "PlaceStats.h"

namespace mrsa {

struct RegionStat {

	char region;
	long infection_incidence, colonization_incidence;
	long infection_prevalence, colonization_prevalence;
	long c_from_i, c_from_c, i_to_c_from_na;

	//RegionStat(const char& r) : region(r), infection_incidence(0), colonization_incidence(0),
	//		infection_prevalence(0), colonization_prevalence(0), c_from_i(0), c_from_c(0) {

	//}

	void reset() {
		infection_incidence = colonization_incidence = infection_prevalence =
				colonization_prevalence = c_from_i = c_from_c = i_to_c_from_na = 0;
	}
};

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
		yearly_c_from_c = yearly_c_from_i = yearly_i_to_c_from_na = 0;
		yearly_colonization_duration = yearly_infection_duration = 0;
		yearly_infected_r0 = yearly_colonized_r0 = 0;
		colonization_from_infection_override = 0;

		for (std::map<char, RegionStat>::iterator iter = region_stats.begin();
				iter != region_stats.end(); ++iter) {
			iter->second.reset();
		}

		for (std::map<std::string, PlaceStats>::iterator iter = place_stats.begin();
				iter != place_stats.end(); ++iter) {
			iter->second.reset();
		}
	}

	/**
	 * Manually increment colonization from infection. This should
	 * only be called when there isn't an actualy person to
	 * tag as the cause of the infection.
	 */
	void incrementColonizationFromInfection();

	/**
	 * Set the initial counts using the specified AgentSet of persons.
	 * This counts people whose disease status is set as part of setup
	 * rather than through the TA algorithm.
	 */
	void setInitialCounts(repast::relogo::AgentSet<Person>& people);

	/**
	 * Increments the colonization count for the specified place type.
	 */
	void incrementColonizationCount(const std::string& type, const unsigned int zip_code);

	/**
	 * Increments the infection count for the specified place type.
	 */
	void incrementInfectionCount(const std::string& type, const unsigned int zip_code);

	/**
	 * Increments the hospital stay count.
	 */
	void incrementHospitalStayCount(unsigned int zip_code);

	/**
	 * Increments the hospital stay duration count by the specified amount.
	 */
	void incrementHospitalDurationCount(double hours, unsigned int zip_code);

	/**
	 * Increments the prison stay count.
	 */
	void incrementPrisonStayCount(unsigned int zip_code);

	/**
	 * Increments the prison stay duration count by the specified amount.
	 */
	void incrementPrisonDurationCount(double hours, unsigned int zip_code);

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
	void yearEnded(repast::Context<repast::relogo::RelogoAgent>::const_bytype_iterator begin,
			repast::Context<repast::relogo::RelogoAgent>::const_bytype_iterator, int year, repast::Properties& props);

	/**
	 * Calculates the summary stats and writes them to the specified file.
	 *
	 * @param people an AgentSet of the persons to use to calculate the summary stats
	 * @param file the path of the file to write to
	 */
	void calculateSummaryStats(repast::relogo::AgentSet<Person>& people, const std::string& file,
			repast::Properties& props);

	/**
	 * Creates data sources that produce yearly data and adds them to the specified builder
	 */
	void createYearlyDataSources(repast::SVDataSetBuilder& builder);

	/**
	 * Creates the data sources that produce hourly data and adds them to the
	 * specified builder.
	 */
	void createHourlyDataSources(repast::SVDataSetBuilder& builder);

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
	long yearly_c_from_i, yearly_c_from_c, yearly_i_to_c_from_na;

	long total_c_from_i, total_c_from_c;
	double total_infected, total_colonized;

	long colonization_from_infection_override;

	std::map<std::string, PlaceStats> place_stats;
	std::map<char, RegionStat> region_stats;
	YearlyAvg averages;
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

} /* namespace mrsa */
#endif /* Statistics_H_ */
