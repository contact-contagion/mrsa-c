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


#include <boost/mpi.hpp>
#include <boost/serialization/map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/std/vector.hpp>

#include <iostream>
#include <algorithm>
#include <iterator>
#include <exception>

#include "repast_hpc/io.h"
#include "repast_hpc/RepastProcess.h"
#include "relogo/SimulationRunner.h"
#include "relogo/Patch.h"
//#include "repast_hpc/initialize_random.h"

#include "MRSAObserver.h"
#include "CSVReader.h"
#include "../src/utility.h"
#include "Constants.h"

using namespace repast;
using namespace relogo;
using namespace mrsa;

static const int countOfInputRecordValues = 30;
static const int countOfRecordValues = 46;

const int TYPE_IDX = 1;
const int YEARLY_DATA_LENGTH = 39;
const int MAX_YEAR = 5;

using namespace boost::assign;

void usage() {
	std::cerr << "usage: X  string string" << std::endl;
	std::cerr
			<< "  first string: string is the path to the Repast HPC \n\tconfiguration properties file"
			<< std::endl;
	std::cerr << "  second string: string is the path to the model properties file" << std::endl;
}

void setPropertiesForSweep(Properties& props, int sweepIndex) {
	boost::mpi::communicator world;

	srand((int) (time(NULL) * world.rank()));
	for (int i = 0; i < world.rank(); i++)
		rand();
	std::stringstream ss;
	ss << std::fixed << ((world.rank() + 1) * (rand() % 3727));
	props.putProperty("random.seed", ss.str());

//  std::vector<float> aVals, bVals, eVals;
//  aVals += 2.05e-05f, 2.18e-05f, 2.31e-05f, 2.44e-05f;
//  bVals += 2.38e-05f, 2.45e-05f, 2.52e-05f, 2.65e-05f, 2.72e-05f, 2.79e-05f, 3.40e-05f, 4.00e-05f;
//  eVals += 3.26e-05f, 4.12e-05f, 4.98e-05f, 5.84e-05;
//
//  int c = 0;
//  for(size_t i = 0; i < aVals.size(); i++){
//    for(size_t j = 0; j < bVals.size(); j++){
//      for(size_t k = 0; k < eVals.size(); k++){
//        if(c == sweepIndex){
//          // Set Properties here
//          props.putProperty("a", aVals[i]);
//          props.putProperty("b", bVals[j]);
//          props.putProperty("e", eVals[k]);
//          return;
//        }
//        c++;
//      }
//    }
//  }

	if (props.contains("SWEEP")) {
		std::string sweepList = props.getProperty("SWEEP");
		std::vector<std::string> list;
		boost::split(list, sweepList, boost::is_any_of(","));
		int* sizes = new int[list.size()];
		int* index = new int[list.size()];
		std::vector<std::vector<std::string> > valArray;
		for (size_t i = 0; i < list.size(); i++) {
			sizes[i] = 1;
			index[i] = 0;
			if (props.contains(list[i])) {
				std::string valList = props.getProperty(list[i]);
				std::vector<std::string> vals;
				boost::split(vals, valList, boost::is_any_of(","));
				valArray.push_back(vals);
			}
		}
		for (int i = list.size() - 2; i >= 0; i--)
			sizes[i] = valArray[i + 1].size() * sizes[i + 1];

		if (sweepIndex < sizes[0] * valArray[0].size()) { // If sweepIndex is beyond sweeps, ignore
			for (int i = 0; i < list.size(); i++) {
				index[i] = sweepIndex / sizes[i];
				sweepIndex -= (index[i] * sizes[i]);
			}

			for (int i = 0; i < list.size(); i++) {
				std::cout << "rank: " << world.rank() << " i " << i << " sizes[i] " << sizes[i]
						<< " value: " << valArray[i][index[i]] << std::endl;
				props.putProperty(list[i], valArray[i][index[i]]);
			}
		}
		delete sizes;
		delete index;
	}

	// Get the property. If it begins with 'SWEEP'
	// Split the property
	// Create a new vector
	// Add all elements in split to vector (Except first)
	// Add to map with property name, vector

	// Create two arrays
	//

}

