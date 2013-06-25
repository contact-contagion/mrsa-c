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
 * CompositePlace.h
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#ifndef COMPOSITEPLACE_H_
#define COMPOSITEPLACE_H_

#include "Place.h"
#include "AbstractPlace.h"

namespace mrsa {

enum CompPlaceType {
	PRISON = 0, GQ, GYM, SCHOOL
};

/**
 * A group of persons of the same age in a school. Transmission
 * occurs between members of an AgeGroup.
 */
class ComponentPlace: public AbstractPlace {

public:
	ComponentPlace(std::string id, std::string type, Risk risk, unsigned int zip);
	virtual ~ComponentPlace();

	/**
	 * Runs the transmission algorithm on the members of this age group.
	 */
	virtual void runTransmission();

	/**
	 * Gets the number of persons in this ComponentPlace.
	 */
	size_t size();

private:
	ComponentPlace(const ComponentPlace& other);
};

/**
 * A CompositePlace contains sub component places in which
 * the transmission actually occurs.
 */
class CompositePlace: public Place {

public:
	CompositePlace(std::vector<std::string>& vec, Risk risk, unsigned int component_max_size,
			CompPlaceType pType);
	virtual ~CompositePlace();

	void addPerson(Person* person, int activity_type);

	/**
	 * Adds a person to a component place in this CompositePlace.
	 */
	//int addPersonToComponent(Person* person, int activity_type, int compartment_index);

	/**
	 * Runs the mrsa transmission algorithm.
	 */
	void runTransmission();

	/**
	 * Resets this CompositePlace for the next iteration of the model.
	 */
	void reset();

	/**
	 * Gets the number of component places in this CompositePlace.
	 */
	size_t componentCount() {
		return components.size();
	}

	/**
	 * Gets the number of persons in the component place at the specified index.
	 *
	 * @return the number of persons in the component place at the specified index.
	 */
	size_t componentSize(size_t index);

	const CompPlaceType compPlaceType() const {
		return placeType_;
	}

private:
	std::vector<ComponentPlace*> components;
	unsigned int component_max_size_;
	CompPlaceType placeType_;
	size_t comp_index;
};

} /* namespace mrsa */
#endif /* COMPOSITEPLACE_H_ */
