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


using namespace repast;
using namespace relogo;
using namespace mrsa;

const int TYPE_IDX = 1;
const int RISK_PLACE_TYPE_IDX = 0;
const int RISK_ACT_TYPE_IDX = 1;
const int RISK_PAR_IDX = 2;
const int RISK_TIP_IDX = 3;
const int RISK_AIP_IDX = 4;


static const int countOfInputRecordValues  = 30;
static const int countOfRecordValues       = 46;

using namespace boost::assign;

void usage() {
	std::cerr << "usage: X  string string" << std::endl;
	std::cerr
			<< "  first string: string is the path to the Repast HPC \n\tconfiguration properties file"
			<< std::endl;
	std::cerr << "  second string: string is the path to the model properties file" << std::endl;
}

void setPropertiesForSweep(Properties& props, int sweepIndex){
  boost::mpi::communicator world;

  srand((int)(time(NULL) * world.rank()));
  for(int i = 0; i < world.rank(); i++) rand();
  std::stringstream ss;
  ss << std::fixed << ((world.rank()+ 1) * (rand() % 3727));
  props.putProperty("random.seed", ss.str());

  std::vector<float> aVals, bVals, eVals;
  aVals += 2.05e-05f, 2.18e-05f, 2.31e-05f, 2.44e-05f;
  bVals += 2.38e-05f, 2.45e-05f, 2.52e-05f, 2.65e-05f, 2.72e-05f, 2.79e-05f, 3.40e-05f, 4.00e-05f;
  eVals += 3.26e-05f, 4.12e-05f, 4.98e-05f, 5.84e-05;

  int c = 0;
  for(size_t i = 0; i < aVals.size(); i++){
    for(size_t j = 0; j < bVals.size(); j++){
      for(size_t k = 0; k < eVals.size(); k++){
        if(c == sweepIndex){
          // Set Properties here
          props.putProperty("a", aVals[i]);
          props.putProperty("b", bVals[j]);
          props.putProperty("e", eVals[k]);
          return;
        }
        c++;
      }
    }
  }
}

