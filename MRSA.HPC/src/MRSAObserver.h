/*
 * MRSAObserver.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef MRSAOBSERVER_H_
#define MRSAOBSERVER_H_

#include "relogo/Observer.h"
#include "repast_hpc/Properties.h"

#include "Place.h";
#include "Person.h";
#include "PersonStats.h"

namespace mrsa {

class MRSAObserver : public repast::relogo::Observer {

public:
	MRSAObserver();
	virtual ~MRSAObserver();

	void go();

	virtual void setup(repast::Properties& props);

private:

	int personType;

	PersonStats* person_stats;
	std::vector<Place*> places;
	repast::relogo::AgentSet<Person>* people_;

	void initializeActivities(repast::Properties& props);
	void initializeDataCollection();
	void createPersons(repast::Properties& props, std::map<std::string, Place*>* placeMap);
	void createPlaces(repast::Properties& props, std::map<std::string, Place*>* placeMap);
	void initializeDiseaseStatus(repast::Properties& props, repast::relogo::AgentSet<Person>& people);
};

} /* namespace mrsa */
#endif /* MRSAOBSERVER_H_ */
