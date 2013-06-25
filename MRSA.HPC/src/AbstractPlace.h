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
 * AbstractPlace.h
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#ifndef ABSTRACTPLACE_H_
#define ABSTRACTPLACE_H_

#include "Place.h"
#include "Person.h"
#include "TransmissionAlgorithm.h"

namespace mrsa {

/**
 * Abstract base implementation of a Place that
 * accumulates the disease status counts for Persons
 * in this place using the addPerson method.
 */
class AbstractPlace: public Place {

public:
	AbstractPlace(std::vector<std::string>& vec, Risk risk);
	AbstractPlace(std::string id, std::string type, Risk risk, unsigned int zip);
	virtual ~AbstractPlace();

	/**
	 * Adds a person to this place.
	 *
	 * @param person the person to add.
	 */
	virtual void addPerson(Person* person, int activity_type);

	/**
	 * Runs the transmission algorithm appropriate to this place.
	 */
	virtual void runTransmission() = 0;

	/**
	 * Resets this place for the next iteration of the model. Typically
	 * this will set the counter to 0 and remove all the persons
	 * from the this place.
	 */
	virtual void reset();

	/**
	 * Write contact event
	 */
	void writeContactEvent(const std::string& place_type);


protected:
	typedef std::vector<Person*>::iterator PersonIter;
	// number of colonized, and infected persons in this
	// place.
	int activity_type_;
	std::vector<Person*> infected, colonized, uncolonized;

	/**
	 * Process the specifed infected person for transmission.
	 */
	void processInfected(Person* person, TransmissionAlgorithm* ta);

	/**
	 * Process the specified colonized person for transmission.
	 */
	void processColonized(Person* person, TransmissionAlgorithm* ta);

	/**
	 * Process the specified uncolonized person for transmission.
	 */
	void processUncolonized(Person* person, TransmissionAlgorithm* ta);

};

} /* namespace mrsa */
#endif /* ABSTRACTPLACE_H_ */