void getKeysToWrite(std::vector<std::string>& keylist, bool output = false) {

		keylist +=
		// Basic Info:
				"run.number", "date_time.run", "run.time", "random.seed", "stop.at",

		// Obsolete?
		"seek.and.destroy.at", "seek.and.destroy.cure.fraction",

		// Input files:
		"persons.file", "places.file", "activities.file", "risk.file",

		// Output files:
		"hourly.output.file", "yearly.output.file", "summary.output.file",

		// Parameters:
		"initial.colonization.scaling", "initial.infected.count", "minimum.infection.period", "a", "b", "e",
		"minimum.jail.stay", "maximum.jail.stay",

		// Risks:
		"Hospital_0_PAR", "Hospital_0_TIP", "Hospital_0_AIP", "Hospital_0_X", "Hospital_1_PAR", "Hospital_1_TIP", "Hospital_1_AIP", "Hospital_1_X",

		"Workplace_0_PAR", "Workplace_0_TIP", "Workplace_0_AIP", "Workplace_0_X", "Workplace_1_PAR", "Workplace_1_TIP", "Workplace_1_AIP", "Workplace_1_X",

		"Household_0_PAR", "Household_0_TIP", "Household_0_AIP", "Household_0_X", "Household_1_PAR", "Household_1_TIP", "Household_1_AIP", "Household_1_X",

		"School_0_PAR", "School_0_TIP", "School_0_AIP", "School_0_X", "School_1_PAR", "School_1_TIP", "School_1_AIP", "School_1_X",

		"Gym_0_PAR", "Gym_0_TIP", "Gym_0_AIP", "Gym_0_X","Gym_1_PAR", "Gym_1_TIP", "Gym_1_AIP", "Gym_1_X",

		"Nursing_Home_0_PAR", "Nursing_Home_0_TIP", "Nursing_Home_0_AIP", "Nursing_Home_0_X","Nursing_Home_1_PAR", "Nursing_Home_1_TIP", "Nursing_Home_1_AIP",
		"Nursing_Home_1_X",

		"Prison_0_PAR", "Prison_0_TIP", "Prison_0_AIP", "Prison_0_X", "Prison_1_PAR", "Prison_1_TIP", "Prison_1_AIP", "Prison_1_X",

		"College_Dorm_0_PAR", "College_Dorm_0_TIP", "College_Dorm_0_AIP", "College_Dorm_0_X", "College_Dorm_1_PAR", "College_Dorm_1_TIP", "College_Dorm_1_AIP",
		"College_Dorm_1_X",

		// Spatial for ReLogo
		"min.x", "min.y", "max.x", "max.y", "grid.buffer", "proc.per.x", "proc.per.y";
	if (output) {
		keylist +=

		// Output
		"process.count", "run.time",

		// Summary Statistics
		"total_infections", "colonizations", "total_from_infection", "total_from_colonization";



		std::string yearly_data[YEARLY_DATA_LENGTH] = {"infections_incidence", "colonizations_incidence", "infections_prevalence",
				"colonizations_prevalence", "hospital_colonizations", "hospital_infections", "hospital_stays",
				"hospital_days",
				"jail_colonizations", "jail_infections", "jail_stays", "jail_days",
				"jail_colonizations_N", "jail_infections_N", "jail_stays_N", "jail_days_N",
				"jail_colonizations_S", "jail_infections_S", "jail_stays_S", "jail_days_S",
				"jail_colonizations_W", "jail_infections_W", "jail_stays_W", "jail_days_W",
				"jail_colonizations_C", "jail_infections_C", "jail_stays_C", "jail_days_C",
				"jail_colonizations_O", "jail_infections_O", "jail_stays_O", "jail_days_O",

				HOUSEHOLD_COL_COUNT, OTHER_H_COL_COUNT, WORKPLACE_COL_COUNT,
				SCHOOL_COL_COUNT, GYM_COL_COUNT, DORM_COL_COUNT, NURSING_HOME_COL_COUNT
		};

		for (int i = 0; i < YEARLY_DATA_LENGTH; ++i) {
			for (int j = 1; j <= MAX_YEAR; ++j) {
				std::stringstream ss;
				ss << yearly_data[i] << "_year_" << j;
				keylist.push_back(ss.str());
			}
		}


		// By Year Stats
//		"infections_incidence_year_1", "infections_incidence_year_2", "infections_incidence_year_3", "infections_incidence_year_4",
//		"infections_incidence_year_5", "colonizations_incidence_year_1", "colonizations_incidence_year_2", "colonizations_incidence_year_3",
//		"colonizations_incidence_year_4", "colonizations_incidence_year_5", "infections_prevalence_year_1", "infections_prevalence_year_2",
//		"infections_prevalence_year_3", "infections_prevalence_year_4", "infections_prevalence_year_5", "colonizations_prevalence_year_1",
//		"colonizations_prevalence_year_2", "colonizations_prevalence_year_3", "colonizations_prevalence_year_4", "colonizations_prevalence_year_5",
//
//		"hospital_colonizations_year_1", "hospital_colonizations_year_2", "hospital_colonizations_year_3", "hospital_colonizations_year_4",
//		"hospital_colonizations_year_5", "hospital_infections_year_1", "hospital_infections_year_2", "hospital_infections_year_3",
//		"hospital_infections_year_4", "hospital_infections_year_5", "hospital_stays_year_1", "hospital_stays_year_2", "hospital_stays_year_3",
//		"hospital_stays_year_4", "hospital_stays_year_5", "hospital_days_year_1", "hospital_days_year_2", "hospital_days_year_3", "hospital_days_year_4",
//		"hospital_days_year_5";
	}
}

