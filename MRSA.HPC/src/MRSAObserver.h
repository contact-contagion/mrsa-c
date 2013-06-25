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
 * MRSAObserver.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef MRSAOBSERVER_H_
#define MRSAOBSERVER_H_

#include "relogo/Observer.h"
#include "repast_hpc/Properties.h"

#include "Place.h"
#include "Person.h"
#include "Statistics.h"

namespace mrsa {

/**
 * The Observer class for the MRSA model.
 */
class MRSAObserver : public repast::relogo::Observer {

public:
	MRSAObserver();
	virtual ~MRSAObserver();

	/**
	 * Called every tick of the simulation. This makes each person
	 * perform their current activity, and then performs the disease
	 * transmission based on the places the persons moved to.
	 */
	void go();

	/**
	 * Performs some additional final statistics collection
	 * when called at the end of the simluation.
	 */
	void atEnd();

	/**
	 * Sets up the simulation for a run, given the specified properties.
	 */
	virtual void setup(repast::Properties& props);

	/**
	 * Calculates the yearly stats.
	 */
	void calcYearlyStats();

	/**
	 * Turns seek and destroy behavior on.
	 */
	void activateSeekAndDestroy();

private:

	typedef repast::Context<repast::relogo::RelogoAgent>::const_bytype_iterator AgentIter;

	int personType;

	std::vector<Place*> places;
	//repast::relogo::AgentSet<Person>* people_;
	std::string summary_output_file;
	Calendar calendar;

	repast::Properties* propsPtr;
	int yearCounter;
	unsigned long long person_count;

	/**
	 * Load the activities and assign them to persons.
	 */
	void initializeActivities(repast::Properties& props);

	/**
	 * Setup the hourly data collection.
	 *
	 * @param file the file to write the hourly output to
	 */
	void initializeHourlyDataCollection(const std::string& file);

	/**
	 * Setup the yearly data collection.
	 *
	 * @param file the file to write yearly output to
	 */
	void initializeYearlyDataCollection(const std::string& file);

	/**
	 * Creates the persons from the persons file.
	 */
	void createPersons(repast::Properties& props, std::map<std::string, Place*>* placeMap);

	/**
	 * Creates the places from the places file.
	 */
	void createPlaces(repast::Properties& props, std::map<std::string, Place*>* placeMap);

};

} /* namespace mrsa */
#endif /* MRSAOBSERVER_H_ */
