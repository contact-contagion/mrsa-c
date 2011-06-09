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
	
	// The person's activity type identifier.
	public String tucaseid = ""
	
	// The person's infection status.
	public PersonStatus status = PersonStatus.UNCOLONIZED
	
	// The person's activities.
	ActivityList activityList = new ActivityList()
	
	// The person's current activity.
	Activity currentActivity = null
	
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
			
			// Update the place marker.
			currentPlace = newPlace
			if ((showPersonMovement.equalsIgnoreCase('Yes')) || (ticks() == 0)) {
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
		
		// Check the status.
		if (status != PersonStatus.UNCOLONIZED) {
			
			// Update the person's status and appearance.	   
			status = PersonStatus.UNCOLONIZED
		}
	}
	
	/* This routine notes a colonization.
	 *
	 * @author Michael J. North
	 *
	 */
	def colonize() {
		
		// Check the status.
		if (status != PersonStatus.COLONIZED) {
			
			// Update the person's status and appearance.	   
			status = PersonStatus.COLONIZED
		}
	}	
	
	/* This routine notes an infection.
	 *
	 * @author Michael J. North
	 *
	 */
	def infect() {
		
		// Check the status.
		if (status != PersonStatus.INFECTED) {
			
			// Update the person's status and appearance.	   
			status = PersonStatus.INFECTED
		}
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
	public void activateDetailedTransition(Activity activity) {
		
		// Calculate the person's exposure risk and impact, if needed.
		if ((activity != null) && (currentPlace != null)) {
			
			// Find the response rate.
			boolean fasterResponse = false
			if ((hh != null) && (hh.fasterResponse)) {
				fasterResponse = true
				println("fasterResponse for " + person_id + " in " + hh_id)
			}
			
			// Find the next state.
			PersonStatus other = PersonStatus.UNCOLONIZED
			if (currentPlace.infected > 0) {
				other = PersonStatus.INFECTED
			} else if (currentPlace.colonized > 0) {
				other = PersonStatus.COLONIZED
			}
			
			// Transition.
			PersonStatus nextStatus = nextState(status, other,
					activity.risk(), fasterResponse)
			if (nextStatus == PersonStatus.UNCOLONIZED) {
				decolonize()
			} else if (nextStatus == PersonStatus.COLONIZED) {
				colonize()
			} else if (nextStatus == PersonStatus.INFECTED) {
				infect()
			}
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
			
			case PersonStatus.UNCOLONIZED:
				switch (otherStatus) {
					case PersonStatus.UNCOLONIZED:
					return chooseOne(1, 0, 0, activityRisk)
					break
					case PersonStatus.COLONIZED:
					return chooseOne(1-a, a, 0, activityRisk)
					break
					case PersonStatus.INFECTED:
					return chooseOne(1-2*a, 2*a, 0, activityRisk)
					break
				}
			
			case PersonStatus.COLONIZED:
				return chooseOne(e, 1-b-e, b, 1)
			
			case PersonStatus.INFECTED:
				if (fasterResponse) {
					double s = fasterResponseScalingFactor
					return chooseOne(s*d, s*c, 1-s*(c+d), 1)
				} else {
					return chooseOne(d, c, 1-c-d, 1)
				}
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
		
		// Select the next state.
		double draw = randomFloat(1.0)
		if (draw <= (risk * p3)) {
			return PersonStatus.INFECTED
		} else if (draw <= (risk * (p2 + p3))) {
			return PersonStatus.COLONIZED
		} else {
			return PersonStatus.UNCOLONIZED
		}
	}
	
	// The comparator
	@Override
	public int compareTo(Object obj) {
		
		// Check the object.
		if ((this.tucaseid != null) && (obj instanceof Person)) {
			Person otherPerson = (Person) obj;
			return this.tucaseid.compareTo(otherPerson.tucaseid);
		} else {
			return 0;
		}
	}
}
