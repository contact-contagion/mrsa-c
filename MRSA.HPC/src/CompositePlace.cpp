/*
 * CompositePlace.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#include "CompositePlace.h"

namespace mrsa {

using namespace std;

ComponentPlace::ComponentPlace(string id, string type, Risk risk, unsigned int zip) :
		AbstractPlace(id, type, risk, zip) {

}

ComponentPlace::~ComponentPlace() {
}

void ComponentPlace::runTransmission() {
	TransmissionAlgorithm* ta = TransmissionAlgorithm::instance();

	// uncolonized must always go first so that the infected and colonized persons
	// if they cause a colonization of the uncolonized are still in that infected or
	// colonized state.
	for (PersonIter iter = uncolonized.begin(); iter != uncolonized.end(); ++iter) {
		processUncolonized(*iter, ta);
	}
	for (PersonIter iter = infected.begin(); iter != infected.end(); ++iter) {
		processInfected(*iter, ta);
	}

	for (PersonIter iter = colonized.begin(); iter != colonized.end(); ++iter) {
		processColonized(*iter, ta);
	}

//	if (size() > 0) {
//		(*FileOutput::instance())
//				<< repast::RepastProcess::instance()->getScheduleRunner().currentTick() << ","
//				<< id_ << "," << type_ << "," << size() << std::endl;
//	}
}

size_t ComponentPlace::size() {
	return uncolonized.size() + infected.size() + colonized.size();
}

CompositePlace::CompositePlace(std::vector<std::string>& vec, Risk risk,
		unsigned int component_max_size, CompPlaceType pType) :
		Place(vec, risk), components(), component_max_size_(component_max_size), placeType_(pType), comp_index(
				0) {

	// add an initial component.
	components.push_back(new ComponentPlace(id_, type_, risk_, zip_code));
}

CompositePlace::~CompositePlace() {
	// delete the component places.
	for (vector<ComponentPlace*>::iterator iter = components.begin(); iter != components.end();
			++iter) {
		delete (*iter);
	}
}

void CompositePlace::runTransmission() {
	for (vector<ComponentPlace*>::iterator iter = components.begin(); iter != components.end();
			++iter) {
		(*iter)->runTransmission();
	}
}

void CompositePlace::addPerson(Person* person, int activity_type) {
	ComponentPlace* place = components[comp_index];
	place->addPerson(person, activity_type);

	if (place->size() == component_max_size_) {
		++comp_index;
		if (comp_index == components.size())
			components.push_back(new ComponentPlace(id_, type_, risk_, zip_code));
	}
}

void CompositePlace::reset() {
	for (vector<ComponentPlace*>::iterator iter = components.begin(); iter != components.end();
			++iter) {
		(*iter)->reset();
	}
	comp_index = 0;
}

/**
 * Gets the number of persons in the component place at the specified index.
 *
 * @return the number of persons in the component place at the specified index.
 */
size_t CompositePlace::componentSize(size_t index) {
	return components.at(index)->size();
}

} /* namespace mrsa */
