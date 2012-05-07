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

	// The hour the person was last infected.
	int lastInfectedHour = 0

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

		// assumes that all the places will be reset
		// and the empty, prior to person's changing place.
		currentPlace.addPerson(this);
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
			lastInfectedHour = ticks()
		}

		// Update the person's status and appearance.
		status = PersonStatus.INFECTED

	}

	/**
	 * Gets whether or not this Person is an valid state for updating its disease state.
	 * For example, the person is in the middle of the infection period, he/she 
	 * cannot be transitioned to a different state.
	 * 
	 * @return true if this Person's disease state can be updated, otherwise false.
	 **/
	boolean canTransition() {
		if ((status == PersonStatus.INFECTED) &&
		((ticks() - lastInfectedHour) <= minimumInfectionPeriod)) {
			return false
		}
		return true;
	}
}
