#ifndef MRSA_UTILITY_H_
#define MRSA_UTILITY_H_

#include "repast_hpc/Properties.h"

namespace mrsa {

/**
 * Loads the initial per zip code colonization and infection fractions
 * into props from the colonization and infection init file.
 */
void load_init_col_inf_fractions(repast::Properties& props);

/**
 * Loads the initial risk values into props from the risk file.
 */
void load_risks(repast::Properties& props);

}


#endif /* MRSA_UTILITY_H_ */
