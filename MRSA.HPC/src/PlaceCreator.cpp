/*
 * PlaceCreator.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <vector>
#include <map>
#include <iostream>

#include "repast_hpc/Utilities.h"

#include "PlaceCreator.h"
#include "CSVReader.h"
#include "Household.h"
#include "School.h"
#include "Workplace.h"
#include "DefaultPlace.h"
#include "Constants.h"

namespace mrsa {

using namespace std;

const int TYPE_IDX = 1;
const int RISK_PLACE_TYPE_IDX = 0;
const int RISK_ACT_TYPE_IDX = 1;
const int RISK_A_IDX = 2;
const int RISK_B_IDX = 3;

PlaceCreator::PlaceCreator() {
}

PlaceCreator::~PlaceCreator() {
}

void PlaceCreator::setRisk(Risk& risk, int act_type, float a, float b) {
	if (act_type == 0) {
		risk.a0_ = a;
		risk.b0_ = b;
	} else {
		risk.a1_ = a;
		risk.b1_ = b;
	}
}

void PlaceCreator::loadRisk(const string& risk_file, std::map<string, Risk>& map) {
	CSVReader reader(risk_file);
	vector<string> vec;
	// skip the first header line.
	reader.next(vec);

	// read each line and depending on the type, create that type of place.
	while (reader.next(vec)) {
		int act_type = repast::strToInt(vec[RISK_ACT_TYPE_IDX]);
		float a = (float) repast::strToDouble(vec[RISK_A_IDX]);
		float b = (float) repast::strToDouble(vec[RISK_B_IDX]);

		string type = vec[RISK_PLACE_TYPE_IDX];
		// lower case the type for easier, less error prone comparisons
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
		std::map<string, Risk>::iterator iter = map.find(type);
		if (iter == map.end()) {
			Risk risk;
			setRisk(risk, act_type, a, b);
			map.insert(std::make_pair(type, risk));
		} else {
			Risk& risk = iter->second;
			setRisk(risk, act_type, a, b);
		}
	}

	//for (std::map<string, Risk>::iterator iter = map.begin(); iter != map.end(); ++iter) {
	//	Risk& risk = iter->second;
	//	std::cout << iter->first << ": a0 = " << risk.a0_ << ", a1 = " << risk.a1_ << ", b0 = "
	//			<< risk.b0_  << ", b1= " << risk.b1_ << std::endl;
	//}
}



// reads the file and fills the vector of places with the created places.
void PlaceCreator::run(const string& places_file, const string& risk_file, vector<Place*>& places) {
	std::map<string, Risk> map;
	loadRisk(risk_file, map);

	CSVReader reader(places_file);
	vector<string> vec;
	// skip the first header line.
	reader.next(vec);

	// read each line and depending on the type, create that type of place.
	while (reader.next(vec)) {
		string type = vec[TYPE_IDX];
		// lower case the type for easier, less error prone comparisons
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
		vec[TYPE_IDX] = type;

		std::map<string, Risk>::iterator iter = map.find(type);
		Risk& risk = iter->second;

		Place* place = 0;
		if (type == SCHOOL_TYPE || type == GYM_TYPE || type == HOSPITAL_TYPE) {
			place = new School(vec, risk);
			///std::cout << "making a school" << std::endl;
		} else if (type == HOUSEHOLD_TYPE) {
			place = new Household(vec, risk);
			//std::cout << "making a household" << std::endl;
		} else if (type == WORKPLACE_TYPE) {
			place = new Workplace(vec, risk);
			//std::cout << "making a workplace" << std::endl;
		} else {
			// if the type is not one we have a specific place for
			// yet, then create a DefaultPlace.
			place = new DefaultPlace(vec, risk);
			//std::cout << "making a default" << std::endl;
		}
		// add the place to the vector.
		places.push_back(place);
	}
}

}
