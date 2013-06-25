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

	/**
	 * Skips the specified number of lines in the file.
	 */
	void skip(int lines);

	// assignment operator
	CSVReader& operator=(const CSVReader& rhs);

private:
	void findDelimeter();

	std::string fname;
	char delim;
	std::ifstream in;
};

}

#endif /* CSVREADER_H_ */
