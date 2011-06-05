package mrsa.relogo

import static repast.simphony.relogo.Utility.*;
import static repast.simphony.relogo.UtilityG.*;
import repast.simphony.relogo.BasePatch;
import repast.simphony.relogo.BaseTurtle;
import repast.simphony.relogo.Plural;
import repast.simphony.relogo.Stop;
import repast.simphony.relogo.Utility;
import repast.simphony.relogo.UtilityG;

// The place class.
class Place extends BaseTurtle {

	// The location identifier.
	public String place_id = ""
	
	// The type of location.
	public String type = ""
	
	// The latitude. This is normalized to a Y coordinate.
	public double latitude = 0
	
	// The longitude. This is normalized to an X coordinate.
	public double longitude = 0
	
	// The number of uncolonized people currently here.
	int uncolonized = 0;

	// The number of colonized people currently here.
	int colonized = 0;
	
	// The number of infected people currently here.
	int infected = 0;

}
