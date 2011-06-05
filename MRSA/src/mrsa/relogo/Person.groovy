package mrsa.relogo
import bsh.This;

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
	
	// The person's relationship to the head of the household.
	public int relate = 0
	
	// The person's gender.
	public int sex = 0
	
	// The person's age.
	public int age = 0
	
	// The person's current place.
	public Place currentPlace = null;
	
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
	
	// The person's infection status.
	public PersonStatus status = PersonStatus.UNCOLONIZED
	
	/* This routine sends the person to their household.
	 *
	 * @author Michael J. North
	 *
	 */
	def goToHH() {
		
		// Go to the household.
		if (hh != null) changePlace(hh)
		
	}
	
	/* This routine sends the person to their group quarters.
	 *
	 * @author Michael J. North
	 *
	 */
	def goToGQ() {
		
		// Go to the group quarters.
		if (gq != null) changePlace(gq)
		
	}
	
	/* This routine sends the person to their home.
	 *
	 * @author Michael J. North
	 *
	 */
	def goToHHorGQ() {
		
		// Go to the household.
		if (hh != null) {
			changePlace(hh)
			// Go to the group quarters.
		} else if (gq != null) {
			changePlace(gq)
		}
	}
	
	/* This routine sends the person to work.
	 *
	 * @author Michael J. North
	 *
	 */
	def goToWork() {
		
		// Go to work.
		if (work != null) changePlace(work)
		
	}
	
	/* This routine sends the person to school.
	 *
	 * @author Michael J. North
	 *
	 */
	def goToSchool() {
		
		// Go to school.
		if (school != null) changePlace(school)
		
	}
	
	/* This routine changes the person's place.
	 *
	 * @author Michael J. North
	 *
	 */
	def changePlace(Place newPlace) {
		
		// Update the old place, if it is defined.
		if (currentPlace != null) {
			
			// Update the old place.
			switch (status) {
			case PersonStatus.UNCOLONIZED:
				currentPlace.uncolonized--
				break
			case PersonStatus.COLONIZED:
				currentPlace.colonized--
				break
			case PersonStatus.INFECTED:
				currentPlace.infected--
				break
			}
			
		}
		
		// Update the place marker.
		currentPlace = newPlace
		
		// Update the old place, if it is defined.
		if (currentPlace != null) {
			
			// Update the new place.
			switch (status) {
			case PersonStatus.UNCOLONIZED:
				currentPlace.uncolonized++
				break
			case PersonStatus.COLONIZED:
				currentPlace.colonized++
				break
			case PersonStatus.INFECTED:
				currentPlace.infected++
				break
			}
			
		}
		
	}
	
	/* This routine performs a simple transition.
	 *
	 * @author Michael J. North
	 *
	 */
	public void activateSimpleTransition() {
		
		// Calculate the person's exposure risk and impact.
		double risk = act.risk()
		
		// Calculate the person's exposure impact.
		if (!allQ(inRadius(persons(), 0.1), { !infected })) {
			
			// At least one person in the area is infected.
			if (random(maximumRisk) < risk) {
				infect()
			}
		}
	}
	
	/* This routine performs a detailed transition.
	 *
	 * @author Michael J. North
	 *
	 */
	public void activateDetailedTransition() {

		// Find the next state.
		PersonStatus other = PersonStatus.UNCOLONIZED
		if (currentPlace != null) {
			if (currentPlace.infected > 0) {
				other = PersonStatus.INFECTED 
			} else if (currentPlace.colonized >0)
				other = PersonStatus.COLONIZED
		}
		
		// Transition.
		PersonStatus nextStatus = TransitionMatrix.next(status, other)
		if (nextStatus != status) {
			if (nextStatus == PersonState.UNCOLONIZED) {
				decolonize()
			} else if (nextStatus == PersonState.COLONIZED) {
				colonize()
			} else if (nextStatus == PersonState.INFECTED) {
				infect()
			}
		}

	}
	
	/* This routine notes a decolonization.
	*
	* @author Michael J. North
	*
	*/
   def decolonize() {

	   // Update the counts.
	   if (currentPlace != null) {
		   if (status == PersonState.COLONIZED) {
			   	currentPlace.colonized--
		   } else if (status == PersonState.INFECTED) {
			   	currentPlace.infected--
		   }
		   currentPlace.uncolonized++
	   }

	   // Update the person's status.	   
	   status = PersonStatus.UNCOLONIZED
	   setColor(Utility.orange())
	   setSize(1.00)

   }

   	/* This routine notes a colonization.
	*
	* @author Michael J. North
	*
	*/
   def colonize() {
	   
	   // Update the counts.
	   if (currentPlace != null) {
		   if (status == PersonState.DECOLONIZED) {
			   	currentPlace.uncolonized--
		   } else if (status == PersonState.INFECTED) {
			   	currentPlace.infected--
		   }
		   currentPlace.colonized++
	   }

	   // Update the person's status.	   
	   status = PersonStatus.COLONIZED
	   setColor(Utility.orange())
	   setSize(1.25)
	   
   }

	
	/* This routine notes an infection.
	 *
	 * @author Michael J. North
	 *
	 */
	def infect() {

	   // Update the counts.
	   if (currentPlace != null) {
		   if (status == PersonState.UNCOLONIZED) {
			   	currentPlace.uncolonized--
		   } else if (status == PersonState.COLONIZED) {
			   	currentPlace.colonized--
		   }
		   currentPlace.infected++
	   }

	   // Update the person's status.	   
		status = PersonStatus.INFECTED
		setColor(Utility.red())
		setSize(1.75)
		
	}
	
}