void getKeysToWrite(std::vector<std::string>& keylist, bool output = false){
  if(!output){
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
        "seek.care.fraction",
        "initial.colonization.fraction",
        "initial.infected.count",
        "minimum.infection.period",
        "a", "b", "e",

        // Risks:
        "Hospital_0_PAR",     "Hospital_0_TIP",     "Hospital_0_AIP",
        "Hospital_1_PAR",     "Hospital_1_TIP",     "Hospital_1_AIP",

        "Workplace_0_PAR",    "Workplace_0_TIP",    "Workplace_0_AIP",
        "Workplace_1_PAR",    "Workplace_1_TIP",    "Workplace_1_AIP",

        "Household_0_PAR",    "Household_0_TIP",    "Household_0_AIP",
        "Household_1_PAR",    "Household_1_TIP",    "Household_1_AIP",

        "School_0_PAR",       "School_0_TIP",       "School_0_AIP",
        "School_1_PAR",       "School_1_TIP",       "School_1_AIP",

        "Gym_0_PAR",          "Gym_0_TIP",          "Gym_0_AIP",
        "Gym_1_PAR",          "Gym_1_TIP",          "Gym_1_AIP",

        "Nursing_Home_0_PAR", "Nursing_Home_0_TIP", "Nursing_Home_0_AIP",
        "Nursing_Home_1_PAR", "Nursing_Home_1_TIP", "Nursing_Home_1_AIP",

        "Prison_0_PAR",       "Prison_0_TIP",       "Prison_0_AIP",
        "Prison_1_PAR",       "Prison_1_TIP",       "Prison_1_AIP",

        "College_Dorm_0_PAR", "College_Dorm_0_TIP", "College_Dorm_0_AIP",
        "College_Dorm_1_PAR", "College_Dorm_1_TIP", "College_Dorm_1_AIP",


        // Spatial for ReLogo
        "min.x", "min.y", "max.x", "max.y",
        "grid.buffer", "proc.per.x", "proc.per.y";
  }
  else{
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
        "seek.care.fraction",
        "initial.colonization.fraction",
        "initial.infected.count",
        "minimum.infection.period",
        "a", "b", "e",

        // Risks:
        "Hospital_0_PAR",     "Hospital_0_TIP",     "Hospital_0_AIP",
        "Hospital_1_PAR",     "Hospital_1_TIP",     "Hospital_1_AIP",

        "Workplace_0_PAR",    "Workplace_0_TIP",    "Workplace_0_AIP",
        "Workplace_1_PAR",    "Workplace_1_TIP",    "Workplace_1_AIP",

        "Household_0_PAR",    "Household_0_TIP",    "Household_0_AIP",
        "Household_1_PAR",    "Household_1_TIP",    "Household_1_AIP",

        "School_0_PAR",       "School_0_TIP",       "School_0_AIP",
        "School_1_PAR",       "School_1_TIP",       "School_1_AIP",

        "Gym_0_PAR",          "Gym_0_TIP",          "Gym_0_AIP",
        "Gym_1_PAR",          "Gym_1_TIP",          "Gym_1_AIP",

        "Nursing_Home_0_PAR", "Nursing_Home_0_TIP", "Nursing_Home_0_AIP",
        "Nursing_Home_1_PAR", "Nursing_Home_1_TIP", "Nursing_Home_1_AIP",

        "Prison_0_PAR",       "Prison_0_TIP",       "Prison_0_AIP",
        "Prison_1_PAR",       "Prison_1_TIP",       "Prison_1_AIP",

        "College_Dorm_0_PAR", "College_Dorm_0_TIP", "College_Dorm_0_AIP",
        "College_Dorm_1_PAR", "College_Dorm_1_TIP", "College_Dorm_1_AIP",

        // Spatial for ReLogo
        "min.x", "min.y", "max.x", "max.y",
        "grid.buffer", "proc.per.x", "proc.per.y",

        // Output
        "process.count",
        "run.time",

        // Summary Statistics
        "total_infections",
        "colonizations",
        "total_from_infection",
        "total_from_colonization",
        "avg_infected_r0",
        "avg_colonized_r0",

        // By Year Stats
        "infections_year_1", "infections_year_2", "infections_year_3", "infections_year_4", "infections_year_5",
        "colonizations_year_1", "colonizations_year_2", "colonizations_year_3", "colonizations_year_4", "colonizations_year_5",
        "new_infections_year_1", "new_infections_year_2", "new_infections_year_3", "new_infections_year_4", "new_infections_year_5",
        "new_colonizations_year_1", "new_colonizations_year_2", "new_colonizations_year_3", "new_colonizations_year_4", "new_colonizations_year_5";
  }
}

void writePropertiesFromAllProcesses(Properties& props, std::string fileName, bool includeOutput = false){
  std::map<std::string, std::string> map;
  Properties::key_iterator keyIter = props.keys_begin();
  while(keyIter != props.keys_end()){
    map[*keyIter] = props.getProperty(*keyIter);
    keyIter++;
  }

  boost::mpi::communicator world;
  if(world.rank() != 0){
    boost::mpi::gather(world, map, 0);
  }
  else{
    std::vector<std::map<std::string, std::string> > rec;
    boost::mpi::gather(world, map, rec, 0);

    std::vector<std::string> keysToWrite;
    getKeysToWrite(keysToWrite, includeOutput);
    for(size_t i = 0; i < rec.size(); i++){
      Properties outProps;
      std::map<std::string, std::string> recdMap = rec[i];
      for(std::map<std::string, std::string>::iterator iter = recdMap.begin(); iter != recdMap.end(); iter++){
        outProps.putProperty(iter->first, iter->second);
      }
      outProps.log("root");
      outProps.writeToSVFile(fileName, keysToWrite);
    }
  }
}


