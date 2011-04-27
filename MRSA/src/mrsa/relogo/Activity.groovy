package mrsa.relogo

import static repast.simphony.relogo.Utility.*;
import static repast.simphony.relogo.UtilityG.*;
import repast.simphony.relogo.BasePatch;
import repast.simphony.relogo.BaseTurtle;
import repast.simphony.relogo.Plural;
import repast.simphony.relogo.Stop;
import repast.simphony.relogo.Utility;
import repast.simphony.relogo.UtilityG;

// The activity class.
class Activity extends BaseTurtle {
	
	// The activity's type identifier.
	public String tucaseid = ""

	// The activity's place type.
	public String place_type = ""
	
	// The activity's beginning time.
	public String start_time = ""
	
	// The activity's ending time.
	public String stop_time = ""
	
	// The activity's code.
	public String activity_code = ""
	
	// The activity's contagion risk.
	public int activity_risk = 0
	
	// The activity's place's contagion risk.
	public int place_risk = 0
	
	// Calculate the activity's total risk.
	double risk() {
		
		// Return the results.
		return (activity_risk + place_risk)
		
	}

	// Parse the activity's start time.
	double getBeginTime() {
		
		// Return the results.
		return (Double.valueOf(start_time.split(':')[0]))
		
	}

	// Parse the activity's start time.
	double getEndTime() {
		
		// Return the results.
		return (Double.valueOf(stop_time.split(':')[0]))
		
	}

}
