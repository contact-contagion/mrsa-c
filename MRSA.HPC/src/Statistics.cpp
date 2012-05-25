/*
 * PersonStats.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: nick
 */

#define BOOST_FILESYSTEM_VERSION 2

#include <boost/filesystem.hpp>

#include "Statistics.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

namespace fs = boost::filesystem;

using namespace repast::relogo;

std::string findFreeFile(const std::string& file) {
	fs::path filepath(file);
	if (!fs::exists(filepath.parent_path()))
		fs::create_directories(filepath.parent_path());
	int i = 1;
	std::string stem = filepath.stem();
	// This will increment i until it hits a unique name
	while (fs::exists(filepath)) {
		++i;
		std::stringstream ss;
		ss << stem << "_" << i << filepath.extension();
		fs::path newName(filepath.parent_path() / ss.str());
		filepath = newName;
	}

	return filepath.string();
}


Statistics::Statistics() :
		total_infected(0), total_colonized(0), total_uncolonized(0), prev_total_infected(0), prev_total_colonized(
				0), ni_over_ti(0), ni_over_tc(0), nc_over_ti(0), nc_over_tc(0), ti_over_pti(0), tc_over_ptc(
				0), count(0), ni_over_tc_sum(0), nc_over_tc_sum(0), nc_over_ti_sum(0), ni_over_ti_sum(
				0), ti_over_pti_sum(0), tc_over_ptc_sum(0) {
}

Statistics::~Statistics() {
}

void Statistics::countPerson(Person* person) {
	// get the person's status and increment the appropriate count.
	DiseaseStatus status = person->status();
	if (status == UNCOLONIZED)
		total_uncolonized++;
	else if (status == COLONIZED)
		total_colonized++;
	else if (status == INFECTED)
		total_infected++;
}

void Statistics::calculateHourlyStats() {
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

void Statistics::calculateSummaryStats(repast::relogo::AgentSet<Person>& people,
		const std::string& file) {
	std::map<unsigned int, unsigned long> infection_hist;
	std::map<unsigned int, unsigned long> colonized_hist;

	long infection_count = 0, colonized_count = 0;
	double infection_duration = 0, colonization_duration = 0;
	for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); ++iter) {
		Person* p = (*iter);
		// setting to UNCOLONIZED will update any "open" stats
		// given that states are only counted when transitioning out
		// of them.
		p->updateStatus(UNCOLONIZED);
		unsigned int i_count = p->status_.infected_count;
		addToHistogram(i_count, infection_hist);
		infection_count += i_count;
		unsigned int c_count = p->status_.colonized_count;
		addToHistogram(c_count, colonized_hist);
		colonized_count += c_count;
		infection_duration += p->status_.infected_time;
		colonization_duration += p->status_.colonized_time;
	}

	if (infection_count != 0)
		infection_duration = infection_duration / infection_count;
	if (colonized_count != 0)
		colonization_duration = colonization_duration / colonized_count;

	std::ofstream out;
	out.open(findFreeFile(file).c_str());

	out << "total infections: " << infection_count << std::endl << "total colonization: "
			<< colonized_count << std::endl << "avg. infection duration: " << infection_duration
			<< std::endl << "avg. colonization duration: " << colonization_duration << std::endl;

	out << "newly_infected_over_total_infected_r0: " << (ni_over_ti_sum / count)
			<< std::endl << "newly_infected_over_total_colonized_r0: "
			<< (ni_over_tc_sum / count) << std::endl
			<< "newly_colonized_over_total_infected_r0: "
			<< (nc_over_ti_sum / count) << std::endl
			<< "newly_colonized_over_total_colonized_r0: "
			<< (nc_over_tc_sum / count) << std::endl

			<< "avg. total infected over prev total infected: "
			<< (ti_over_pti_sum / count) << std::endl
			<< "avg. total colonized over prev total colonized: "
			<< (tc_over_ptc_sum / count) << std::endl;

	for (ConstHistIter iter = infection_hist.begin();
			iter != infection_hist.end(); ++iter) {
		out << "" << iter->second << " persons infected " << iter->first << " times" << std::endl;
	}

	for (ConstHistIter iter = colonized_hist.begin();
			iter != colonized_hist.end(); ++iter) {
		out << "" << iter->second << " persons colonized " << iter->first << " times" << std::endl;
	}

	out.close();

}

void Statistics::addToHistogram(unsigned int count, std::map<unsigned int, unsigned long>& hist) {
	HistIter iter = hist.find(count);
	if (iter == hist.end()) {
		hist.insert(std::pair<unsigned int, unsigned long>(count, 1));
	} else {
		++(iter->second);
	}
}

InfectionSum::InfectionSum(Statistics* stats) :
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

UnColonizedSum::UnColonizedSum(Statistics* stats) :
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

ColonizedSum::ColonizedSum(Statistics* stats) :
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

TotalSum::TotalSum(Statistics* stats) :
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

IOverIR0::IOverIR0(Statistics* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

IOverIR0::~IOverIR0() {
}

double IOverIR0::getData() {
	return stats_->ni_over_ti;
}

IOverCR0::IOverCR0(Statistics* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

IOverCR0::~IOverCR0() {
}

double IOverCR0::getData() {
	return stats_->ni_over_tc;
}

COverIR0::COverIR0(Statistics* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

COverIR0::~COverIR0() {
}

double COverIR0::getData() {
	return stats_->nc_over_ti;
}

COverCR0::COverCR0(Statistics* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

COverCR0::~COverCR0() {
}

double COverCR0::getData() {
	return stats_->nc_over_tc;
}

TCOverPTC::TCOverPTC(Statistics* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

TCOverPTC::~TCOverPTC() {
}

double TCOverPTC::getData() {
	return stats_->tc_over_ptc;
}

TIOverPTI::TIOverPTI(Statistics* stats) :
		repast::TDataSource<double>(), stats_(stats) {
}

TIOverPTI::~TIOverPTI() {
}

double TIOverPTI::getData() {
	return stats_->ti_over_pti;
}

} /* namespace mrsa */
