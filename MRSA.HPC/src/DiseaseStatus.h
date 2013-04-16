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

// this needs to be renamed to transmission cause
enum ColonizationCause {
	NA, C_FROM_C, C_FROM_I, I_FROM_C, C
};

}

#endif /* DISEASESTATUS_H_ */
