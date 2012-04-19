/*
 * PersonsCreator.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef PERSONSCREATOR_H_
#define PERSONSCREATOR_H_

#include <string>
#include <map>

#include "relogo/Observer.h"

#include "CSVReader.h"
#include "Person.h"
#include "Place.h"

namespace mrsa {

/**
 * Functor that is to create Persons agents.
 *
 */
class PersonsCreator {
public:
	PersonsCreator(const std::string& file, std::map<std::string, Place*>* map);
	PersonsCreator(const PersonsCreator& creator);
	virtual ~PersonsCreator();

	Person* operator()(repast::AgentId id, repast::relogo::Observer* obs);

private:
	CSVReader reader;
	std::map<std::string, Place*>* places;

	Place* findPlace(const std::string id);
};

} /* namespace mrsa */
#endif /* PERSONSCREATOR_H_ */
