package mrsa.relogo
import PersonStatus;
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
class Person extends BaseTurtle implements Comparable {
	
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
	
	// The person's work place.
	public Place work = null
	
	// The person's work place.
	public String work_id = ""
	
	// The person's school.
	public Place school = null
	
	// The person's school identifier.
	public String school_id = ""
	
	// The person's weekday activity type identifier.
	public String tucaseid_weekday = ""

	// The person's weekend activity type identifier.
	public String tucaseid_weekend = ""
		
	// The person's infection status.
	public PersonStatus status = PersonStatus.UNCOLONIZED
	
	// The person's weekday activities.
	ActivityList activityList_weekday = new ActivityList()
	
	// The person's weekend activities.
	ActivityList activityList_weekend = new ActivityList()

		// The person's current activity.
	Activity currentActivity = null
	
	// The day the person was last infected.
	int lastInfectedDay = 0
	
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
	
	/* This routine sends the person to their home.
	 *
	 * @author Michael J. North
	 *
	 */
	def goToHome() {
		
		// Go to the household.
		if (hh != null) {
			changePlace(hh)
			// Go to the group quarters.
		} else if (gq != null) {
			changePlace(gq)
			// Go to work.
		} else if (work != null) {
			changePlace(work)
			// Go to school.
		} else if (school != null) {
			changePlace(school)
		}
	}
	
	/* This routine changes the person's place.
	 *
	 * @author Michael J. North
	 *
	 */
	def changePlace(Place newPlace) {
		
		// Check for changes.
		if (newPlace != currentPlace) {
			
			// Note the current location.
			currentPlace = newPlace
			
			// Update the display if appropriate.
			if ((showPersonMovement.equalsIgnoreCase('Yes')) || (ticks() == 0)) {
				
				// Update the place marker.
				moveTo(currentPlace)

			}
		}
	}
	
	/* This routine notes decolonization.
	 *
	 * @author Michael J. North
	 *
	 */
	def decolonize() {
		
		// Update the person's status and appearance.	   
		status = PersonStatus.UNCOLONIZED
		
	}
	
	/* This routine notes a colonization.
	 *
	 * @author Michael J. North
	 *
	 */
	def colonize() {
		
		// Update the person's status and appearance.	   
		status = PersonStatus.COLONIZED
		
	}	
	
	/* This routine notes an infection.
	 *
	 * @author Michael J. North
	 *
	 */
	def infect() {
		
		// Note the new last day of infection, if appropriate.
		if (status != PersonStatus.INFECTED) {
			lastInfectedDay = ticks()
		}
		
		// Update the person's status and appearance.	   
		status = PersonStatus.INFECTED
		
	}
	
	/* This routine performs a simple transition.
	 *
	 * @author Michael J. North
	 *
	 */
	public void activateSimpleTransition(Activity activity) {
		
		// Calculate the person's exposure risk and impact, if needed.
		if (activity != null) {
			
			// Calculate the person's exposure risk and impact.
			double risk = activity.risk()
			
			// Calculate the person's exposure impact.
			if (!allQ(inRadius(persons(), 0.1), { !infected })) {
				
				// At least one person in the area is infected.
				if (random(maximumRisk) < risk) {
					infect()
				}
				
			}
			
		}
		
	}
	
	/* This routine performs a detailed transition.
	 *
	 * @author Michael J. North
	 *
	 */
	public void activateDetailedActivityTransition(Activity activity) {
		
	    // Check for an infected person that is not yet ready to move on.
	    if ((status == PersonStatus.INFECTED) &&
		   ((ticks() - lastInfectedDay) <= minimumInfectionPeriod)) {
			   return;
		}
		   
		// Set the default state.
		PersonStatus other = PersonStatus.UNCOLONIZED
		
		// Check for other states if a place is defined.
		if (currentPlace != null) {
			
			// Find out about other people currently at this place.
			if (currentPlace.place_infected > 0) {
				
				// There is an infected person at this location.
				other = PersonStatus.INFECTED
				
			// Check for colonized people. 
			} else if (currentPlace.place_colonized > 0) {

				// There is a colonized person at this location.
				other = PersonStatus.COLONIZED
				
			}
			
		}
		
		// Find the activity risk.
		double activityRisk = 0
		if (activity != null) {
			
			// Note the activity risk.
			activityRisk = activity.risk()
			
		}
		
		// Prepare to find the response rate.
		boolean fasterResponse = false
		
		// Check for a valid household with a faster response.
		if ((hh != null) && (hh.fasterResponse)) {
			
			// Note a faster response rate.
			fasterResponse = true
		}
		
		// Transition to the next state.
		PersonStatus nextStatus = nextState(status, other, activityRisk, fasterResponse)
		
		// Check for a move to an uncolonized state.
		if (nextStatus == PersonStatus.UNCOLONIZED) {
			
			// Move into a decolonized state.
			decolonize()
		
		// Check for a move to an colonized state.
		} else if (nextStatus == PersonStatus.COLONIZED) {
		
			// Move into a colonized state.
			colonize()
			
		// Check for a move to an infected state.
		} else if (nextStatus == PersonStatus.INFECTED) {
		
			// Move into an infected state.
			infect()
		}

	}
	
