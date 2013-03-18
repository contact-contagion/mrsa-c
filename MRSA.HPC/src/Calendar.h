/*
 * Calendar.h
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#ifndef CALENDAR_H_
#define CALENDAR_H_

namespace mrsa {

struct Calendar {

	int hour_of_day, day_of_week, day_of_year, year;

	Calendar();
	void increment();
	bool isWeekDay();
};

} /* namespace mrsa */
#endif /* CALENDAR_H_ */
