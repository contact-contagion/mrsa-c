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
# MRSA Model
# 
# Copyright (c) 2012 University of Chicago and Argonne National Laboratory
#    All rights reserved.
#   
#    Redistribution and use in source and binary forms, with 
#    or without modification, are permitted provided that the following 
#    conditions are met:
#   
#   	 Redistributions of source code must retain the above copyright notice,
#   	 this list of conditions and the following disclaimer.
#   
#   	 Redistributions in binary form must reproduce the above copyright notice,
#   	 this list of conditions and the following disclaimer in the documentation
#   	 and/or other materials provided with the distribution.
#   
#   	 Neither the name of the Argonne National Laboratory nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#   
#    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#    ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
#    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
#    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*
 * HospitalStayManager.cpp
 *
 *  Created on: Oct 24, 2012
 *      Author: nick
 */

#include <utility>
#include <repast_hpc/Random.h>

#include "HospitalStayManager.h"

namespace mrsa {

HospitalStayManager::HospitalStayManager(unsigned int y1_length, unsigned int y2_length,
		unsigned int y3_length, unsigned int y4_length, unsigned int y5_length) :
		AbstractStayManager(11) {

	unsigned int start = y1_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
			365 - y1_length).next() : 1;
	if (y1_length > 364) y1_length = 365;
	stay_data[0] = (std::pair<unsigned int, unsigned int>(start, y1_length));

	if (y2_length > 364) y2_length = 365;
	start = y2_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y2_length).next() : 1;
	stay_data[1] = (std::pair<unsigned int, unsigned int>(start, y2_length));

	start = y3_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y3_length).next() : 1;
	if (y3_length > 364) y3_length = 365;
	stay_data[2] = (std::pair<unsigned int, unsigned int>(start, y3_length));

	start = y4_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y4_length).next() : 1;
	if (y4_length > 364) y4_length = 365;
	stay_data[3] =(std::pair<unsigned int, unsigned int>(start, y4_length));

	start = y5_length < 364 ? (unsigned int) repast::Random::instance()->createUniIntGenerator(1,
				365 - y5_length).next() : 1;
	if (y5_length > 364) y5_length = 365;
	stay_data[4] =(std::pair<unsigned int, unsigned int>(start, y5_length));

	// repeat 5 - 9
	stay_data[5] = stay_data[0];
	stay_data[6] = stay_data[1];
	stay_data[7] = stay_data[2];
	stay_data[8] = stay_data[3];
	stay_data[9] = stay_data[4];
}

HospitalStayManager::~HospitalStayManager() {
}


} /* namespace mrsa */
