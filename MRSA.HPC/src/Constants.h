/*
 * constants.h
 *
 *  Created on: May 9, 2012
 *      Author: nick
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>

namespace mrsa {

// the type is lower-cased before the compare
// so these are OK, even though they don't match
// the case of those in the data files.
const std::string SCHOOL_TYPE = "school";
const std::string WORKPLACE_TYPE = "workplace";
const std::string HOUSEHOLD_TYPE = "household";
const std::string GYM_TYPE = "gym";
const std::string HOSPITAL_TYPE = "hospital";
const std::string DORM_TYPE = "college dorm";
const std::string NURSING_HOME_TYPE = "nursing home";
const std::string PRISON_TYPE = "prison";

// name of the random dist used to draw other households.
const std::string OH_DIST = "other.household.random.dist";
// name of distribution used to draw hospital stay length from
const std::string HOSPITAL_STAY_DURATION = "hospital.stay.duration";

}

#endif /* CONSTANTS_H_ */
