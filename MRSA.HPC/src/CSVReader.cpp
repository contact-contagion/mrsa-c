/*
 * CSVReader.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: nick
 */

#include "CSVReader.h"

#include <boost/tokenizer.hpp>

namespace mrsa {

using namespace std;
using namespace boost;

CSVReader::CSVReader(const string& file) : fname(file),
		in(file.c_str()) {
	if (!in.is_open()) std::cerr << "Error opening " << file << std::endl;
}

CSVReader::CSVReader(const CSVReader& reader) : fname(reader.fname), in(reader.fname.c_str()) {
	if (!in.is_open()) std::cerr << "Error opening " << fname << std::endl;
}

bool CSVReader::next(vector<string>& vec) {
	string line;
	if (getline(in, line)) {
		tokenizer<escaped_list_separator<char> > tok(line);
		vec.assign(tok.begin(), tok.end());
		return true;
	}
	return false;

}

CSVReader::~CSVReader() {
	in.close();
}

}

