#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN main

#include <boost/test/unit_test.hpp>
#include <boost/mpi.hpp>

#include "repast_hpc/RepastProcess.h"

// global fixture
struct MyConfig {

	boost::mpi::environment env;
	boost::mpi::communicator world;

	MyConfig() : env(true),  world() {
		std::cout << "My Config" << std::endl;
		repast::RepastProcess::init("../config/config.props", &world);
	}

	virtual ~MyConfig() {
		std::cout << "~MyConfig" << std::endl;
		repast::RepastProcess::instance()->done();
	}
};


BOOST_GLOBAL_FIXTURE(MyConfig);
