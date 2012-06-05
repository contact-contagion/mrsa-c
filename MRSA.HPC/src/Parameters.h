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

extern const std::string HOURLY_OUTPUT_FILE;
extern const std::string YEARLY_OUTPUT_FILE;
extern const std::string SUMMARY_OUTPUT_FILE;

extern const std::string COLONIZATION_FRAC;
extern const std::string INITIAL_INFECTION_COUNT;
extern const std::string INFECTION_PERIOD;
extern const std::string FASTER_SCALING;
extern const std::string SEEK_CARE_FRACTION;

extern const std::string MIN_INFECT_PERIOD;
extern const std::string SEEK_AND_DESTROY_AT;
extern const std::string SEEK_AND_DESTROY_CURE_FRACTION;


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

	std::string getStringParameter(const std::string& prop_name) const;

	int getIntParameter(const std::string& prop_name) const;

	double getDoubleParameter(const std::string& prop_name) const;

	void putParameter(const std::string& key, bool value);

	bool getBooleanParameter(const std::string& prop_name) const;

	void activateSeekAndDestroy() {
		seek_and_destroy_on = true;
	}

	bool seekAndDestroyActivated() const {
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