	/* This routine performs a detailed transition.
	*
	* @author Michael J. North
	*
	*/
   public void activateDetailedPlaceTransition() {
	   
	   // Check for an infected person that is not yet ready to move on.
	   if ((status == PersonStatus.INFECTED) &&
		  ((ticks() - lastInfectedDay) <= minimumInfectionPeriod)) {
			   return;
	   }
		   
	   // Find out about other people currently at this place.
	   PersonStatus other = PersonStatus.UNCOLONIZED
	   
	   // Check the place.
	   if (currentPlace != null) {
		   
		   // Check the number of infected people.
		   if(currentPlace.place_infected > 0) {
			   
			   // Note one or more infected people.
			   other = PersonStatus.INFECTED
			   
		   // Check the number of colonized people.
		   } else if (currentPlace.place_colonized > 0) {

			   // Note one or more colonized people.
			   other = PersonStatus.COLONIZED

		   }
		   
	   }
	   
	   // Prepare to find the response rate.
	   boolean fasterResponse = false
	   
	   // Check for a valid household with a faster response rate.
	   if ((hh != null) && (hh.fasterResponse)) {
		   
		   // A household with a faster response rate.
		   fasterResponse = true
		   
	   }
	   
		// Transition to the next state.
	   PersonStatus nextStatus = nextState(status, other,
			   currentPlace.risk, fasterResponse)
	   
	  
		// Check for a move to an uncolonized state.
		if (nextStatus == PersonStatus.UNCOLONIZED) {
			
			// Move into a decolonized state.
			decolonize()
		
		// Check for a move to an colonized state.
		} else if (nextStatus == PersonStatus.COLONIZED) {
		
			// Move into a colonized state.
			colonize()
			if (status == PersonStatus.UNCOLONIZED) println("Colonized " + person_id)
			
		// Check for a move to an infected state.
		} else if (nextStatus == PersonStatus.INFECTED) {
		
			// Move into an infected state.
			infect()
			
		}

   }
	
	/*
	 * This routine finds the next state.
	 *
	 * @author Michael J. North
	 */
	public PersonStatus nextState(PersonStatus startStatus,
		PersonStatus otherStatus, double activityRisk, boolean fasterResponse) {
		
		// Find the next state.
		switch (startStatus) {
			
			// Start with a colonized state.
			case PersonStatus.UNCOLONIZED:
			
				// Check the colocated person's status.
				switch (otherStatus) {

					// Deal with uncolonized colocated people.
					case PersonStatus.UNCOLONIZED:
					
						// Choose the next state using the appropriate rates.
						return chooseOne(1, 0, 0, activityRisk)
						
						// Finish the state assignment.
						break
						
					// Deal with colonized colocated people.
					case PersonStatus.COLONIZED:
					
						// Choose the next state using the appropriate rates.
						return chooseOne(1-a, a, 0, activityRisk)

						// Finish the state assignment.
						break
						
					// Deal with infected colocated people.
					case PersonStatus.INFECTED:

						// Choose the next state using the appropriate rates.
						return chooseOne(1-2*a, 2*a, 0, activityRisk)

						// Finish the state assignment.
						break
						
				}
			
			// Deal with colonized colocated people.
			case PersonStatus.COLONIZED:

				// Choose the next state using the appropriate rates.
				return chooseOne(e, 1-b-e, b, 1)
			
			// Deal with uncolonized colocated people.
			case PersonStatus.INFECTED:
			
				// Check for faster response rates.
				if (fasterResponse) {
					
					// Note the faster response rate scaling factor.
					double s = fasterResponseScalingFactor
					
					// Choose the next state using the appropriate rates.
					return chooseOne(s*d, s*c, 1-s*(c+d), 1)
					
				} else {
				
					// Choose the next state using the appropriate rates.
					return chooseOne(d, c, 1-c-d, 1)
					
				}
								
				// Finish the state assignment.
				break
		}
		
		// Return the default if no other matches are found.
		return PersonStatus.UNCOLONIZED
		
	}
	
	/*
	 * This routine chooses the next state.
	 *
	 * @author Michael J. North
	 */
	public PersonStatus chooseOne(double p1, double p2, double p3, double risk) {

		// Draw a new random threshold.		
		double draw = randomFloat(1.0)
		
		// Select the next state.
		// Check for an infected state draw.
		if (draw < (risk * p3)) {
			
			// Select an infected state.
			return PersonStatus.INFECTED
		
		// Check for a colonized state draw.
		} else if (draw < (risk * (p2 + p3))) {

			// Select a colonized state.
			return PersonStatus.COLONIZED
			
		} else {
		
			// Select an uncolonized state.
			return PersonStatus.UNCOLONIZED
			
		}
		
	}

}
