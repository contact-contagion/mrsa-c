package mrsa.relogo

import static repast.simphony.relogo.Utility.*;
import static repast.simphony.relogo.UtilityG.*;
import repast.simphony.relogo.BasePatch;
import repast.simphony.relogo.Stop;
import repast.simphony.relogo.Utility;
import repast.simphony.relogo.UtilityG;
import repast.simphony.relogo.ast.Diffusible;

// The user patch base class.
class UserPatch extends BasePatch {

	// The patch uncolonized count.
	int patch_uncolonized = 0
	
	// The patch colonized count.
	int patch_colonized = 0
	
	// The patch infected count.
	int patch_infected = 0

}