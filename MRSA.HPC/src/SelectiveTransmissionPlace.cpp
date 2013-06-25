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
 * SelectiveTransmissionPlace.cpp
 *
 *  Created on: Apr 27, 2012
 *      Author: nick
 */

#include "SelectiveTransmissionPlace.h"

namespace mrsa {

SelectiveTransmissionPlace::SelectiveTransmissionPlace(std::vector<std::string>& vec, Risk risk,
		int uncolonized_count) :
		AbstractPlace(vec, risk), uncp_count(uncolonized_count) {
}

SelectiveTransmissionPlace::~SelectiveTransmissionPlace() {
}

// runs the transmission algorithm on the persons in this place.
void SelectiveTransmissionPlace::runTransmission() {
	//writeContactEvent(placeId());

	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	// uncolonized should always go first so that if colonized or infected
	// cause a colonization they are still in that state when the uncolonized
	// to colonized transmission is attributed them.

	// if the number of uncolonized persons is less than the number of persons to randomly
	// select for transmision, then run the transmission algorithm on all of them.
	if (uncolonized.size() <= uncp_count) {
		for (PersonIter iter = uncolonized.begin(); iter != uncolonized.end(); ++iter) {
			processUncolonized(*iter, ta);
		}

	} else {
		// choose "uncp_count" persons at random and run the transmission algorithm
		// on them.
		// vector of persons already processed. This is likely to be a small number
		// so a std::set would be overkill.
		std::vector<Person*> processed;
		// create a random number generator to generate an int between 0 and the number of persons
		// - 1. This will be used an index into the uncolonized persons vector.
		repast::IntUniformGenerator gen = repast::Random::instance()->createUniIntGenerator(0,
				(int) uncolonized.size() - 1);
		// for "uncp_count" times:
		//std::cout << uncolonized.size() << std::endl;
		//std::cout << "uncp_count: " << uncp_count << std::endl;
		//for (int i = 0; i < uncolonized.size(); i++) {
		//	std::cout << "i: " << i << " " << uncolonized[i] << " " << (*uncolonized[i]) << std::endl;
		//}
		for (unsigned int i = 0; i < uncp_count; ++i) {
			// get a person at random
			Person* person = uncolonized[(int) gen.next()];
			// see if that person has already been processed, if so then
			// do the draw again, until we get an unprocessed person.
			PersonIter iter = std::find(processed.begin(), processed.end(), person);
			while (iter != processed.end()) {
				int idx = (int)gen.next();
				person = uncolonized[idx];
				iter = std::find(processed.begin(), processed.end(), person);
			}


			// process (run the transmission algorithm on) the person.
			processUncolonized(person, ta);
			// add the person to the vector of persons who have been processed.
			processed.push_back(person);
		}
	}

	// run the algorithm on the non-uncolonized persons, so they can transition from
	// colonized to infected etc.
	for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
		processColonized(*iter, ta);
	}

	for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
		processInfected(*iter, ta);
	}

}

} /* namespace mrsa */
