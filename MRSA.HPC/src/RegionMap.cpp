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
 * RegionMap.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: nick
 */

#include <sstream>
#include <stdexcept>

#include "RegionMap.h"

namespace mrsa {

RegionMap* RegionMap::instance_ = 0;

/*
const unsigned int N_ZIPS[] = { 60666, 60645, 60626, 60631, 60646, 60659, 60660, 60656, 60630, 60625,
		60640, 60634, 60641, 60618, 60613, 60657, 60707, 60639, 60647, 60614 };

const unsigned int W_ZIPS[] = { 60651, 60644, 60624, 60612, 60623, 60608 };

const unsigned int E_ZIPS[] = { 60622, 60642, 60610, 60654, 60611, 60601, 60661, 60606, 60602, 60607,
		60604, 60605, 60616 };

const unsigned int S_ZIPS[] = { 60638, 60632, 60609, 60653, 60615, 60629, 60636, 60621, 60637, 60652,
		60620, 60619, 60649, 60655, 60643, 60628, 60617, 60827, 60633 };
*/

// regions in data are numeric: 0 - 4. We use this
// array to look up the letter for the region.
const char REGIONS[] = {'O', 'N', 'C', 'W', 'S'};

RegionMap::RegionMap() :
		region_map() {
}

RegionMap::~RegionMap() {
}

void RegionMap::addZipCode(const unsigned int zip_code, const unsigned int region) {
	region_map.insert(std::pair<unsigned int, char>(zip_code, REGIONS[region]));
}

const char RegionMap::region(const unsigned int zip_code) {
	std::map<unsigned int, char>::const_iterator iter = region_map.find(zip_code);
	if (iter == region_map.end()) return 'O';
	return iter->second;
}

void RegionMap::regions(std::vector<char>& out) {
	out.push_back('N');
	out.push_back('S');
	out.push_back('W');
	out.push_back('C');
	out.push_back('O');
}

void RegionMap::zipCodes(std::vector<unsigned int>& out) {
	for (std::map<unsigned int, char>::const_iterator iter = region_map.begin(); iter != region_map.end(); ++iter) {
		out.push_back(iter->first);
	}
}

RegionMap* RegionMap::instance() {
	if (instance_ == 0)
		instance_ = new RegionMap();
	return instance_;
}

} /* namespace mrsa */
