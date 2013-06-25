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
 * PersonsCreator.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef PERSONSCREATOR_H_
#define PERSONSCREATOR_H_

#include <string>
#include <map>

#include "relogo/Observer.h"

#include "CSVReader.h"
#include "Person.h"
#include "Place.h"
#include "NoStayManager.h"

namespace mrsa {

extern const std::string JAIL_DISTRIBUTION;

/**
 * Returns the column index that contains the initail mrsa
 * infection probabilities.
 */
int get_mrsa_prob_idx();

/**
 * Functor that is to create Persons agents.
 *
 */
class PersonsCreator {
public:
	/**
	 * Creates a PersonCreator that will create Persons from the
	 * specified file and using the specified map to
	 * look up the Persons' places.
	 */
	PersonsCreator(const std::string& file, std::map<std::string, Place*>* map, float min_infection_duration,
			unsigned int min_jail_duration, double p_mrsa_sum);
	PersonsCreator(const PersonsCreator& creator);
	virtual ~PersonsCreator();

	/**
	 * Creates a Person given the id and observer.
	 */
	Person* operator()(repast::AgentId id, repast::relogo::Observer* obs);

private:
	CSVReader reader;
	std::map<std::string, Place*>* places;
	float min_infection_duration_;
	unsigned int min_jail_duration_, max_jail_duration_, peak_jail_duration_;
	unsigned int initial_infection_count;
	double colonization_scaling, p_mrsa_sum_;
	boost::shared_ptr<PlaceStayManager> no_stay_manager;

	/**
	 * Finds the named place.
	 */
	Place* findPlace(const std::string id);

	/**
	 * Performs initialization common to the constructors.
	 */
	void init();
};

} /* namespace mrsa */
#endif /* PERSONSCREATOR_H_ */
