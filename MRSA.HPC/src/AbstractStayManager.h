/*
 * AbstractStayManager.h
 *
 *  Created on: Mar 7, 2013
 *      Author: nick
 */

#ifndef ABSTRACTSTAYMANAGER_H_
#define ABSTRACTSTAYMANAGER_H_

#include <vector>

#include "StayManager.h"

namespace mrsa {

class AbstractStayManager : public PlaceStayManager {

public:

	/**
	 * Creates an AbstractStayManager with years of 0, 0 StayData.
	 */
	AbstractStayManager(size_t years);

	virtual ~AbstractStayManager();
	bool inPlace(int year, int day);

protected:
	std::vector<StayData> stay_data;
	unsigned int duration, start_day;
};

} /* namespace mrsa */
#endif /* ABSTRACTSTAYMANAGER_H_ */
