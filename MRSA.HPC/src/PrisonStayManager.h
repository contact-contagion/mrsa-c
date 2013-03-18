/*
 * PrisonStayManager.h
 *
 *  Created on: Mar 7, 2013
 *      Author: nick
 */

#ifndef PRISONSTAYMANAGER_H_
#define PRISONSTAYMANAGER_H_

#include "AbstractStayManager.h"

namespace mrsa {

class PrisonStayManager : public AbstractStayManager {

public:
	PrisonStayManager();
	virtual ~PrisonStayManager();

	/**
	 * Creates a prison stay for the specified year, and of the specified duration.
	 */
	void createStayFor(int year, int duration);
};

} /* namespace mrsa */
#endif /* PRISONSTAYMANAGER_H_ */
