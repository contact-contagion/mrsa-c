/*
 * FileOutput.h
 *
 *  Created on: Mar 19, 2013
 *      Author: nick
 */

#ifndef FILEOUTPUT_MRSA_H_
#define FILEOUTPUT_MRSA_H_

#include <fstream>
#include <string>

namespace mrsa {

/**
 *
 */
class FileOutput {
public:

	std::ostream& operator<<(const std::string& val);
	std::ostream& operator<<(double val);
	std::ostream& operator<<(unsigned int val);

	virtual ~FileOutput();

	/**
	 * Initialize the singleton.
	 */
	static void initialize(const std::string& filename);

	/**
	 * Gets singleton instance. If it has not been initialized, an exception
	 * is thrown.
	 */
	static FileOutput* instance();

	/**
	 * Closes the recorder and writes any unwritten data to the file.
	 */
	void close();

private:
	FileOutput(const std::string& filename);
	static FileOutput* instance_;

	std::ofstream out;
	bool open;
};

} /* namespace mrsa */
#endif /* FILEOUTPUT_MRSA_H_ */
