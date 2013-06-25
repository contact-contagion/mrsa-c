/*
*MRSA Model
*
*Copyright (c) 2013 University of Chicago and Argonne National Laboratory
*   All rights reserved.
*  
*   Redistribution and use in source and binary forms, with 
*   or without modification, are permitted provided that the following 
*   conditions are met:
*  
*  	 Redistributions of source code must retain the above copyright notice,
*  	 this list of conditions and the following disclaimer.
*  
*  	 Redistributions in binary form must reproduce the above copyright notice,
*  	 this list of conditions and the following disclaimer in the documentation
*  	 and/or other materials provided with the distribution.
*  
*  	 Neither the name of the Argonne National Laboratory nor the names of its
*     contributors may be used to endorse or promote products derived from
*     this software without specific prior written permission.
*  
*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
*   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
*   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
*   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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

CSVReader::CSVReader(const string& file) : fname(file),  delim('\n'),
		in(file.c_str()) {
	// if we can't open the file throw exception
	if (!in.is_open()) throw invalid_argument("Error opening: " + file);
	findDelimeter();

}

CSVReader::CSVReader(const CSVReader& reader) : fname(reader.fname), delim(reader.delim), in(reader.fname.c_str()) {
	// if we can't open the file throw exception
	if (!in.is_open()) throw invalid_argument("Error opening: " + fname);
}

void CSVReader::findDelimeter() {

	char c;
	while (in.good()) {
	    c = in.get();
	    if (in.good()) {
	    	if (c == '\n') {
	    		delim = '\n';
	    		break;
	    	}
	    	if (c == '\r') {
	    		delim = '\r';
	    		break;
	    	}
	    }
	}
	in.close();
	in.open(fname.c_str());
}

void CSVReader::skip(int lines) {
	int count = 0;
	string str;
	while (count < lines && getline(in, str)) {
		++count;
	}
}

bool CSVReader::next(vector<string>& vec) {
	string line;
	// read the line
	if (getline(in, line, delim)) {
		// tokenize the line using boost's escaped list separator
		// which parses CSV format
		tokenizer<escaped_list_separator<char> > tok(line);
		// assign those values to the vector
		vec.assign(tok.begin(), tok.end());
		return true;
	}
	// return false when there are no more lines to
	// read
	return false;
}

// copy the file name and close the existing file
// and reopen it with the new file.
CSVReader& CSVReader::operator=(const CSVReader& rhs) {
	if (&rhs != this) {
		fname = rhs.fname;
		in.close();
		delim = rhs.delim;
		in.open(fname.c_str());
	}

	return *this;
}

CSVReader::~CSVReader() {
	in.close();
}

}

