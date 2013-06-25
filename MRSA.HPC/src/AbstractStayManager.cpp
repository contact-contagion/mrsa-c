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
 * AbstractStayManager.cpp
 *
 *  Created on: Mar 7, 2013
 *      Author: nick
 */

#include <iostream>
#include "AbstractStayManager.h"

namespace mrsa {

AbstractStayManager::AbstractStayManager(size_t years) :
		stay_data(years, std::make_pair(0u, 0u)), duration(0), start_day(0) {
}

AbstractStayManager::~AbstractStayManager() {
}

int count = 0;
bool AbstractStayManager::inPlace(int year, int day) {
	bool ret_val = false;

	if (duration > 0) {
		// currently in place. check if should leave.
		ret_val = (day - start_day) < duration;
		if (!ret_val) {
			duration = 0;
			start_day = 0;
		}
	} else {
		// year starts with 1
		StayData& data = stay_data.at(year - 1);
		ret_val = data.second > 0 && data.first == (unsigned int) day;
		if (ret_val) {
			start_day = day;
			duration = data.second;
		}
	}

	return ret_val;
}

} /* namespace mrsa */
