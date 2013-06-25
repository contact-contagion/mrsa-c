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
 * SelectiveTransmissionPlace.h
 *
 *  Created on: Apr 27, 2012
 *      Author: nick
 */

#ifndef SELECTIVETRANSMISSIONPLACE_H_
#define SELECTIVETRANSMISSIONPLACE_H_

#include "AbstractPlace.h"

namespace mrsa {

/**
 * Place implementation that will run the transmision algorithm
 * on only a specified number of randomly selected uncolonized persons, rather
 * than all uncolonized persons.  The transmision state change for non-uncolonized
 * persons will happen for all of the non-uncolonized persons in this place
 */
class SelectiveTransmissionPlace: public AbstractPlace {

public:
	/**
	 * Creates a SelectiveTransmissionPlace from the specified vector of parameters with
	 * the specified risk and count of uncolonized persons to the algorithm on.
	 */
	SelectiveTransmissionPlace(std::vector<std::string>& vec, Risk risk, int uncp_count);
	virtual ~SelectiveTransmissionPlace();


	/**
	 * Runs the SelectiveTransmissionPlace transmission algorithm.
	 */
	virtual void runTransmission();


private:
	unsigned int uncp_count;
};

} /* namespace mrsa */
#endif /* SELECTIVETRANSMISSIONPLACE_H_ */
