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
class Activity {
	
	// The activity's type identifier.
	public String tucaseid = ""

	// The activity's place type.
	public String place_type = ""
	
	// The activity's beginning time.
	public int start_time = 0
	
	// The activity's ending time.
	public int stop_time = 0
	
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

}
