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

const std::string HOURLY_OUTPUT_FILE = "hourly.output.file";
const std::string YEARLY_OUTPUT_FILE = "yearly.output.file";
const std::string SUMMARY_OUTPUT_FILE = "summary.output.file";

const std::string COLONIZATION_FRAC = "initial.colonization.fraction";
const std::string INITIAL_INFECTION_COUNT = "initial.infected.count";
const std::string INFECTION_PERIOD = "minimum.infection.period";
const std::string FASTER_SCALING = "faster.response.scaling.factor";
const std::string SEEK_CARE_FRACTION = "seek.care.fraction";

const std::string HOSPITALIZED_PROBABILITY = "hospital.stay.probability";
const std::string HOSPITAL_STAY_DURATION_MEAN = "hospital.stay.duration.mean";
const std::string HOSPITAL_STAY_DURATION_SD = "hospital.stay.duration.sd";

const std::string MIN_INFECT_PERIOD = "minimum.infection.period";
const std::string SEEK_CARE_INFECT_PERIOD = "seek.care.infection.period";
const std::string SELF_CARE_INFECT_PERIOD = "self.care.infection.period";
const std::string SEEK_AND_DESTROY_AT = "seek.and.destroy.at";
const std::string SEEK_AND_DESTROY_CURE_FRACTION = "seek.and.destroy.cure.fraction";

 const std::string A = "a";
 const std::string B = "b";
 const std::string E = "e";
 const std::string ALPHA ="alpha";
 const std::string THETA = "theta";
 const std::string GAMMA = "gamma";
 const std::string Z = "z";



Parameters* Parameters::instance_ = 0;

Parameters::Parameters(repast::Properties& props) : props_(props), seek_and_destroy_on(false) {

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
	return getIntParameter(prop_name) != 0;
}

} /* namespace mrsa */
