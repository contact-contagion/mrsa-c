/*
 * IHospitalStayManager.h
 *
 *  Created on: Oct 24, 2012
 *      Author: nick
 */

#ifndef STAYMANAGER_H_
#define STAYMANAGER_H_

#include <utility>


namespace mrsa {

typedef std::pair<unsigned int, unsigned int> StayData;

class PlaceStayManager {

public:
	PlaceStayManager() {}
	virtual ~PlaceStayManager() {}

	virtual bool inPlace(int year, int day) = 0;
};

}


#endif /* STAYMANAGER_H_ */
