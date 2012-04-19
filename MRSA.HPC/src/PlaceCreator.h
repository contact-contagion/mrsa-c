/*
 * PlaceCreator.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef PLACECREATOR_H_
#define PLACECREATOR_H_

#include <string>

#include "relogo/Observer.h"

#include "CSVReader.h"
#include "Place.h"

namespace mrsa {

/**
 * Functor that is to create Place agents.
 *
 */
class PlaceCreator {
public:
	PlaceCreator(const std::string& file);
	PlaceCreator(const PlaceCreator& creator);
	virtual ~PlaceCreator();

	Place* operator()(repast::AgentId id, repast::relogo::Observer* obs);

private:
	CSVReader reader;
};

} /* namespace mrsa */
#endif /* PLACECREATOR_H_ */
