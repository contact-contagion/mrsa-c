package mrsa.relogo;

// The activity class.
class Activity {
	
	// The activity's type identifier.
	public String tucaseid = "";

	// The activity's place type.
	public String place_type = "";
	
	// The activity's beginning time.
	public int start_time = 0;
	
	// The activity's ending time.
	public int stop_time = 0;
	
	// The activity's contagion risk.
	public int activity_risk = 0;
	
	// The activity's place's contagion risk.
	public int place_risk = 0;
	
	// Calculate the activity's total risk.
	double risk() {
		
		// Return the results.
		return (activity_risk + place_risk);
		
	}

}
