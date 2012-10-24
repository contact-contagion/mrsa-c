/*
 * NoStayManager.h
 *
 *  Created on: Oct 24, 2012
 *      Author: nick
 */

#ifndef NOSTAYMANAGER_H_
#define NOSTAYMANAGER_H_

#include "IHospitalStayManager.h"

namespace mrsa {

class NoStayManager : public IHospitalStayManager {

public:

	NoStayManager() : IHospitalStayManager() {}
	virtual ~NoStayManager() {}

	bool inHospital(int year, int day) {
		return false;
	}
};

}



#endif /* NOSTAYMANAGER_H_ */