void loadRisks(Properties& props) {
  if(!props.contains("risk.file")) return;
  std::string risk_file = props.getProperty("risk.file");
  CSVReader reader(risk_file);
  std::vector<std::string> vec;
  std::set<std::string> placeNamesSet;
  std::stringstream placeNames;

  // skip the first header line.
  reader.next(vec);

  // read each line and depending on the type, create that type of place.
  while (reader.next(vec)) {
    std::string type = vec[RISK_PLACE_TYPE_IDX];
    if((placeNamesSet.insert(type).second)){
      if(placeNames.str().size() > 0) placeNames << ",";
      placeNames << type;
    }

    boost::replace_all(type, " ", "_");

    int act_type = repast::strToInt(vec[RISK_ACT_TYPE_IDX]);

    std::stringstream PAR;
    PAR << type << "_" << act_type << "_PAR";
    props.putProperty(PAR.str(), (float) repast::strToDouble(vec[RISK_PAR_IDX]));

    std::stringstream TIP;
    TIP << type << "_" << act_type << "_TIP";
    props.putProperty(TIP.str(), (float) repast::strToDouble(vec[RISK_TIP_IDX]));

    std::stringstream AIP;
    AIP << type << "_" << act_type << "_AIP";
    props.putProperty(AIP.str(), (float) repast::strToDouble(vec[RISK_AIP_IDX]));

    props.putProperty("place.types", placeNames.str());

  }

  /*
  for (std::map<string, Risk>::iterator iter = map.begin(); iter != map.end(); ++iter) {
    Risk& risk = iter->second;
    std::cout << iter->first << ": a0 = " << risk.a0_ << ", a1 = " << risk.a1_ << ", b0 = "
        << risk.b0_  << ", b1= " << risk.b1_ << std::endl;
  }
  */
}

/**
 * Runs the MRSA model with the specified model.props file and
 * optional model prop command line values.
 */
void runModel(std::string propsFile, std::string config, int argc, char ** argv) {
	boost::mpi::communicator world;
	// create a properties object from the props file.
	Properties props(propsFile, argc, argv, &world);

	loadRisks(props);

	// add a starting time property
	std::string time;
	repast::timestamp(time);
	props.putProperty("date_time.run", time);

	// add process count property
	props.putProperty("process.count", world.size());

	boost::mpi::communicator sub;
	if(props.contains("do.parameter.sweeps")){
    int sweepIndex = world.rank();
    if(props.contains("base.sweep.number")) sweepIndex += strToInt(props.getProperty("base.sweep.number"));

    // If a 'run.number' is provided, use it.
    if(!props.contains("run.number")){
      // If not, calculate from sweep index
      // If a 'base.run.number' is provided, start from there, otherwise start from 0
      int baseRunNumber = 0;
      if(props.contains("base.run.number")) baseRunNumber = strToInt(props.getProperty("base.run.number"));
      props.putProperty("run.number", baseRunNumber + sweepIndex);
    }

    // Set properties specific to this run
    setPropertiesForSweep(props, sweepIndex);
    // Create SUBCOMMUNICATORS for all ranks
    sub = world.split(world.rank());
	}

	if(!props.contains("run.number")) props.putProperty("run.number", -1);
	RepastProcess::init(config, &sub);

  // Write INPUT parameters (in case of crash)
	std::string inputRecordLocation =
	    (props.contains("input.record.location") ? props.getProperty("input.record.location") : "output/mrsa_model_INPUT.csv");
  writePropertiesFromAllProcesses(props, inputRecordLocation);



	try {
		// create a simulation runner to run the MRSA model.
		SimulationRunner runner(&sub);

		if (sub.rank() == 0) std::cout << " Starting on rank: " << world.rank() << std::endl;

		// start timer to measure how long the model takes to run
		repast::Timer timer;
		timer.start();
		// run the model
		runner.run<MRSAObserver, Patch>(props);
		if (sub.rank() == 0) std::cout << " Done on rank: " << world.rank() << std::endl;

		// model has now finished so add the stop time to the properties
		props.putProperty("run.time", timer.stop());

		// Write out properties and output
		std::string outputRecordLocation =
		      (props.contains("output.record.location") ? props.getProperty("output.record.location") : "output/mrsa_model_OUTPUT.csv");
	  writePropertiesFromAllProcesses(props, outputRecordLocation, true);
	}
	catch (std::exception& exp) {
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

