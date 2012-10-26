#include <boost/mpi.hpp>
#include <boost/serialization/map.hpp>
#include <boost/algorithm/string.hpp>


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
const int RISK_AIP_IDX = 3;


static const int countOfInputRecordValues  = 30;
static const int countOfRecordValues       = 46;

static const char * const recordValues[] = {
    // Input
    "run.number",
    "date_time.run",
    "run.time",
    "random.seed",
    "stop.at",
    "seek.and.destroy.at",
    "seek.and.destroy.cure.fraction",
    "persons.file",
    "places.file",
    "activities.file",
    "hourly.output.file",
    "yearly.output.file",
    "summary.output.file",
    "seek.care.fraction",
    "initial.colonization.fraction",
    "initial.infected.count",
    "minimum.infection.period",
    "a",
    "b",
    "c",
    "d",
    "e",
    "min.x",
    "min.y",
    "max.x",
    "max.y",
    "grid.buffer",
    "proc.per.x",
    "proc.per.y",
    "process.count",
    // Output
    "run.time",
    "total_infections",
    "colonizations",
    "total_from_infection",
    "total_from_colonization",
    "avg_infected_r0",
    "avg_colonized_r0",
    "infections_year_1",
    "infections_year_2",
    "infections_year_3",
    "infections_year_4",
    "infections_year_5"
    "colonizations_year_1",
    "colonizations_year_2",
    "colonizations_year_3",
    "colonizations_year_4",
    "colonizations_year_5"
}
;


void usage() {
	std::cerr << "usage: X  string string" << std::endl;
	std::cerr
			<< "  first string: string is the path to the Repast HPC \n\tconfiguration properties file"
			<< std::endl;
	std::cerr << "  second string: string is the path to the model properties file" << std::endl;
}

void setPropertiesForSweep(Properties& props, int sweepIndex){
  boost::mpi::communicator world;

  float aVals[3] = { 1.92e-05f, 2.59e-05f, 3.25e-05f};
  float bVals[3] = { 2.31e-05f, 2.58e-05f, 2.85e-04f};
  float eVals[3] = { 1.53e-05f, 2.40e-05f, 6.69e-05f};

  int c = 0;
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; i++){
      for(int k = 0; k < 3; k++)
      if(c == sweepIndex){
        // Set Properties here
        props.putProperty("a", aVals[i]);
        props.putProperty("b", bVals[j]);
        props.putProperty("e", eVals[k]);

        props.putProperty("stop.at", 8760.5);

        std::stringstream ss;
        ss << std::fixed << (time(NULL) + (world.rank() + 5) * 7327);
        props.putProperty("random.seed", ss.str());
        return;
      }
      c++;
    }
  }
}

void writePropertiesFromAllProcesses(Properties& props, std::string fileName, bool includeOutput = false){
  std::map<std::string, std::string> map;
  Properties::key_iterator keyIter = props.keys_begin();
  while(keyIter != props.keys_end()){
    map[*keyIter] = props.getProperty(*keyIter);
    keyIter++;
  }

  std::vector<std::map<std::string, std::string> > rec;
  boost::mpi::communicator world;
  boost::mpi::gather(world, map, rec, 0);

  std::vector<std::string> keysToWrite;
  int limit = (includeOutput ? countOfRecordValues : countOfInputRecordValues);
  for(int i = 0; i < limit; i++) keysToWrite.push_back(recordValues[i]);

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

  int sweepIndex = strToInt(props.getProperty("BaseSweepNumber")) + world.rank();
  props.putProperty("run.number", sweepIndex);
  setPropertiesForSweep(props, sweepIndex);

  // Create SUBCOMMUNICATORS for all ranks
  boost::mpi::communicator sub = world.split(world.rank());
  RepastProcess::init(config, &sub);

  // Write INPUT parameters (in case of crash)
  writePropertiesFromAllProcesses(props, "mrsa_model_INPUT.csv");

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
	  writePropertiesFromAllProcesses(props, "mrsa_model_OUTPUT.csv", true);
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

