/*
*MRSA Model
*
*Copyright (c) 2013 University of Chicago and Argonne National Laboratory
*   All rights reserved.
*  
*   Redistribution and use in source and binary forms, with 
*   or without modification, are permitted provided that the following 
*   conditions are met:
*  
*  	 Redistributions of source code must retain the above copyright notice,
*  	 this list of conditions and the following disclaimer.
*  
*  	 Redistributions in binary form must reproduce the above copyright notice,
*  	 this list of conditions and the following disclaimer in the documentation
*  	 and/or other materials provided with the distribution.
*  
*  	 Neither the name of the Argonne National Laboratory nor the names of its
*     contributors may be used to endorse or promote products derived from
*     this software without specific prior written permission.
*  
*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
*   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
*   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
*   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * MRSAObserver.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <map>

#ifdef __APPLE__
	#include <mach/task.h>
	#include <mach/mach.h>
#endif

#include <sys/resource.h>
#include <time.h>

#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/DataSet.h"
// for timestamp
#include "repast_hpc/io.h"

#include "MRSAObserver.h"
#include "Calendar.h"
#include "PersonsCreator.h"
#include "PlaceCreator.h"
#include "ActivityCreator.h"
#include "TransmissionAlgorithm.h"
#include "Parameters.h"
#include "Constants.h"
#include "TransmissionEventRecorder.h"

#include "FileOutput.h"
//#include "RegionMap.h"

namespace mrsa {

using namespace repast::relogo;
using namespace repast;
using namespace std;

struct Timings {
	double setup_time;
	time_t run_start, run_end;
	unsigned long long mem_use;
};

Timings timings;

#ifdef __APPLE__
int getmem (unsigned long long *rss, unsigned long long *vs)
{
    struct task_basic_info_64 t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_64_COUNT;

    if (KERN_SUCCESS != task_info(mach_task_self(),
       TASK_BASIC_INFO_64, (task_info_t)&t_info, &t_info_count))
    {
        return -1;
    }

    if (timings.mem_use < t_info.resident_size) {
    	timings.mem_use = t_info.resident_size;
    }

    *rss = t_info.resident_size;
    *vs  = t_info.virtual_size;
    return 0;
}
#endif

std::string get_mem() {

	unsigned long long rss = 0, vss = 0;
#ifdef __APPLE__
	getmem(&rss, &vss);
#endif
	std::stringstream str;
	str << "RM: " << (rss / 1024.0 /1024.0) << " MB, VM: " << (vss / 1024.0 / 1024.0) << " MB";
			/*(r_usage.ru_maxrss / 1024.0 / 1024.0) << " MB"; */
	return str.str();
}

// counts the number of lines in the specified  persons file.
// and puts the line count and sum of the p_mrsa column the output arguments
void line_count(const std::string& file, unsigned int& line_count, double& p_mrsa_sum) {
	CSVReader reader(file);
	vector<string> vec;
	// skip the header
	reader.skip(1);
	while (reader.next(vec)) {
		p_mrsa_sum += strToDouble(vec[get_mrsa_prob_idx()]);
		++line_count;
	}
}

MRSAObserver::MRSAObserver() :
		personType(0), places(0), summary_output_file(), calendar(), propsPtr(0), yearCounter(0),
		person_count(0){
}

MRSAObserver::~MRSAObserver() {
	// delete each place from the list of places.
	for (vector<Place*>::iterator iter = places.begin(); iter != places.end(); ++iter) {
		delete (*iter);
	}
}

