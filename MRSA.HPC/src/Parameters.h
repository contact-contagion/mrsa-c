/*
 * Parameters.h
 *
 *  Created on: Jun 5, 2012
 *      Author: nick
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "repast_hpc/Properties.h"

namespace mrsa {

// property constants, used to look up
// property values from the model properties file.
extern const std::string PERSONS_FILE;
extern const std::string PLACES_FILE;
extern const std::string ACTIVITIES_FILE;
extern const std::string RISK_FILE;
extern const std::string COL_INFECT_INIT_FILE;

extern const std::string HOURLY_OUTPUT_FILE;
extern const std::string YEARLY_OUTPUT_FILE;
extern const std::string SUMMARY_OUTPUT_FILE;
extern const std::string EVENT_OUTPUT_FILE;

extern const std::string COLONIZATION_FRAC;
extern const std::string INITIAL_INFECTION_COUNT;
extern const std::string INFECTION_PERIOD;
extern const std::string FASTER_SCALING;
extern const std::string SEEK_CARE_FRACTION;

//extern const std::string HOSPITALIZED_PROBABILITY;
//extern const std::string HOSPITAL_STAY_DURATION_MEAN;
//extern const std::string HOSPITAL_STAY_DURATION_SD;

extern const std::string MIN_INFECT_PERIOD;
extern const std::string SEEK_CARE_INFECT_PERIOD;
extern const std::string SELF_CARE_INFECT_PERIOD;
extern const std::string SEEK_AND_DESTROY_ENABLED;

extern const std::string A;
extern const std::string B;
extern const std::string E;
extern const std::string ALPHA;
extern const std::string THETA;
extern const std::string GAMMA;
extern const std::string Z;

/**
 * Parameters singleton that contains the model properties.
 *
 */
class Parameters {
public:

	virtual ~Parameters();

	/**
	 * Initializes the Parameters singleton with the specified properties.
	 */
	static void initialize(repast::Properties& props);

	/**
	 * Gets singleton instance. If it has not been initialized, an exception
	 * is thrown.
	 */
	static Parameters* instance();

	/**
	 * Gets the named parameter as a string.
	 *
	 * @return the named parameter.
	 */
	std::string getStringParameter(const std::string& prop_name) const;

	/**
	 * Gets the named parameter as an int.
	 *
	 * @return the named parameter.
	 */
	int getIntParameter(const std::string& prop_name) const;

	/**
	 * Gets the named parameter as a double.
	 *
	 * @return the named parameter.
	 */
	double getDoubleParameter(const std::string& prop_name) const;

	/**
	 * Puts the named parameter into this Parameters object.
	 *
	 * @param key the parameter's name
	 * @param value the parameter's value.
	 */
	void putParameter(const std::string& key, bool value);

	/**
	 * Gets the named parameter as a boolean.
	 *
	 * @return the named parameter.
	 */
	bool getBooleanParameter(const std::string& prop_name) const;

	/**
	 * Gets whether or not the seek and destroy flag is on.
	 */
	bool seekAndDestroyEnabled() const {
		return seek_and_destroy_on;
	}

private:
	static Parameters* instance_;
	repast::Properties props_;

	bool seek_and_destroy_on;

	Parameters(repast::Properties& props);
};

} /* namespace mrsa */
#endif /* PARAMETERS_H_ */
