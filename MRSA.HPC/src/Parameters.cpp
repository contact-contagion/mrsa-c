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
 * Parameters.cpp
 *
 *  Created on: Jun 5, 2012
 *      Author: nick
 */

#include "Parameters.h"
#include "repast_hpc/Utilities.h"

namespace mrsa {


using namespace repast;


// property constants, used to look up
// property values from the model properties file.
const std::string PERSONS_FILE = "persons.file";
const std::string PLACES_FILE = "places.file";
const std::string ACTIVITIES_FILE = "activities.file";
const std::string RISK_FILE = "risk.file";
const std::string COL_INFECT_INIT_FILE = "init.c_and_i.file";

const std::string HOURLY_OUTPUT_FILE = "hourly.output.file";
const std::string YEARLY_OUTPUT_FILE = "yearly.output.file";
const std::string SUMMARY_OUTPUT_FILE = "summary.output.file";
const std::string EVENT_OUTPUT_FILE = "event.output.file";

const std::string COLONIZATION_SCALING = "initial.colonization.scaling";
const std::string INITIAL_INFECTION_COUNT = "initial.infected.count";
const std::string INFECTION_PERIOD = "minimum.infection.period";
const std::string FASTER_SCALING = "faster.response.scaling.factor";
const std::string SEEK_CARE_FRACTION = "seek.care.fraction";

//const std::string HOSPITALIZED_PROBABILITY = "hospital.stay.probability";
//const std::string HOSPITAL_STAY_DURATION_MEAN = "hospital.stay.duration.mean";
//const std::string HOSPITAL_STAY_DURATION_SD = "hospital.stay.duration.sd";

const std::string MIN_INFECT_PERIOD = "minimum.infection.period";
const std::string SEEK_CARE_INFECT_PERIOD = "seek.care.infection.period";
const std::string SELF_CARE_INFECT_PERIOD = "self.care.infection.period";
const std::string SEEK_AND_DESTROY_ENABLED = "seek.and.destroy.enabled";

const std::string A = "a";
const std::string B = "b";
const std::string E = "e";
const std::string ALPHA ="alpha";
const std::string THETA = "theta";
const std::string GAMMA = "gamma";
const std::string Z = "z";

const std::string MIN_JAIL_DURATION = "minimum.jail.stay";
const std::string MEAN_JAIL_DURATION = "mean.jail.stay";

const std::string MRSA_PROB_COLUMN = "initial.mrsa.probability.column";
const std::string P_2001 = "p_mrsa2001";
const std::string P_2006 = "p_mrsa2006";

Parameters* Parameters::instance_ = 0;

Parameters::Parameters(repast::Properties& props) : props_(props),
		seek_and_destroy_on(false) {
	std::string sd_val = props.getProperty(SEEK_AND_DESTROY_ENABLED);
	std::transform(sd_val.begin(), sd_val.end(), sd_val.begin(), ::tolower);
	seek_and_destroy_on = sd_val == "true";
}

Parameters::~Parameters() {

}

void Parameters::initialize(Properties& props) {
	if (instance_ != 0) {
		throw std::domain_error("Parameters is already initialized");
	}
	instance_ = new Parameters(props);
}

Parameters* Parameters::instance() {
	if (instance_ == 0)
		throw std::domain_error("Parameters must be initialized before instance() is called");

	return instance_;
}

std::string Parameters::getStringParameter(const std::string& prop_name) const {
	std::string val = props_.getProperty(prop_name);
	if (val.length() == 0)
		throw std::invalid_argument("Invalid property name '" + prop_name + "', no property found.");
	return val;
}

int Parameters::getIntParameter(const std::string& prop_name) const {
	return strToInt(getStringParameter(prop_name));
}

double Parameters::getDoubleParameter(const std::string& prop_name) const {
	return strToDouble(getStringParameter(prop_name));
}

void Parameters::putParameter(const std::string& key, bool value) {
	int val = value ? 1 : 0;
	props_.putProperty(key, val);
}

bool Parameters::getBooleanParameter(const std::string& prop_name) const {
	std::string val = getStringParameter(prop_name);
	return val == "true" || val == "TRUE";
}

} /* namespace mrsa */
