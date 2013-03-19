/*
 * CompositePlace.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: nick
 */

#include "CompositePlace.h"

namespace mrsa {

using namespace std;

ComponentPlace::ComponentPlace(string id, string type, Risk risk) :
		AbstractPlace(id, type, risk) {

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

	//std::cout << id_ << ": " << size() << std::endl;
}

size_t ComponentPlace::size() {
	return uncolonized.size() + infected.size() + colonized.size();
}

CompositePlace::CompositePlace(std::vector<std::string>& vec, Risk risk,
		unsigned int component_max_size, CompPlaceType pType) :
		Place(vec, risk), components(), component_max_size_(component_max_size), placeType_(pType) {

	// add an initial component.
	components.push_back(new ComponentPlace(id_, type_, risk_));
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

int CompositePlace::addPersonToComponent(Person* person, int activity_type, int compartment_index) {
	if (compartment_index == -1) {
		int index = components.size() - 1;
		ComponentPlace* place = components[components.size() - 1];
		if (place->size() < component_max_size_) {
			place->addPerson(person, activity_type);
		} else {
			// create a new ComponentPlace
			place = new ComponentPlace(id_, type_, risk_);
			place->addPerson(person, activity_type);
			components.push_back(place);
			++index;
		}
		return index;
	} else {
		components[compartment_index]->addPerson(person, activity_type);
		return compartment_index;
	}
}

void CompositePlace::reset() {
	for (vector<ComponentPlace*>::iterator iter = components.begin(); iter != components.end();
			++iter) {
		(*iter)->reset();
	}
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
