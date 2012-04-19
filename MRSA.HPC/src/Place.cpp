/*
 * Place.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include "Place.h"
#include "repast_hpc/Utilities.h"

namespace mrsa {

using namespace repast;
using namespace repast::relogo;

const int PLACE_ID_IDX = 0;
const int PLACE_TYPE_IDX = 1;
const int NAME_IDX = 2;
const int PERSONS_IDX = 3;
const int LAT_IDX = 4;
const int LON_IDX = 5;

Place::Place(repast::AgentId id, repast::relogo::Observer* obs,
		std::vector<std::string>& vec) :
		Turtle(id, obs), place_id(vec[PLACE_ID_IDX]), place_type(
				vec[PLACE_TYPE_IDX]), place_colonized(0), place_uncolonized(0), place_infected(
				0), faster_response(false) {

	std::string val = vec[LAT_IDX];
	val = trim(val);
	if (val.length() > 0)
		latitude = strToDouble(val);

	val = vec[LON_IDX];
	val = trim(val);
	if (val.length() > 0)
		longitude = strToDouble(val);
}

Place::~Place() {
}

std::ostream& operator<<(std::ostream& os, const Place& place) {
	os << "Place[" << place.place_id << ", " << place.place_type << "]";
	return os;
}

} /* namespace mrsa */
