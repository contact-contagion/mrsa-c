/*
 * RegionMap.h
 *
 *  Created on: Mar 18, 2013
 *      Author: nick
 */

#ifndef REGIONMAP_H_
#define REGIONMAP_H_

#include <map>

namespace mrsa {

class RegionMap {
public:
	virtual ~RegionMap();

	/**
	 * Gets the instance of this RegionMap singleton
	 */
	static RegionMap* instance();

	/**
	 * Gets the region for the specified zip code.
	 *
	 * @return  the region for the specified zip code.
	 */
	const char region(const unsigned int zip_code);


private:
	RegionMap();
	std::map<unsigned int, char> region_map;

	static RegionMap* instance_;

};

} /* namespace mrsa */
#endif /* REGIONMAP_H_ */
