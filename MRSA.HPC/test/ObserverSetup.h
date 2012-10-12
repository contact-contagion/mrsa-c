/*
 * ObserverSetup.h
 *
 *  Created on: Apr 26, 2012
 *      Author: nick
 */

#ifndef OBSERVERSETUP_H_
#define OBSERVERSETUP_H_

#include <boost/mpi.hpp>

#include "relogo/Observer.h"
#include "relogo/WorldCreator.h"
#include "relogo/Patch.h"
#include "repast_hpc/Properties.h"

#include "../src/Parameters.h"

class MyObserver: public repast::relogo::Observer {
	void go() {
	}

	void setUp() {
	}
};

class ObserverSetup {

public:
	repast::relogo::Observer* obs;

public:
	ObserverSetup() {

		repast::relogo::WorldDefinition def(0, 0, 10, 10, true, 0);
		repast::relogo::WorldCreator creator(repast::RepastProcess::instance()->communicator());
		std::vector<int> vec;
		vec.push_back(1);
		vec.push_back(1);
		obs = creator.createWorld<MyObserver, repast::relogo::Patch>(def, vec);
		repast::Properties props;
		try {
			// this will throw an exception if we try to reinit
			// so we can ignore that.
			mrsa::Parameters::initialize(props);
		} catch (std::exception& exp) {}
		obs->_setup(props);
	}

	~ObserverSetup() {
		delete obs;
	}
};



#endif /* OBSERVERSETUP_H_ */
