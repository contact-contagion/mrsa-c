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
 * Activity.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: nick
 */
#include "repast_hpc/Utilities.h"
#include "repast_hpc/Random.h"

#include "Activity.h"
#include "Constants.h"

namespace mrsa {

using namespace repast;

const int ID_IDX = 0;
const int PLACE_TYPE_IDX = 1;
const int START_TIME_IDX = 2;
const int STOP_TIME_IDX = 3;
const int ACTIVITY_TYPE_IDX = 4;

// initializes the activities fields from the vector.
Activity::Activity(std::vector<std::string>& vec) :
		tucase_id(vec[ID_IDX]), place_type(vec[PLACE_TYPE_IDX]), start_time(0), end_time(0), activity_type(
				0) {

	std::string val = vec[START_TIME_IDX];
	val = trim(val);
	// minutes to hours
	start_time = strToInt(val) / 60;

	val = vec[STOP_TIME_IDX];
	val = trim(val);
	// minutes to hours
	end_time = strToInt(val) / 60;

	val = vec[ACTIVITY_TYPE_IDX];
	val = trim(val);
	activity_type = strToInt(val);
}

// copy constructor
Activity::Activity(const Activity& act) :
		tucase_id(act.tucase_id), place_type(act.place_type), start_time(act.start_time), end_time(
				act.end_time), activity_type(act.activity_type) {
}

Place* Activity::selectPlace(Places& places) const {
	if (place_type == "Household") {
		if (places.household != 0) return places.household;
		return places.group_quarters;
	}
	else if (place_type == "Workplace") return places.work;

	else if (place_type == "Other Household") {
		//std::cout << "going to other household" << std::endl;
		NumberGenerator* gen = repast::Random::instance()->getGenerator(OH_DIST);
		//std::cout << gen << std::endl;
		//std::cout << places_.other_households.size() << std::endl;
		Place* place = places.other_households[(int) gen->next()];
		// one of the "other households" may be 0 if we are running
		// on a subset of the data
		if (place == 0) {
			if (places.household != 0) place = places.household;
			else place = places.group_quarters;
		}
		return place;
	}
	else if (place_type == "School") {
		if (places.school != 0) return places.school;
		else return places.daycare;
	}
	else if (place_type == "Group Quarters") return places.group_quarters;

	else if (place_type == "Gym") return places.gym;
	else if (place_type == "Hospital") return places.hospital;
	else if (place_type == "Prison") return places.jail;
	else {
		// unknown place type so go to household if it exists or if not
		// go to group quarters.
		if (places.household != 0) return places.household;
		else return places.group_quarters;
	}
	return 0;
}

// assignment operator
Activity& Activity::operator=(const Activity& rhs) {
	if (&rhs != this) {
		this->tucase_id = rhs.tucase_id;
		this->place_type = rhs.place_type;
		this->start_time = rhs.start_time;
		this->end_time = rhs.end_time;
		this->activity_type = rhs.activity_type;
	}

	return *this;
}

Activity::~Activity() {
}

} /* namespace mrsa */
