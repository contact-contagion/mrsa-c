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

std::ostream& FileOutput::operator<<(std::string val) {
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
