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

}

#endif /* CONSTANTS_H_ */