// called each tick
void MRSAObserver::go() {
	int tick = (int) RepastProcess::instance()->getScheduleRunner().currentTick();
	// start the sim at very end of JUNE
	if (tick < YEAR_START)
		return;

	// reset the hourly stats, as they are per tick.
	Statistics* stats = Statistics::getInstance();
	stats->resetHourlyStats();
	calendar.increment();

	// print out time stamp each simulated month
	// on process 0
	if (RepastProcess::instance()->rank() == 0 && tick % 720 == 0) {
		std::string time;
		repast::timestamp(time);
		std::cout << time << " -- Month: " << (tick / 720 - 6) << ", " << get_mem() << std::endl;

	}

	// for each person,

	for (AgentIter iter = context.byTypeBegin(personType); iter != context.byTypeEnd(personType);
			++iter) {
		//for (unsigned int i = 0, n = people_->size(); i < n; i++) {
		//Person* person = (*people_)[i];
		Person* person = static_cast<Person*>((*iter).get());
		// perform the activity for the specified time and day_of_week
		person->performActivity(calendar);
		// update the stats -- update the disease status counts
		// with the status of the current person
		stats->countPerson(person);
	}

	// for each place,
	for (unsigned int i = 0, n = places.size(); i < n; ++i) {
		Place* place = places[i];
		// run the transmission algorithm specific to
		// this place.
		place->runTransmission();
		// reset the place for the next iteration
		place->reset();
	}

	stats->hourEnded();
	// this has to be called last so that we capture the
	// initial infection counts that were set during setup.
	TransmissionAlgorithm::instance()->resetCounts();
}

// load the activities and match them to the persons
void MRSAObserver::initializeActivities(Properties& props) {
	// create the activities -- adding them to a map where
	// the key is the activity list id.
	map<string, vector<Activity> > map;
	ActivityCreator creator;
	creator.run(props.getProperty(ACTIVITIES_FILE), map);

	// each person finds his / her list of activities
	// in the map
	for (AgentIter iter = context.byTypeBegin(personType); iter != context.byTypeEnd(personType);
			++iter) {
		//for (unsigned int i = 0, n = people_->size(); i < n; i++) {
		//Person* person = (*people_)[i];
		Person* person = static_cast<Person*>((*iter).get());
		//std::cout << (*person) << std::endl;
		if (!person->initializeActivities(map)) {
			// if the person can't find its activities then
			// we remove it from the simulation
			person->die();
		}
	}
}

// creates the places. The places are pointers to
// Place so they can be easily shared among persons.
void MRSAObserver::createPlaces(Properties& props, map<string, Place*>* placeMap) {
	const string placesFile = props.getProperty(PLACES_FILE);

	// create the places.
	PlaceCreator placeCreator;
//	placeCreator.run(placesFile, riskFile, places);

	placeCreator.run(placesFile, props, places);

	// put the places in a map. Key is the place id
	// so that the PersonsCreator can easily look up
	// places and assign them to Persons.
	for (int i = 0, n = places.size(); i < n; i++) {
		Place* place = places[i];
		placeMap->insert(pair<string, Place*>(place->placeId(), place));
	}
}

// Creates the Persons. The Persons are turtles and
// so must be created by the Observer using a functor.
void MRSAObserver::createPersons(Properties& props, map<string, Place*>* placeMap) {
	boost::mpi::communicator world;

	const string personsFile = props.getProperty(PERSONS_FILE);

	unsigned int lines = 0;
	double p_mrsa_sum = 0;
	// count the lines in the file so we know how many
	// persons to create: one per line.
	line_count(personsFile, lines, p_mrsa_sum);
	std::cout << lines << ", " << p_mrsa_sum << std::endl;

	float min_infection_duration = (float) strToDouble(props.getProperty(MIN_INFECT_PERIOD));
	Parameters* params = Parameters::instance();

	double mean_jail_duration = params->getDoubleParameter(MEAN_JAIL_DURATION);
	double min_jail_duration = params->getDoubleParameter(MIN_JAIL_DURATION);
	_ExponentialGenerator gen(Random::instance()->engine(),
			boost::exponential_distribution<>(1 / (mean_jail_duration - min_jail_duration)));
	Random::instance()->putGenerator(JAIL_DISTRIBUTION,
			new DefaultNumberGenerator<_ExponentialGenerator>(gen));

	// A PersonsCreator is used as a functor to create the persons
	// in concert with this MRSAObserver.
	PersonsCreator pCreator(personsFile, placeMap, min_infection_duration, min_jail_duration,
			p_mrsa_sum);
	// create N number of persons, where N is the number of non-header
	// lines in the persons file
	personType = create<Person>(lines, pCreator);

	std::vector<Person*> to_kill;
	for (AgentIter iter =
			context.byTypeBegin(personType); iter != context.byTypeEnd(personType); ++iter) {
		//for (unsigned int i = 0, n = people_->size(); i < n; i++) {
		//Person* person = (*people_)[i];
		Person* person = static_cast<Person*>((*iter).get());
		if (!person->validate()) {
			to_kill.push_back(person);
		}
		++person_count;
	}

	for (size_t i = 0, n = to_kill.size(); i < n; ++i) {
		(to_kill[i])->die();
	}

	/*
	 // get all the created Persons and validate them.
	 AgentSet<Person> people;
	 get(people);

	 for (AgentSet<Person>::as_iterator iter = people.begin(); iter != people.end(); iter++) {
	 Person* person = (*iter);
	 // check if a person was assigned any places, and if not
	 // then remove the person with die().
	 person->validate();
	 }

	 people.clear();
	 get(people);
	 */
}

