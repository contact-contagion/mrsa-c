/*
 * School.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#include "School.h"
#include "DiseaseStatus.h"
#include "Constants.h"
#include "Statistics.h"

#include "FileOutput.h"

namespace mrsa {

using namespace std;


const unsigned int MAX_SCHOOL_COMPONENT_SIZE = 30;

School::School(std::vector<std::string>& vec, Risk risk) : CompositePlace(vec, risk, MAX_SCHOOL_COMPONENT_SIZE, SCHOOL) {
}


School::~School() {

}

} /* namespace mrsa */
