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

class TransitionFunctor {
public:
	virtual ~TransitionFunctor() {}
	virtual void operator()(Person* person) = 0;
};

class NoOpTransitionFunctor : public TransitionFunctor {
public:
	NoOpTransitionFunctor() : TransitionFunctor() {}
	virtual ~NoOpTransitionFunctor() {}

	virtual void operator()(Person* person) {}
};

/*
class SimpleTransitionFunctor : public TransitionFunctor {
public:
	SimpleTransitionFunctor() : TransitionFunctor() {}
	virtual ~SimpleTransitionFunctor() {}

	virtual void operator()(Person* person) {
		person->simpleTransition();
	}
};


class DetailedActivityTransitionFunctor : public TransitionFunctor {
public:
	DetailedActivityTransitionFunctor() : TransitionFunctor() {}
	virtual ~DetailedActivityTransitionFunctor() {}

	virtual void operator()(Person* person) {
		person->detailedActivityTransition();
	}
};
*/

class DetailedPlaceTransitionFunctor : public TransitionFunctor {
public:
	DetailedPlaceTransitionFunctor(float infection_period) : TransitionFunctor(), _infection_period(infection_period) {}
	virtual ~DetailedPlaceTransitionFunctor() {}

	virtual void operator()(Person* person) {
		person->detailedPlaceTransition(_infection_period);
	}

private:
	float _infection_period;
};

class MRSAObserver : public repast::relogo::Observer {

public:
	MRSAObserver();
	virtual ~MRSAObserver();

	void go();

	virtual void setup(repast::Properties& props);

private:

	int placeType, personType;

	TransitionFunctor* transition;
	PersonStats* person_stats;
	repast::relogo::AgentSet<Place>* places;

	void initializeActivities(repast::Properties& props);
	void initializeDataCollection();
	void createPersons(repast::Properties& props, std::map<std::string, Place*>* placeMap);
	void createPlaces(repast::Properties& props, std::map<std::string, Place*>* placeMap);
	void initializeDiseaseStatus(repast::Properties& props, repast::relogo::AgentSet<Person>& people);
};

} /* namespace mrsa */
#endif /* MRSAOBSERVER_H_ */
