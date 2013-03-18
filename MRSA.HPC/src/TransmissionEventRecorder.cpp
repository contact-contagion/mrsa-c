/*
 * TransmissionEventRecorder.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: nick
 */

#define BOOST_FILESYSTEM_VERSION 2

#include <boost/filesystem.hpp>

#include "TransmissionEventRecorder.h"

namespace fs = boost::filesystem;

namespace mrsa {

TransmissionEventRecorder* TransmissionEventRecorder::instance_ = 0;

void TransmissionEventRecorder::initialize(const std::string& filename) {
	// if the instance exists, delete the old one.
	if (instance_ != 0) {
		delete instance_;
	}
	// create the one.
	instance_ = new TransmissionEventRecorder(filename);
}

TransmissionEventRecorder* TransmissionEventRecorder::instance() {
	if (instance_ == 0) {
		throw std::domain_error(
				"TransmissionEventRecorder must be initialized before instance() is called");
	}
	return instance_;
}

TransmissionEventRecorder::TransmissionEventRecorder(const std::string& filename) :
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
	out << "tick,person_id,place_id,place_type,zip_code,event_type" << std::endl;
	out.flush();
}

void TransmissionEventRecorder::recordEvent(double tick, Person* person, Place* place,
		const std::string& event) {
	out << tick << "," << person->personId() << "," << place->placeId() << "," << place->placeType()
			<< "," << person->zipCode() << "," << event << std::endl;
}

void TransmissionEventRecorder::close() {
	if (open) {
		out.flush();
		out.close();
		open = false;
	}
}

TransmissionEventRecorder::~TransmissionEventRecorder() {
	close();
}

} /* namespace mrsa */
