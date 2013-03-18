#include <boost/algorithm/string.hpp>
#include <sstream>
#include "repast_hpc/io.h"
#include "repast_hpc/Utilities.h"

#include "utility.h"
#include "CSVReader.h"
#include "Parameters.h"

namespace mrsa {

const int RISK_PLACE_TYPE_IDX = 0;
const int RISK_ACT_TYPE_IDX = 1;
const int RISK_PAR_IDX = 2;
const int RISK_TIP_IDX = 3;
const int RISK_AIP_IDX = 4;
const int RISK_X_IDX = 5;

void load_init_col_inf_fractions(repast::Properties& props) {
	if (!props.contains(COL_INFECT_INIT_FILE))
		return;
	std::string init_file = props.getProperty(COL_INFECT_INIT_FILE);
	CSVReader reader(init_file);
	std::vector<std::string> vec;

	// skip the first header line.
	reader.next(vec);
	while (reader.next(vec)) {
		std::string zip = repast::trim(vec[0]);
		props.putProperty(zip + "_colonized", repast::strToDouble(repast::trim(vec[1])));
		props.putProperty(zip + "_infected", repast::strToDouble(repast::trim(vec[2])));
	}
}

void load_risks(repast::Properties& props) {
	if (!props.contains(RISK_FILE))
		return;
	std::string risk_file = props.getProperty(RISK_FILE);
	CSVReader reader(risk_file);
	std::vector<std::string> vec;
	std::set<std::string> placeNamesSet;
	std::stringstream placeNames;

	// skip the first header line.
	reader.next(vec);

	// read each line and depending on the type, create that type of place.
	while (reader.next(vec)) {
		std::string type = vec[RISK_PLACE_TYPE_IDX];
		if ((placeNamesSet.insert(type).second)) {
			if (placeNames.str().size() > 0)
				placeNames << ",";
			placeNames << type;
		}

		boost::replace_all(type, " ", "_");

		int act_type = repast::strToInt(vec[RISK_ACT_TYPE_IDX]);

		std::stringstream PAR;
		PAR << type << "_" << act_type << "_PAR";
		if (!props.contains(PAR.str()))
			props.putProperty(PAR.str(), (float) repast::strToDouble(vec[RISK_PAR_IDX]));

		std::stringstream TIP;
		TIP << type << "_" << act_type << "_TIP";
		if (!props.contains(TIP.str()))
			props.putProperty(TIP.str(), (float) repast::strToDouble(vec[RISK_TIP_IDX]));

		std::stringstream AIP;
		AIP << type << "_" << act_type << "_AIP";
		if (!props.contains(AIP.str()))
			props.putProperty(AIP.str(), (float) repast::strToDouble(vec[RISK_AIP_IDX]));

		std::stringstream X;
		X << type << "_" << act_type << "_X";
		if (!props.contains(X.str()))
			props.putProperty(X.str(), (float) repast::strToDouble(vec[RISK_X_IDX]));

		props.putProperty("place.types", placeNames.str());

	}
}

}

