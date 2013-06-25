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
 * Place.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include "Place.h"
#include "repast_hpc/Utilities.h"

namespace mrsa {

using namespace repast;

const int PLACE_ID_IDX = 0;
const int PLACE_TYPE_IDX = 1;
const int ZIP_IDX = 6;
// skip name and persons as we don't use them
// in the model.
//const int NAME_IDX = 2;
//const int PERSONS_IDX = 3;
const int LAT_IDX = 4;
const int LON_IDX = 5;

// creates a Place from the data in the vector and with the specified risk.
Place::Place(std::vector<std::string>& vec, Risk risk) : id_(vec[PLACE_ID_IDX]), type_(vec[PLACE_TYPE_IDX]),
		longitude(0), latitude(0), risk_(risk), zip_code(strToUInt(trim(vec[ZIP_IDX]))) {

	std::string val = vec[LAT_IDX];
	val = trim(val);
	if (val.length() > 0)
		latitude = strToDouble(val);

	val = vec[LON_IDX];
	val = trim(val);
	if (val.length() > 0)
		longitude = strToDouble(val);
}

Place::Place(std::string id, std::string type, Risk risk, unsigned int zip) : id_(id), type_(type), longitude(0), latitude(0), risk_(risk),
		zip_code(zip) {}

Place::~Place() {
}

std::ostream& operator<<(std::ostream& os, const Place& place) {
	os << "Place[" << place.id_ << ", " << place.type_ << "]";
	return os;
}

} /* namespace mrsa */
