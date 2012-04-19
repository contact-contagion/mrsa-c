/*
 * PlaceCreator.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include <vector>

#include "PlaceCreator.h"

namespace mrsa {

using namespace std;

PlaceCreator::PlaceCreator(const string& file) :
		reader(file) {
	// skip the first line
	vector<string> vec;
	reader.next(vec);
}

PlaceCreator::PlaceCreator(const PlaceCreator& creator) :
		reader(creator.reader) {
	// skip the first line
	vector<string> vec;
	reader.next(vec);
}

PlaceCreator::~PlaceCreator() {
}

Place* PlaceCreator::operator()(repast::AgentId id,
		repast::relogo::Observer* obs) {
	vector<string> vec;
	reader.next(vec);
	Place* place = new Place(id, obs, vec);
	//std::cout << (*place) << std::endl;

	return place;
}

}
