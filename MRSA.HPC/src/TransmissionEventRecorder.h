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
 * TransmissionEventRecorder.h
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#ifndef TRANSMISSIONEVENTRECORDER_H_
#define TRANSMISSIONEVENTRECORDER_H_

#include <vector>
#include <string>
#include <fstream>

#include "Person.h"
#include "Place.h"

namespace mrsa {

const std::string U_TO_C = "UNCOLONIZED TO COLONIZED";
const std::string C_TO_U = "COLONIZED TO UNCOLONIZED";
const std::string C_TO_I = "COLONIZED TO INFECTED";
const std::string I_TO_C = "INFECTED TO COLONIZED";
const std::string I_TO_U = "INFECTED TO UNCOLONIZED";

class TransmissionEventRecorder {

public:

	virtual ~TransmissionEventRecorder();

	/**
	 * Initialize the singleton.
	 */
	static void initialize(const std::string& filename);

	/**
	 * Gets singleton instance. If it has not been initialized, an exception
	 * is thrown.
	 */
	static TransmissionEventRecorder* instance();

	/**
	 * Closes the recorder and writes any unwritten data to the file.
	 */
	void close();

	void recordEvent(double tick, Person* person, Place* place, const std::string& event);

private:
	TransmissionEventRecorder(const std::string& filename);
	static TransmissionEventRecorder* instance_;

	std::ofstream out;
	bool open;
};

} /* namespace mrsa */
#endif /* TRANSMISSIONEVENTRECORDER_H_ */
