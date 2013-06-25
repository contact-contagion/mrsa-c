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
 * Activity.h
 *
 *  Created on: Apr 18, 2012
 *      Author: nick
 */

#ifndef ACTIVITY_H_
#define ACTIVITY_H_

#include <vector>
#include <string>
#include <iostream>

#include "Places.h"

namespace mrsa {

/**
 * Names a places that a person will go to a particular time, together
 * with the disease risk associated with that going there.
 */
class Activity {

public:
	Activity(std::vector<std::string>& vec);
	// copy constructor
	Activity(const Activity& act);
	virtual ~Activity();

	/**
	 * Gets the activity type associated with this activity.
	 */
	int activityType() const {
		return activity_type;
	}

	/**
	 * Gets the id of this Activity.
	 */
	const std::string& id() const {
		return tucase_id;
	}

	/**
	 * Gets the place type of this activity.
	 */
	const std::string& getPlaceType() const {
		return place_type;
	}

	/**
	 * Gets whether or not the time space of this activity
	 * contains the specified time.
	 *
	 * @return true if it is does contain this time, otherwise false.
	 */
	bool contains(int time) const {
		//std::cout << place_type << ": " << start_time << " - " << end_time << std::endl;
		return start_time <= time && end_time > time;
	}

	/**
	 * Selects the place appropriate to this activity from the Places.
	 *
	 * @return the place appropriate to this activity from the Places.
	 */
	Place* selectPlace(Places& places) const;

	// assignment operator
	Activity& operator=(const Activity&);

private:
	std::string tucase_id, place_type;
	int start_time, end_time;
	int activity_type;

};

} /* namespace mrsa */
#endif /* ACTIVITY_H_ */
