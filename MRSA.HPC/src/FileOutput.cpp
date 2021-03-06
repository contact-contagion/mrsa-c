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
 * FileOutput.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: nick
 */
#define BOOST_FILESYSTEM_VERSION 2

#include <boost/filesystem.hpp>
#include <sstream>

#include "FileOutput.h"


namespace fs = boost::filesystem;

namespace mrsa {

FileOutput* FileOutput::instance_ = 0;

void FileOutput::initialize(const std::string& filename) {
	// if the instance exists, delete the old one.
	if (instance_ != 0) {
		delete instance_;
	}
	// create the one.
	instance_ = new FileOutput(filename);
}

FileOutput* FileOutput::instance() {
	if (instance_ == 0) {
		throw std::domain_error(
				"FileOutput must be initialized before instance() is called");
	}
	return instance_;
}

FileOutput::FileOutput(const std::string& filename) :
		out(), open(true) {

	fs::path filepath(filename);
	if (!fs::exists(filepath.parent_path()))
		fs::create_directories(filepath.parent_path());
	int i = 1;
	std::string stem = filepath.stem();
	while (fs::exists(filepath)) { // This will increment i until it hits a unique name
		i++;
		std::stringstream ss;
		ss << stem << "_" << i << filepath.extension();
		fs::path newName(filepath.parent_path() / ss.str());
		filepath = newName;
	}
	out.open(filepath.string().c_str());
}


void FileOutput::close() {
	if (open) {
		out.flush();
		out.close();
		open = false;
	}
}

FileOutput::~FileOutput() {
	close();
}

std::ostream& FileOutput::operator<<(const std::string& val) {
	if (open) {
		out << val;
	}
	return out;
}

std::ostream& FileOutput::operator<<(unsigned int val) {
	if (open) {
		out << val;
	}
	return out;
}

std::ostream& FileOutput::operator<<(double val) {
	if (open) {
		out << val;
	}
	return out;
}

} /* namespace mrsa */
