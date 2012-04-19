/*
 * Place.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef PLACE_H_
#define PLACE_H_


#include "relogo/Turtle.h"

namespace mrsa {


class Place : public repast::relogo::Turtle {

	friend std::ostream& operator<<(std::ostream& os, const Place& person);

public:

	Place(repast::AgentId id, repast::relogo::Observer* obs, std::vector<std::string>& vec);
	virtual ~Place();

	std::string place_id, place_type;
	double longitude, latitude;
	int place_colonized, place_uncolonized, place_infected;
	bool faster_response;

};

std::ostream& operator<<(std::ostream& os, const Place& place);

} /* namespace mrsa */
#endif /* PLACE_H_ */
