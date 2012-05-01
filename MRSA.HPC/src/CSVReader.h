/*
 * CSVReader.h
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#ifndef CSVREADER_H_
#define CSVREADER_H_

#include <vector>
#include <string>
#include <fstream>

namespace mrsa {

/**
 * Reads CSV formatted data files. Each line of the file is read into a vector
 * where each element of the vector is a data element from that line.
 */
class CSVReader {
public:

	/**
	 * Create a CSVReader that will read the specified file.
	 */
	CSVReader(const std::string& file);

	// copy constructor
	CSVReader(const CSVReader& reader);

	virtual ~CSVReader();

	/**
	 * Reads the next line into the vector, returning true
	 * if a line was read, otherwise false. This will return
	 * false when it reaaches the end of the file.
	 */
	bool next(std::vector<std::string>& vec);

	// assignment operator
	CSVReader& operator=(const CSVReader& rhs);

private:
	std::string fname;
	std::ifstream in;
};

}

#endif /* CSVREADER_H_ */
