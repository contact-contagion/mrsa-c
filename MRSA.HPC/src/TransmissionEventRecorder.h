/*
 * TransmissionEventRecorder.h
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#ifndef TRANSMISSIONEVENTRECORDER_H_
#define TRANSMISSIONEVENTRECORDER_H_

#include <vector>
#include <string>
#include <fstream>

#include "Person.h"
#include "Place.h"

namespace mrsa {

const std::string U_TO_C = "UNCOLONIZED TO COLONIZED";
const std::string C_TO_U = "COLONIZED TO UNCOLONIZED";
const std::string C_TO_I = "COLONIZED TO INFECTED";
const std::string I_TO_C = "INFECTED TO COLONIZED";
const std::string I_TO_U = "INFECTED TO UNCOLONIZED";

class TransmissionEventRecorder {

public:

	virtual ~TransmissionEventRecorder();

	/**
	 * Initialize the singleton.
	 */
	static void initialize(const std::string& filename);

	/**
	 * Gets singleton instance. If it has not been initialized, an exception
	 * is thrown.
	 */
	static TransmissionEventRecorder* instance();

	/**
	 * Closes the recorder and writes any unwritten data to the file.
	 */
	void close();

	void recordEvent(double tick, Person* person, Place* place, const std::string& event);

private:
	TransmissionEventRecorder(const std::string& filename);
	static TransmissionEventRecorder* instance_;

	std::ofstream out;
	bool open;
};

} /* namespace mrsa */
#endif /* TRANSMISSIONEVENTRECORDER_H_ */
