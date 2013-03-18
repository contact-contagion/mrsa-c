/*
 * PersonStatus.h
 *
 *  Created on: Apr 25, 2012
 *      Author: nick
 */

#ifndef DISEASESTATUS_H_
#define DISEASESTATUS_H_

namespace mrsa {

/**
 * Enum for the status of the disease with respect to Persons.
 */
enum DiseaseStatus {
	UNCOLONIZED, COLONIZED, INFECTED
};

enum InfectionStatus {
	NONE, SELF_CARE, SEEK_CARE
};

enum ColonizationCause {
	NA, C_FROM_C, C_FROM_I
};

}

#endif /* DISEASESTATUS_H_ */
