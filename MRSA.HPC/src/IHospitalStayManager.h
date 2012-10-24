/*
 * IHospitalStayManager.h
 *
 *  Created on: Oct 24, 2012
 *      Author: nick
 */

#ifndef IHOSPITALSTAYMANAGER_H_
#define IHOSPITALSTAYMANAGER_H_


namespace mrsa {


class IHospitalStayManager {

public:
	IHospitalStayManager() {}
	virtual ~IHospitalStayManager() {}

	virtual bool inHospital(int year, int day) = 0;
};

}


#endif /* IHOSPITALSTAYMANAGER_H_ */
