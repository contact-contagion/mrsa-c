#include <boost/algorithm/string.hpp>
#include <sstream>
#include "repast_hpc/io.h"
#include "repast_hpc/Utilities.h"

#include "utility.h"
#include "CSVReader.h"

namespace mrsa {

const int RISK_PLACE_TYPE_IDX = 0;
const int RISK_ACT_TYPE_IDX = 1;
const int RISK_PAR_IDX = 2;
const int RISK_TIP_IDX = 3;
const int RISK_AIP_IDX = 4;
const int RISK_X_IDX = 5;

void load_risks(repast::Properties& props) {
  if(!props.contains("risk.file")) return;
  std::string risk_file = props.getProperty("risk.file");
  CSVReader reader(risk_file);
  std::vector<std::string> vec;
  std::set<std::string> placeNamesSet;
  std::stringstream placeNames;

  // skip the first header line.
  reader.next(vec);

  // read each line and depending on the type, create that type of place.
  while (reader.next(vec)) {
    std::string type = vec[RISK_PLACE_TYPE_IDX];
    if((placeNamesSet.insert(type).second)){
      if(placeNames.str().size() > 0) placeNames << ",";
      placeNames << type;
    }

    boost::replace_all(type, " ", "_");

    int act_type = repast::strToInt(vec[RISK_ACT_TYPE_IDX]);

    std::stringstream PAR;
    PAR << type << "_" << act_type << "_PAR";
    if(!props.contains(PAR.str()))
        props.putProperty(PAR.str(), (float) repast::strToDouble(vec[RISK_PAR_IDX]));

    std::stringstream TIP;
    TIP << type << "_" << act_type << "_TIP";
    if(!props.contains(TIP.str()))
        props.putProperty(TIP.str(), (float) repast::strToDouble(vec[RISK_TIP_IDX]));

    std::stringstream AIP;
    AIP << type << "_" << act_type << "_AIP";
    if(!props.contains(AIP.str()))
        props.putProperty(AIP.str(), (float) repast::strToDouble(vec[RISK_AIP_IDX]));

    std::stringstream X;
    X << type << "_" << act_type << "_X";
    if(!props.contains(X.str()))
    	props.putProperty(X.str(), (float) repast::strToDouble(vec[RISK_X_IDX]));

    props.putProperty("place.types", placeNames.str());

  }
}

}


