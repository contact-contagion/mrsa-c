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

struct Calendar {

	int hour_of_day, day_of_week, day_of_year, year;

	Calendar();
	void increment();
	bool isWeekDay();
};

/**
 * The Observer class for the MRSA model.
 */
class MRSAObserver : public repast::relogo::Observer {

  repast::Properties* propsPtr;
  int yearCounter;

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

	/**
	 * Calculates the yearly stats.
	 */
	void calcYearlyStats();

	/**
	 * Turns seek and destroy behavior on.
	 */
	void activateSeekAndDestroy();

private:

	int personType;

	std::vector<Place*> places;
	repast::relogo::AgentSet<Person>* people_;
	std::string summary_output_file;
	Calendar calendar;


	/**
	 * Load the activities and assign them to persons.
	 */
	void initializeActivities(repast::Properties& props);

	/**
	 * Setup the hourly data collection.
	 *
	 * @param file the file to write the hourly output to
	 */
	void initializeHourlyDataCollection(const std::string& file);

	/**
	 * Setup the yearly data collection.
	 *
	 * @param file the file to write yearly output to
	 */
	void initializeYearlyDataCollection(const std::string& file);

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
