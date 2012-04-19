#include <boost/mpi.hpp>

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

using namespace repast;
using namespace relogo;
using namespace mrsa;

void usage() {
	std::cerr << "usage: X  string string" << std::endl;
	std::cerr
			<< "  first string: string is the path to the Repast HPC \n\tconfiguration properties file"
			<< std::endl;
	std::cerr
			<< "  second string: string is the path to the model properties file"
			<< std::endl;
}

void runModel(std::string propsFile, int argc, char ** argv) {
	boost::mpi::communicator world;
	Properties props(propsFile, argc, argv, &world);

	std::string time;
	repast::timestamp(time);
	props.putProperty("date_time.run", time);

	props.putProperty("process.count", world.size());

	try {
		SimulationRunner runner(&world);

		if (world.rank() == 0)
			std::cout << " Starting... " << std::endl;
		repast::Timer timer;
		timer.start();
		runner.run<MRSAObserver, Patch>(props);

		props.putProperty("run.time", timer.stop());

		if (world.rank() == 0) {
			std::vector<std::string> keysToWrite;
			keysToWrite.push_back("run.number");
			keysToWrite.push_back("date_time.run");
			keysToWrite.push_back("process.count");
			keysToWrite.push_back("random.seed");
			keysToWrite.push_back("min.x");
			keysToWrite.push_back("min.y");
			keysToWrite.push_back("max.x");
			keysToWrite.push_back("max.y");
			keysToWrite.push_back("proc.per.x");
			keysToWrite.push_back("proc.per.y");
			keysToWrite.push_back("grid.buffer");
			keysToWrite.push_back("init.time");
			keysToWrite.push_back("stop.at");
			keysToWrite.push_back("run.time");
			props.log("root");
			props.writeToSVFile("mrsa_model.csv", keysToWrite);

		}
	}
	catch (std::exception& exp) {
		std::cerr << "ERROR: " << exp.what() << std::endl;
	}

}

int main(int argc, char **argv) {
	boost::mpi::environment env(argc, argv);
	std::string config, props;
	boost::mpi::communicator world;

	if (argc >= 3) {
		config = argv[1];
		props = argv[2];

	} else {
		if (world.rank() == 0)
			usage();
		return 0;
	}

	if (config.size() > 0 && props.size() > 0) {
		RepastProcess::init(config, &world);
		runModel(props, argc, argv);
	} else {
		if (world.rank() == 0)
			usage();
		return 0;
	}

	RepastProcess::instance()->done();

	return 0;
}
