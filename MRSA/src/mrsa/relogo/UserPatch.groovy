package mrsa.relogo

import static repast.simphony.relogo.Utility.*;
import static repast.simphony.relogo.UtilityG.*;
import repast.simphony.relogo.BasePatch;
import repast.simphony.relogo.Stop;
import repast.simphony.relogo.Utility;
import repast.simphony.relogo.UtilityG;
import repast.simphony.relogo.ast.Diffusible;

class UserPatch extends BasePatch {

	// The patch uncolonized count.
	int uncolonized = 0
	
	// The patch colonized count.
	int colonized = 0
	
	// The patch infected count.
	int infected = 0

}