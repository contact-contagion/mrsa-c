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
#include "Statistics.h"

namespace mrsa {

/**
 * The Observer class for the MRSA model.
 */
class MRSAObserver : public repast::relogo::Observer {

public:
	MRSAObserver();
	virtual ~MRSAObserver();

	/**
	 * Called every tick of the simulation. This makes each person
	 * perform their current activity, and then performs the disease
	 * transmission based on the places the persons moved to.
	 */
	void go();

	/**
	 * Performs some additional final statistics collection
	 * when called at the end of the simluation.
	 */
	void atEnd();

	/**
	 * Sets up the simulation for a run, given the specified properties.
	 */
	virtual void setup(repast::Properties& props);

private:

	int personType;

	Statistics* stats;
	std::vector<Place*> places;
	repast::relogo::AgentSet<Person>* people_;

	/**
	 * Load the activities and assign them to persons.
	 */
	void initializeActivities(repast::Properties& props);

	/**
	 * Setup the data collection.
	 */
	void initializeDataCollection();

	/**
	 * Creates the persons from the persons file.
	 */
	void createPersons(repast::Properties& props, std::map<std::string, Place*>* placeMap);

	/**
	 * Creates the places from the places file.
	 */
	void createPlaces(repast::Properties& props, std::map<std::string, Place*>* placeMap);

	/**
	 * Assigns randomly selected persons a disease status of colonized or infected based on
	 * parameters in the properties.
	 */
	void initializeDiseaseStatus(repast::Properties& props, repast::relogo::AgentSet<Person>& people);
};

} /* namespace mrsa */
#endif /* MRSAOBSERVER_H_ */
