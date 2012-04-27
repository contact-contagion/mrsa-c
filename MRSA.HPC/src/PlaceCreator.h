/*
 * PlaceCreator.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef PLACECREATOR_H_
#define PLACECREATOR_H_

#include <string>
#include <vector>

#include "CSVReader.h"
#include "Place.h"

namespace mrsa {

/**
 * Creates the various places.
 *
 */
class PlaceCreator {
public:
	PlaceCreator();
	PlaceCreator(const PlaceCreator& creator);
	virtual ~PlaceCreator();


	/**
	 * Runs this PlaceCreator on the specified file, putting the
	 * created places in the the places vector.
	 *
	 * @param file the path to the places file
	 * @param places the vector to put the created places in
	 */
	void run(const std::string& file, std::vector<Place*>& places);
};

} /* namespace mrsa */
#endif /* PLACECREATOR_H_ */
