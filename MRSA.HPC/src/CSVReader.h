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

class CSVReader {
public:

	CSVReader(const std::string& file);
	CSVReader(const CSVReader& reader);

	virtual ~CSVReader();

	bool next(std::vector<std::string>& vec);

private:
	std::string fname;
	std::ifstream in;
};

}

#endif /* CSVREADER_H_ */
