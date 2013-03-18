/*
 * NoStayManager.h
 *
 *  Created on: Oct 24, 2012
 *      Author: nick
 */

#ifndef NOSTAYMANAGER_H_
#define NOSTAYMANAGER_H_

#include "StayManager.h"

namespace mrsa {

class NoStayManager : public PlaceStayManager {

public:

	NoStayManager() : PlaceStayManager() {}
	virtual ~NoStayManager() {}

	bool inPlace(int year, int day) {
		return false;
	}
};

}



#endif /* NOSTAYMANAGER_H_ */
