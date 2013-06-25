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
 * PlaceCreator.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <vector>
#include <map>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "repast_hpc/Utilities.h"

#include "PlaceCreator.h"
#include "CSVReader.h"
#include "Household.h"
#include "School.h"
#include "Hospital.h"
#include "Prison.h"
#include "Gym.h"
#include "GeneralQuarters.h"
#include "Workplace.h"
#include "DefaultPlace.h"
#include "Constants.h"
#include "RegionMap.h"

namespace mrsa {

using namespace std;

const int TYPE_IDX = 1;
const int RISK_PLACE_TYPE_IDX = 0;
const int RISK_ACT_TYPE_IDX = 1;
const int RISK_PAR_IDX = 2;
const int RISK_TIP_IDX = 3;
const int RISK_AIP_IDX = 4;
const int RISK_X_IDX = 5;

const int ZIP_GROUP_IDX = 7;

PlaceCreator::PlaceCreator() {
}

PlaceCreator::~PlaceCreator() {
}

void setRisk(Risk& risk, int act_type, float a, float b, float x) {
	if (act_type == 0) {
		risk.a0_ = a;
		risk.b0_ = b;
		risk.x0_ = x;
	} else {
		risk.a1_ = a;
		risk.b1_ = b;
		risk.x1_ = x;
	}
}

//void loadRisk(const string& risk_file, std::map<string, Risk>& map) {
//	CSVReader reader(risk_file);
//	vector<string> vec;
//	// skip the first header line.
//	reader.next(vec);
//
//	// read each line and depending on the type, create that type of place.
//	while (reader.next(vec)) {
//		int act_type = repast::strToInt(vec[RISK_ACT_TYPE_IDX]);
//		float par = (float) repast::strToDouble(vec[RISK_PAR_IDX]);
//		float tip = (float) repast::strToDouble(vec[RISK_TIP_IDX]);
//		float aip = (float) repast::strToDouble(vec[RISK_AIP_IDX]);
//
//		string type = vec[RISK_PLACE_TYPE_IDX];
//		// lower case the type for easier, less error prone comparisons
//		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
//		std::map<string, Risk>::iterator iter = map.find(type);
//		if (iter == map.end()) {
//			Risk risk;
//			setRisk(risk, act_type, par * tip, aip);
//			map.insert(std::make_pair(type, risk));
//		} else {
//			Risk& risk = iter->second;
//			setRisk(risk, act_type, par * tip, aip);
//		}
//	}
//
//	/*
//	for (std::map<string, Risk>::iterator iter = map.begin(); iter != map.end(); ++iter) {
//		Risk& risk = iter->second;
//		std::cout << iter->first << ": a0 = " << risk.a0_ << ", a1 = " << risk.a1_ << ", b0 = "
//				<< risk.b0_  << ", b1= " << risk.b1_ << std::endl;
//	}
//	*/
//}

void loadRisk(Properties& props, std::map<string, Risk>& map) {

  std::vector<std::string> placeTypes;
  const std::string placeTypeList = props.getProperty("place.types");
  boost::split(placeTypes, placeTypeList, boost::is_any_of(","));

  for(size_t i = 0; i< placeTypes.size(); i++){
    for(int j = 0; j < 2; j++){
      std::string type = placeTypes[i];
      std::string typeProp = type;
      boost::replace_all(typeProp, " ", "_");

      std::stringstream PARname; PARname << typeProp << "_" << j << "_PAR";
      std::stringstream TIPname; TIPname << typeProp << "_" << j << "_TIP";
      std::stringstream AIPname; AIPname << typeProp << "_" << j << "_AIP";
      std::stringstream Xname; Xname << typeProp << "_" << j << "_X";

      float par = (float) repast::strToDouble(props.getProperty(PARname.str()));
      float tip = (float) repast::strToDouble(props.getProperty(TIPname.str()));
      float aip = (float) repast::strToDouble(props.getProperty(AIPname.str()));
      float x = (float) repast::strToDouble(props.getProperty(Xname.str()));

      // lower case the type for easier, less error prone comparisons
      std::transform(type.begin(), type.end(), type.begin(), ::tolower);
      std::map<string, Risk>::iterator iter = map.find(type);
      if (iter == map.end()) {
        Risk risk;
        setRisk(risk, j, par * tip, aip, x);
        map.insert(std::make_pair(type, risk));
        //std::cout << type << " " << j <<  ": par = " << par << ", tip = " << tip << ", aip = " << aip << ", x = " << x << std::endl;
      } else {
        Risk& risk = iter->second;
        setRisk(risk, j, par * tip, aip, x);
        //std::cout << type << j << ": par = " << par << ", tip = " << tip << ", aip = " << aip << ", x = " << x << std::endl;
      }
    }
  }
}

// reads the file and fills the vector of places with the created places.
void PlaceCreator::run(const string& places_file, Properties& props, vector<Place*>& places) {
	std::map<string, Risk> map;
	loadRisk(props, map);

	CSVReader reader(places_file);
	vector<string> vec;
	// skip the first header line.
	reader.skip(1);

	// skip to the line equal to the rank
	boost::mpi::communicator* world = RepastProcess::instance()->getCommunicator();
	reader.skip(world->rank());

	RegionMap* region_map = RegionMap::instance();

	// read each line and depending on the type, create that type of place.
	while (reader.next(vec)) {
		string type = vec[TYPE_IDX];
		// lower case the type for easier, less error prone comparisons
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
		vec[TYPE_IDX] = type;

		std::map<string, Risk>::iterator iter = map.find(type);
		if (iter == map.end()) throw std::domain_error("Missing risk values for place type '" + type + "'");
		Risk& risk = iter->second;

		Place* place = 0;
		if (type == SCHOOL_TYPE) {
			place = new School(vec, risk);
			///std::cout << "making a school" << std::endl;
		} else if (type == HOUSEHOLD_TYPE) {
			place = new Household(vec, risk);
			//std::cout << "making a household" << std::endl;
		} else if (type == WORKPLACE_TYPE) {
			place = new Workplace(vec, risk);
			//std::cout << "making a workplace" << std::endl;
		} else if (type == HOSPITAL_TYPE) {
			place = new Hospital(vec, risk);
			//std::cout << "making a hospital" << std::endl;
		} else if (type == DORM_TYPE || type == NURSING_HOME_TYPE) {
			place = new GeneralQuarters(vec, risk);
			//std::cout << "making a " << type << std::endl;
		} else if (type == PRISON_TYPE) {
			place = new Prison(vec, risk);
			//std::cout << "making a " << type << std::endl;
		} else if (type == GYM_TYPE) {
			place = new Gym(vec, risk);
		} else {
			// if the type is not one we have a specific place for
			// yet, then create a DefaultPlace.
			place = new DefaultPlace(vec, risk);
			//std::cout << "making a default" << std::endl;
		}
		// add the place to the vector.
		places.push_back(place);

		region_map->addZipCode(place->zipCode(), strToUInt(vec[ZIP_GROUP_IDX]));

		// skip to the next line to read
		reader.skip(world->size() - 1);
	}
}

}