void writePropertiesFromAllProcesses(Properties& props, std::string fileName, bool includeOutput =
		false) {
	std::map<std::string, std::string> map;
	Properties::key_iterator keyIter = props.keys_begin();
	while (keyIter != props.keys_end()) {
		// cast isn't strictly necessary but otherwise eclipse gives spurious error
		map[*keyIter] = props.getProperty((std::string)(*keyIter));
		keyIter++;
	}

	boost::mpi::communicator world;
	if (world.rank() != 0) {
		boost::mpi::gather(world, map, 0);
	} else {
		std::vector<std::map<std::string, std::string> > rec;
		boost::mpi::gather(world, map, rec, 0);

		std::vector<std::string> keysToWrite;
		getKeysToWrite(keysToWrite, includeOutput);
		for (size_t i = 0; i < rec.size(); i++) {
			Properties outProps;
			std::map<std::string, std::string> recdMap = rec[i];
			for (std::map<std::string, std::string>::iterator iter = recdMap.begin();
					iter != recdMap.end(); iter++) {
				outProps.putProperty(iter->first, iter->second);
			}
			outProps.log("root");
			outProps.writeToSVFile(fileName, keysToWrite);
		}
	}
}

/**
 * Runs the MRSA model with the specified model.props file and
 * optional model prop command line values.
 */
void runModel(std::string propsFile, std::string config, int argc, char ** argv) {
	boost::mpi::communicator world;
	// create a properties object from the props file.
	Properties props(propsFile, argc, argv, &world);

	mrsa::load_risks(props);
	mrsa::load_init_col_inf_fractions(props);

	// add a starting time property
	std::string time;
	repast::timestamp(time);
	props.putProperty("date_time.run", time);

	// add process count property
	props.putProperty("process.count", world.size());

	boost::mpi::communicator sub;
	if (props.contains("do.parameter.sweeps")) {
		int sweepIndex = world.rank();
		if (props.contains("base.sweep.number"))
			sweepIndex += strToInt(props.getProperty("base.sweep.number"));

		// If a 'run.number' is provided, use it.
		if (!props.contains("run.number")) {
			// If not, calculate from sweep index
			// If a 'base.run.number' is provided, start from there, otherwise start from 0
			int baseRunNumber = 0;
			if (props.contains("base.run.number"))
				baseRunNumber = strToInt(props.getProperty("base.run.number"));
			props.putProperty("run.number", baseRunNumber + sweepIndex);
		}

		// Set properties specific to this run
		setPropertiesForSweep(props, sweepIndex);

		// Create SUBCOMMUNICATORS for all ranks
		sub = world.split(world.rank());
	}

	if (!props.contains("run.number"))
		props.putProperty("run.number", -1);
	RepastProcess::init(config, &sub);

	// Write INPUT parameters (in case of crash)
	std::string inputRecordLocation = (
			props.contains("input.record.location") ?
					props.getProperty("input.record.location") : "output/mrsa_model_INPUT.csv");
	writePropertiesFromAllProcesses(props, inputRecordLocation);


        std::stringstream  eventOutputFile;
	eventOutputFile << props.getProperty("event.output.file") << "_" << props.getProperty("run.number") << ".csv";
        props.putProperty("event.output.file", eventOutputFile.str());


	try {
		// create a simulation runner to run the MRSA model.
		SimulationRunner runner(&sub);

		if (sub.rank() == 0)
			std::cout << " Starting on rank: " << world.rank() << std::endl;

		// start timer to measure how long the model takes to run
		repast::Timer timer;
		timer.start();
		// run the model
		runner.run<MRSAObserver, Patch>(props);
		if (sub.rank() == 0)
			std::cout << " Done on rank: " << world.rank() << std::endl;

		// model has now finished so add the stop time to the properties
		props.putProperty("run.time", timer.stop());

		// Write out properties and output
		std::string outputRecordLocation =
				(props.contains("output.record.location") ?
						props.getProperty("output.record.location") : "output/mrsa_model_OUTPUT.csv");
		writePropertiesFromAllProcesses(props, outputRecordLocation, true);
	} catch (std::exception& exp) {
		// catch any exception (e.g. if data files couldn't be opened) and
		// print out the errors.
		std::cerr << "ERROR: " << exp.what() << std::endl;
	}

}

/**
 * Main entry point to the MRSA model.
 */
int main(int argc, char **argv) {
	// initialize MPI via boost
	boost::mpi::environment env(argc, argv);
	std::string config, props;
	boost::mpi::communicator world;

	// grab the args for the config and model properties files.
	if (argc >= 3) {
		config = argv[1];
		props = argv[2];

	} else {
		if (world.rank() == 0)
			usage();
		return 0;
	}

	// initialize Repast HPC and run the model.
	if (config.size() > 0 && props.size() > 0) {
		runModel(props, config, argc, argv);
	} else {
		if (world.rank() == 0)
			usage();
		return 0;
	}

	// clean up RepastHPC
	RepastProcess::instance()->done();

	return 0;
}

