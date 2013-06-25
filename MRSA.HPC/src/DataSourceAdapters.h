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
 * DataSourceAdapters.h
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#ifndef DATASOURCEADAPTERS_H_
#define DATASOURCEADAPTERS_H_

#include <map>
#include <string>

#include "repast_hpc/TDataSource.h"

namespace mrsa {

/**
 * Adapts long stat vars to the TDataSource interface.
 */
class LDataSourceAdapter: public repast::TDataSource<double> {

public:
	LDataSourceAdapter(long* stat);
	virtual ~LDataSourceAdapter();

	double getData();

private:
	long* stat_;
};

/**
 * Adapts double stat vars to the TDataSource interface.
 */
class DDataSourceAdapter: public repast::TDataSource<double> {

public:
	DDataSourceAdapter(double* stat);
	virtual ~DDataSourceAdapter();

	double getData();

private:
	double* stat_;
};

/**
 * Adapts double stat vars to the TDataSource interface and
 * divides the result by 24 to convert hours into days
 */
class HoursToDaysDataSourceAdapter: public repast::TDataSource<double> {

public:
	HoursToDaysDataSourceAdapter(double* stat);
	virtual ~HoursToDaysDataSourceAdapter();

	double getData();

private:
	double* stat_;
};


/**
 * Adapts map double values to the TDataSource API. The constructor
 * takes a map key and a map. getData returns the current value
 * for that key.
 */
class PlaceCount: public repast::TDataSource<double> {

public:
	PlaceCount(std::map<std::string, double>* place_map, const std::string& place_name);
	virtual ~PlaceCount();

	double getData();

private:

	std::map<std::string, double>* place_map_;
	std::string place_name_;
};

}



#endif /* DATASOURCEADAPTERS_H_ */
