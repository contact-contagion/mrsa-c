/*
 * HospitalStayManager.h
 *
 *  Created on: Oct 24, 2012
 *      Author: nick
 */

#ifndef HOSPITALSTAYMANAGER_H_
#define HOSPITALSTAYMANAGER_H_

#include <utility>
#include <vector>

#include "AbstractStayManager.h"

namespace mrsa {

class HospitalStayManager : public AbstractStayManager {

public:
	HospitalStayManager(unsigned int y1_length, unsigned int y2_length, unsigned int y3_length, unsigned int y4_length,
			unsigned int y5_length);

	virtual ~HospitalStayManager();
};

} /* namespace mrsa */
#endif /* HOSPITALSTAYMANAGER_H_ */
