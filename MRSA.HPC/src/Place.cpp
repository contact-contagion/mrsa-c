/*
 * Place.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include "Place.h"
#include "repast_hpc/Utilities.h"

namespace mrsa {

bool SEEK_AND_DESTROY = false;

using namespace repast;

const int PLACE_ID_IDX = 0;
const int PLACE_TYPE_IDX = 1;
// skip name and persons as we don't use them
// in the model.
//const int NAME_IDX = 2;
//const int PERSONS_IDX = 3;
const int LAT_IDX = 4;
const int LON_IDX = 5;

// creates a Place from the data in the vector and with the specified risk.
Place::Place(std::vector<std::string>& vec, float risk) : id_(vec[PLACE_ID_IDX]), type_(vec[PLACE_TYPE_IDX]),
		longitude(0), latitude(0), risk_(risk) {

	std::string val = vec[LAT_IDX];
	val = trim(val);
	if (val.length() > 0)
		latitude = strToDouble(val);

	val = vec[LON_IDX];
	val = trim(val);
	if (val.length() > 0)
		longitude = strToDouble(val);
}

Place::Place(std::string id, std::string type, float risk) : id_(id), type_(type), longitude(0), latitude(0), risk_(risk) {}

Place::~Place() {
}

std::ostream& operator<<(std::ostream& os, const Place& place) {
	os << "Place[" << place.id_ << ", " << place.type_ << "]";
	return os;
}

} /* namespace mrsa */
