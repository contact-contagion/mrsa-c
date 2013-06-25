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
 * CompositePlace.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#include "CompositePlace.h"

namespace mrsa {

using namespace std;

ComponentPlace::ComponentPlace(string id, string type, Risk risk, unsigned int zip) :
		AbstractPlace(id, type, risk, zip) {

}

ComponentPlace::~ComponentPlace() {
}

void ComponentPlace::runTransmission() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	// uncolonized must always go first so that the infected and colonized persons
	// if they cause a colonization of the uncolonized are still in that infected or
	// colonized state.
	for (PersonIter iter = uncolonized.begin(); iter != uncolonized.end(); ++iter) {
		processUncolonized(*iter, ta);
	}
	for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
		processInfected(*iter, ta);
	}

	for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
		processColonized(*iter, ta);
	}
}

size_t ComponentPlace::size() {
	return uncolonized.size() + infected.size() + colonized.size();
}

CompositePlace::CompositePlace(std::vector<std::string>& vec, Risk risk,
		unsigned int component_max_size, CompPlaceType pType) :
		Place(vec, risk), components(), component_max_size_(component_max_size), placeType_(pType), comp_index(
				0) {

	// add an initial component.
	components.push_back(new ComponentPlace(id_, type_, risk_, zip_code));
}

CompositePlace::~CompositePlace() {
	// delete the component places.
	for (vector<ComponentPlace*>::iterator iter = components.begin(); iter != components.end();
			++iter) {
		delete (*iter);
	}
}

void CompositePlace::runTransmission() {
	int i = 1;
	for (vector<ComponentPlace*>::iterator iter = components.begin(); iter != components.end();
			++iter) {
		(*iter)->runTransmission();

		//stringstream ss;
		//ss << placeId() << "_" << i;
		//(*iter)->writeContactEvent(ss.str());
		//++i;
	}
}

void CompositePlace::addPerson(Person* person, int activity_type) {
	ComponentPlace* place = components[comp_index];
	place->addPerson(person, activity_type);

	if (place->size() == component_max_size_) {
		++comp_index;
		if (comp_index == components.size())
			components.push_back(new ComponentPlace(id_, type_, risk_, zip_code));
	}
}

void CompositePlace::reset() {
	for (vector<ComponentPlace*>::iterator iter = components.begin(); iter != components.end();
			++iter) {
		(*iter)->reset();
	}
	comp_index = 0;
}

/**
 * Gets the number of persons in the component place at the specified index.
 *
 * @return the number of persons in the component place at the specified index.
 */
size_t CompositePlace::componentSize(size_t index) {
	return components.at(index)->size();
}

} /* namespace mrsa */
