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
 * constants.h
 *
 *  Created on: May 9, 2012
 *      Author: nick
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>

namespace mrsa {

// the type is lower-cased before the compare
// so these are OK, even though they don't match
// the case of those in the data files.
const std::string SCHOOL_TYPE = "school";
const std::string WORKPLACE_TYPE = "workplace";
const std::string HOUSEHOLD_TYPE = "household";
const std::string OTHER_HOUSEHOLD_TYPE = "other household";
const std::string GYM_TYPE = "gym";
const std::string HOSPITAL_TYPE = "hospital";
const std::string DORM_TYPE = "college dorm";
const std::string NURSING_HOME_TYPE = "nursing home";
const std::string PRISON_TYPE = "prison";

// data property headings
const std::string HOUSEHOLD_COL_COUNT = "household_colonizations";
const std::string OTHER_H_COL_COUNT ="other_household_colonizations";
const std::string WORKPLACE_COL_COUNT ="workplace_colonizations";
const std::string SCHOOL_COL_COUNT ="school_colonizations";
const std::string GYM_COL_COUNT ="sports_facilities_colonizations";
const std::string DORM_COL_COUNT ="college_dorm_colonizations";
const std::string NURSING_HOME_COL_COUNT ="nursing_home_colonizations";

// name of the random dist used to draw other households.
const std::string OH_DIST = "other.household.random.dist";
// name of distribution used to draw hospital stay length from
const std::string HOSPITAL_STAY_DURATION = "hospital.stay.duration";

const double YEAR_START = 4380.0;



}

#endif /* CONSTANTS_H_ */
