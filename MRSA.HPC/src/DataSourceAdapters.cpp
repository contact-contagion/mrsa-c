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
 * DataSourceAdapters.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */
#include <map>
#include <cmath>

#include "DataSourceAdapters.h"

namespace mrsa {

LDataSourceAdapter::LDataSourceAdapter(long* stat) :
		repast::TDataSource<double>(), stat_(stat) {
}

LDataSourceAdapter::~LDataSourceAdapter() {
}

double LDataSourceAdapter::getData() {
	return (double) (*stat_);
}

DDataSourceAdapter::DDataSourceAdapter(double* stat) :
		repast::TDataSource<double>(), stat_(stat) {
}

DDataSourceAdapter::~DDataSourceAdapter() {
}

double DDataSourceAdapter::getData() {
	return *stat_;
}

HoursToDaysDataSourceAdapter::HoursToDaysDataSourceAdapter(double* stat) :
		repast::TDataSource<double>(), stat_(stat) {
}

HoursToDaysDataSourceAdapter::~HoursToDaysDataSourceAdapter() {
}

double HoursToDaysDataSourceAdapter::getData() {
	return ceil((*stat_) / 24);
}


PlaceCount::PlaceCount(std::map<std::string, double>* place_map, const std::string& place_name) :
		repast::TDataSource<double>(), place_map_(place_map), place_name_(place_name) {
}

PlaceCount::~PlaceCount() {
}

double PlaceCount::getData() {
	std::map<std::string, double>::iterator iter = place_map_->find(place_name_);
	if (iter == place_map_->end())
		return 0;
	return iter->second;
}


}

