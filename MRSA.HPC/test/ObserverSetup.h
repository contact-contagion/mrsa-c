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
