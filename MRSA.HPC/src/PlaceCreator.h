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
 * PlaceCreator.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef PLACECREATOR_H_
#define PLACECREATOR_H_

#include <string>
#include <vector>

#include "CSVReader.h"
#include "Place.h"
#include "repast_hpc/Properties.h"

using namespace repast;

namespace mrsa {

/**
 * Creates the various places.
 *
 */
class PlaceCreator {

public:
	PlaceCreator();
	PlaceCreator(const PlaceCreator& creator);
	virtual ~PlaceCreator();


	/**
	 * Runs this PlaceCreator on the specified file, putting the
	 * created places in the the places vector.
	 *
	 * @param file the path to the places file
	 * @param places the vector to put the created places in
	 */
//	void run(const std::string& places_file, const std::string& risk_file, std::vector<Place*>& places);

	void run(const std::string& places_file, Properties& props, std::vector<Place*>& places);
};

} /* namespace mrsa */
#endif /* PLACECREATOR_H_ */
