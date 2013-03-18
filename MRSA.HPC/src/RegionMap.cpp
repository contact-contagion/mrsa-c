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

const unsigned int N_ZIPS[] = { 60666, 60645, 60626, 60631, 60646, 60659, 60660, 60656, 60630, 60625,
		60640, 60634, 60641, 60618, 60613, 60657, 60707, 60639, 60647, 60614 };

const unsigned int W_ZIPS[] = { 60651, 60644, 60624, 60612, 60623, 60608 };

const unsigned int E_ZIPS[] = { 60622, 60642, 60610, 60654, 60611, 60601, 60661, 60606, 60602, 60607,
		60604, 60605, 60616 };

const unsigned int S_ZIPS[] = { 60638, 60632, 60609, 60653, 60615, 60629, 60636, 60621, 60637, 60652,
		60620, 60619, 60649, 60655, 60643, 60628, 60617, 60827, 60633 };

RegionMap::RegionMap() :
		region_map() {

	for (int i = 0; i < 20; ++i) {
		region_map.insert(std::pair<unsigned int, char>(N_ZIPS[i], 'N'));
	}

	for (int i = 0; i < 6; ++i) {
		region_map.insert(std::pair<unsigned int, char>(W_ZIPS[i], 'W'));
	}

	for (int i = 0; i < 13; ++i) {
		region_map.insert(std::pair<unsigned int, char>(E_ZIPS[i], 'E'));
	}

	for (int i = 0; i < 19; ++i) {
		region_map.insert(std::pair<unsigned int, char>(S_ZIPS[i], 'S'));
	}
}

RegionMap::~RegionMap() {
}

const char RegionMap::region(const unsigned int zip_code) {
	std::map<unsigned int, char>::const_iterator iter = region_map.find(zip_code);
	if (iter == region_map.end()) {
		std::stringstream ss;
		ss << "Region not found for zip code " << zip_code;
		throw std::invalid_argument(ss.str());
	}
	return iter->second;
}

RegionMap* RegionMap::instance() {
	if (instance_ == 0)
		instance_ = new RegionMap();
	return instance_;
}

} /* namespace mrsa */
