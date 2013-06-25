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
 * PlaceStats.h
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#ifndef PLACESTATS_H_
#define PLACESTATS_H_

#include <map>

#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/Properties.h"

namespace mrsa {

struct RegionPlaceStat {

	char region;
	long infection_count, colonization_count, stay_count;
	double duration;

	void reset() {
		infection_count = colonization_count = stay_count = 0;
		duration = 0;
	}

};

class PlaceStats {

public:
	PlaceStats();
	virtual ~PlaceStats();

	void incrementStayCount(unsigned int zip_code);
	void incrementDuration(double val, unsigned int zip_code);

	void incrementColonizationCount(unsigned int zip_code);
	void incrementInfectionCount(unsigned int zip_code);

	void createDataSources(const std::string& key_prefix, repast::SVDataSetBuilder& builder, bool include_region);

	void reset() {
		infection_count = colonization_count = stay_count = 0;
		duration_ = 0;
		for (std::map<char, RegionPlaceStat>::iterator iter = region_stats.begin();
				iter != region_stats.end(); ++iter) {
			iter->second.reset();
		}
	}

	void updateColonizationFraction(double colonization_sum) {
		col_fraction = colonization_sum == 0 ? 0 : colonization_count / colonization_sum;
	}

	void addToProps(repast::Properties& props, const std::string& key_prefix, int year, bool include_region);

	long infection_count, colonization_count;
	// the number individual stays in this place
	long stay_count;
	// total duration in hours
	double duration_;
	double col_fraction;

	std::map<char, RegionPlaceStat> region_stats;
};

} /* namespace mrsa */
#endif /* PLACESTATS_H_ */
