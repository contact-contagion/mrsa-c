package mrsa.relogo

import static repast.simphony.relogo.Utility.*;
import static repast.simphony.relogo.UtilityG.*;
import repast.simphony.relogo.BasePatch;
import repast.simphony.relogo.BaseTurtle;
import repast.simphony.relogo.Plural;
import repast.simphony.relogo.Stop;
import repast.simphony.relogo.Utility;
import repast.simphony.relogo.UtilityG;

// The person class.
class Person extends BaseTurtle {
	
	// The person's identifier.
	public String person_id = ""
	
	// The person's relate value.
	public int relate = 0
	
	// The person's gender.
	public int sex = 0
	
	// The person's age.
	public int age = 0

	// The person's home.
	public Place hh = null

	// The person's home identifier.
	public String hh_id = ""

	// The person's group quarters.
	public Place gq = null

	// The person's group quarters' identifier.
	public String gq_id = ""

	// The person's workplace.
	public Place work = null

	// The person's workplace.
	public String work_id = ""

	// The person's school.
	public Place school = null
	
	// The person's school identifier.
	public String school_id = ""

	// The person's activity type identifier.
	public String tucaseid = ""
	
	// THe person's infection status.
	public boolean infected = false;
	
	/* This routine sends the person to their household.
 	 *
	 * @author Michael J. North
	 *
	 */
	def goToHH() {
		
		// Go to the household.
		if (hh != null) moveTo(hh)
		
	}

	/* This routine sends the person to their group quarters.
	 *
	 * @author Michael J. North
	 *
	 */
	def goToGQ() {
		
		// Go to the group quarters.
		if (gq != null) moveTo(gq)
		
	}

	/* This routine sends the person to work.
	 *
	 * @author Michael J. North
	 *
	 */
	def goToWork() {
		
		// Go to work.
		if (work != null) moveTo(work)
		
	}

	/* This routine sends the person to school.
	 *
	 * @author Michael J. North
	 *
	 */
	def goToSchool() {
		
		// Go to school.
		if (school != null) moveTo(school)
		
	}
	
	/* This routine notes an infection.
	 *
	 * @author Michael J. North
	 *
	 */
	def infect() {
		infected = true
		setColor(Utility.red())
	    setSize(1.5)
	}

}
