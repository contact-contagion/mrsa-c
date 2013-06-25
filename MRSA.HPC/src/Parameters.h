/*
*MRSA Model
*
*Copyright (c) 2013 University of Chicago and Argonne National Laboratory
*   All rights reserved.
*  
*   Redistribution and use in source and binary forms, with 
*   or without modification, are permitted provided that the following 
*   conditions are met:
*  
*  	 Redistributions of source code must retain the above copyright notice,
*  	 this list of conditions and the following disclaimer.
*  
*  	 Redistributions in binary form must reproduce the above copyright notice,
*  	 this list of conditions and the following disclaimer in the documentation
*  	 and/or other materials provided with the distribution.
*  
*  	 Neither the name of the Argonne National Laboratory nor the names of its
*     contributors may be used to endorse or promote products derived from
*     this software without specific prior written permission.
*  
*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
*   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
*   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
*   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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

extern const std::string COLONIZATION_SCALING;
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

extern const std::string MIN_JAIL_DURATION;
extern const std::string MEAN_JAIL_DURATION;

extern const std::string MRSA_PROB_COLUMN;
extern const std::string P_2001;
extern const std::string P_2006;

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
