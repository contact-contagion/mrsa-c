/*
 * PlaceCreator.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <vector>
#include <iostream>

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

PlaceCreator::PlaceCreator() {
}

PlaceCreator::~PlaceCreator() {
}

// reads the file and fills the vector of places with the created places.
void PlaceCreator::run(const string& file, vector<Place*>& places) {
	CSVReader reader(file);
	vector<string> vec;
	// skip the first header line.
	reader.next(vec);

	// read each line and depending on the type, create that type of place.
	while (reader.next(vec)) {
		string type = vec[TYPE_IDX];
		// lower case the type for easier, less error prone comparisons
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
		vec[TYPE_IDX] = type;
		Place* place = 0;
		if (type == SCHOOL_TYPE) {
			place = new School(vec);
			///std::cout << "making a school" << std::endl;
		} else if (type == HOUSEHOLD_TYPE) {
			place = new Household(vec);
			//std::cout << "making a household" << std::endl;
		} else if (type == WORKPLACE_TYPE) {
			place = new Workplace(vec);
			//std::cout << "making a workplace" << std::endl;
		} else {
			// if the type is not one we have a specific place for
			// yet, then create a DefaultPlace.
			place = new DefaultPlace(vec);
			//std::cout << "making a default" << std::endl;
		}
		// add the place to the vector.
		places.push_back(place);
	}
}

}