void MRSAObserver::calcYearlyStats() {
	yearCounter++;
	Statistics::getInstance()->yearEnded(context.byTypeBegin(personType),
			context.byTypeEnd(personType), yearCounter, *propsPtr);
}

void MRSAObserver::initializeYearlyDataCollection(const string& file) {
	Statistics* stats = Statistics::getInstance();

	// Create a DataSet that will output data to yearly_data.csv
	SVDataSetBuilder builder(file, ",",
			repast::RepastProcess::instance()->getScheduleRunner().schedule());
	stats->createYearlyDataSources(builder);

	repast::DataSet* ds = builder.createDataSet();
	// add it to the inherit dataSets vector.
	// this insures that it will be written on end
	// and properly cleaned up.
	Observer::dataSets.push_back(ds);
	// schedule the execution for yearly intervals.
	// 8760 = 24 * 365
	ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
	// schedule calcYearlyStats at year + .15
	runner.scheduleEvent(8760.15 + YEAR_START, 8760.0,
			Schedule::FunctorPtr(
					new MethodFunctor<MRSAObserver>(this, &MRSAObserver::calcYearlyStats)));
	// schedule record at year + .2
	runner.scheduleEvent(8760.2 + YEAR_START, 8760.0,
			Schedule::FunctorPtr(new MethodFunctor<repast::DataSet>(ds, &repast::DataSet::record)));
	// schedule write at year + .3
	runner.scheduleEvent(8760.3 + YEAR_START, 8760.0,
			Schedule::FunctorPtr(new MethodFunctor<repast::DataSet>(ds, &repast::DataSet::write)));
	// schedule write at year + .4
	runner.scheduleEvent(8760.4 + YEAR_START, 8760.0,
			Schedule::FunctorPtr(
					new MethodFunctor<Statistics>(stats, &Statistics::resetYearlyStats)));

}

// initialize the data collection by creating
// a data set and adding data sources to it.
void MRSAObserver::initializeHourlyDataCollection(const string& file) {
	Statistics* stats = Statistics::getInstance();

	// Create a DataSet that will output data to hourly_data.csv
	SVDataSetBuilder builder(file, ",",
			repast::RepastProcess::instance()->getScheduleRunner().schedule());
	stats->createHourlyDataSources(builder);

	repast::DataSet* ds = builder.createDataSet();
	// add it to the inherit dataSets vector.
	// this insures that it will be written on end
	// and properly cleaned up.
	Observer::dataSets.push_back(ds);

	ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
	runner.scheduleEvent(YEAR_START + 1.1, 1,
			Schedule::FunctorPtr(new MethodFunctor<repast::DataSet>(ds, &repast::DataSet::record)));
	Schedule::FunctorPtr dsWrite = Schedule::FunctorPtr(
			new MethodFunctor<repast::DataSet>(ds, &repast::DataSet::write));
	runner.scheduleEvent(YEAR_START + 100.2, 100, dsWrite);
}

void MRSAObserver::atEnd() {
	//Statistics::getInstance()->calculateSummaryStats(*people_, summary_output_file, *propsPtr);
	TransmissionEventRecorder::instance()->close();

	time(&timings.run_end);
	std::cout << (timings.setup_time / 60) << "," << (difftime(timings.run_end, timings.run_start) / 60) << "," <<
			(timings.mem_use / 1024.0 / 1024.0) << "," << person_count << "," << places.size() << std::endl;
}

