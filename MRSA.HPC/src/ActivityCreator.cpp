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
 * ActivityCreator.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: nick
 */

#include "ActivityCreator.h"
#include "CSVReader.h"

namespace mrsa {

using namespace std;

ActivityCreator::ActivityCreator() {
}

ActivityCreator::~ActivityCreator() {
}

void ActivityCreator::run(const std::string& file, std::map<std::string, std::vector<Activity> >& map) {
	typedef  std::map<std::string, std::vector<Activity> >::iterator MapIter;

	// use a CSVReader to read the file.
	CSVReader reader(file);
	vector<string> vec;
	// skip the first line
	reader.next(vec);
	// read each row in the file into vec
	while (reader.next(vec)) {
		// create an activity from that row
		Activity activity(vec);
		// find the map entry with that tucaseid
		// if it doesn't exist make a vector inser that into the map
		MapIter iter = map.find(activity.id());
		if (iter == map.end()) {
			vector<Activity> activities;
			activities.push_back(activity);
			map.insert(pair<string, vector<Activity> >(activity.id(), activities));
		} else {
			// vector with this tucaseid does exist so get a reference to it
			// and put that activity in it.
			vector<Activity>& activities = iter->second;
			activities.push_back(activity);
		}
	}
}

} /* namespace mrsa */