// entry point for model setup.
void MRSAObserver::setup(Properties& props) {
	time_t start;
	time(&start);

	propsPtr = &props;
	Parameters::initialize(props);
	Parameters* params = Parameters::instance();

	// time stamp to mark start of setup
	std::string time_str;
	repast::timestamp(time_str);
	std::cout << "Setup Started at " << time_str << std::endl;

	TransmissionEventRecorder::initialize(props.getProperty(EVENT_OUTPUT_FILE));

	// create a random distribution used by Person-s to
	// choose which "other household" to go to
	_IntUniformGenerator gen(Random::instance()->engine(), boost::uniform_int<>(0, 3));
	Random::instance()->putGenerator(OH_DIST,
			new DefaultNumberGenerator<_IntUniformGenerator>(gen));

	// setup the transmission algorithm with the
	// a, b, c etc. parameters.
	TAParameters taParams;
	taParams.a = params->getDoubleParameter(A);
	taParams.b = params->getDoubleParameter(B);
	taParams.e = params->getDoubleParameter(E);
	taParams.alpha = params->getDoubleParameter(ALPHA);
	taParams.beta = params->getDoubleParameter(THETA);
	taParams.gamma = params->getDoubleParameter(GAMMA);
	taParams.rho = params->getDoubleParameter(Z);

	TransmissionAlgorithm::initialize(taParams);

	// create the persons and places
	// use a pointer because it is eventually passed
	// to the PersonsCreator whose copy constructor is
	// called and so a pointer is faster to copy.
	map<string, Place*>* placeMap = new map<string, Place*>();
	createPlaces(props, placeMap);
	createPersons(props, placeMap);

	delete placeMap;

	// initialize the activities
	initializeActivities(props);

	int i_count = 0;
	int c_count = 0;
	for (AgentIter iter = context.byTypeBegin(personType); iter != context.byTypeEnd(personType);
			++iter) {
		Person* p = static_cast<Person*>((*iter).get());
		if (p->status() == INFECTED)
			++i_count;
		else if (p->status() == COLONIZED)
			++c_count;
	}
	std::cout << i_count << ", " << c_count << std::endl;

	// initialize the hourly data collection
	int runNumber = strToInt(props.getProperty("run.number"));

	std::stringstream hourlyDataFileName;
	hourlyDataFileName << props.getProperty(HOURLY_OUTPUT_FILE) << "_" << runNumber << ".csv";
	initializeHourlyDataCollection(hourlyDataFileName.str());

	// initialize yearly data collection
	std::stringstream yearlyDataFileName;
	yearlyDataFileName << props.getProperty(YEARLY_OUTPUT_FILE) << "_" << runNumber << ".csv";
	initializeYearlyDataCollection(yearlyDataFileName.str());

//	summary_output_file = params->getStringParameter(SUMMARY_OUTPUT_FILE);
	std::stringstream summaryOutputFileName;
	summaryOutputFileName << props.getProperty(SUMMARY_OUTPUT_FILE) << "_" << runNumber << ".txt";
	summary_output_file = summaryOutputFileName.str();

	//FileOutput::initialize("contact_events.csv");
	//(*FileOutput::instance()) << "time, place, place_type, persons" << std::endl;

	// get a permanent set of people so we don't have to retrieve
	// them each iteration which will be slow. We do this here because
	// prevoius setup type methods may have altered the initial set of
	// persons.
	//people_ = new AgentSet<Person>();
	//get(*people_);

	// no long set the initial stat counts
	//Statistics* stats = Statistics::getInstance();
	//stats->setInitialCounts(*people_);

	// schedule MRSAObserver::atEnd() to be called with the sim terminates.
	ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
	runner.scheduleEndEvent(
			Schedule::FunctorPtr(
					new MethodFunctor<MRSAObserver>(this, &mrsa::MRSAObserver::atEnd)));

	time_t finish;
	time(&finish);
	timings.setup_time = difftime(finish, start);
	repast::timestamp(time_str);
	std::cout << "Setup Finished at " << time_str << ", " << get_mem() << std::endl;
	time(&timings.run_start);
}

} /* namespace mrsa */
