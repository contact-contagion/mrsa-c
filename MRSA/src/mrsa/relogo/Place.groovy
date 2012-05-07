package mrsa.relogo

import static repast.simphony.relogo.Utility.*;
import static repast.simphony.relogo.UtilityG.*;
import repast.simphony.relogo.BasePatch;
import repast.simphony.relogo.BaseTurtle;
import repast.simphony.relogo.Plural;
import repast.simphony.relogo.Stop;
import repast.simphony.relogo.Utility;
import repast.simphony.relogo.UtilityG;

import repast.simphony.random.RandomHelper;

// The place class.
abstract class Place extends BaseTurtle {

	// id of the Place.
	String place_id = ""

	// The type of location.
	String place_type = ""

	// The latitude. This is normalized to a Y coordinate.
	double latitude = 0

	// The longitude. This is normalized to an X coordinate.
	double longitude = 0

	// the risk associated with this place
	float risk;

	/**
	 * Adds a Person to this place.
	 */
	public abstract void addPerson(Person p);

	/**
	 * Runs the disease transmission algorithm appropriate to
	 * this Place implementation.
	 */
	public abstract void runTransmission();

	/**
	 * Resets this Place back to an unoccupied status.
	 */
	public abstract void reset();

	protected PersonStatus runTransmission(int infected, int colonized, PersonStatus currentStatus, float risk) {
		// Depending on the current status, run the algorithm for an uncolonized,
		// colonized, or infected person
		if (currentStatus == PersonStatus.UNCOLONIZED) {
			return runUncolonized(risk, infected, colonized);
		} else if (currentStatus == PersonStatus.COLONIZED) {
			return runColonized();
		} else if (currentStatus == PersonStatus.INFECTED) {
			return runInfected();
		}

		return currentStatus;
	}

	// the algorithm for uncolonized persons.
	private runUncolonized(float risk, int infected, int colonized) {

		// default return value.
		PersonStatus ret = PersonStatus.UNCOLONIZED
		if (infected > 0) {
			// set the return value based on the results of a random draw
			// vs. the risk and a_ * 2
			double draw = RandomHelper.nextDouble();
			ret = draw <= (2 * risk * a) ? PersonStatus.COLONIZED : PersonStatus.UNCOLONIZED;
			//if (ret == COLONIZED) ++from_infection;
		} else if (colonized > 0) {
			// set the return value based on the results of a random draw
			// vs. the risk and a_
			double draw = RandomHelper.nextDouble();
			ret = draw <= (risk * a) ? PersonStatus.COLONIZED : PersonStatus.UNCOLONIZED;
			//if (ret == COLONIZED) ++from_colonization;
		}

		return ret;
	}

	// the algorithm for colonized persons.
	private PersonStatus runColonized() {
		PersonStatus ret = PersonStatus.COLONIZED;
		double draw = RandomHelper.nextDouble();
		if (draw <= b)
			// move from colonized to infected with a probability of b_
			ret = PersonStatus.INFECTED;
		else if (draw <= e + b)
			// move from colonized to infected with a probability of e_
			// we already tested for <= b_ so only get here if > b_ but
			// <= e_.
			ret = PersonStatus.UNCOLONIZED;

		return ret;
	}

	private PersonStatus runInfected() {

		PersonStatus ret = PersonStatus.INFECTED;
		double draw = RandomHelper.nextDouble();
		if (draw <= d)
			// move from infected to uncolonized with a probability of d_
			ret = PersonStatus.UNCOLONIZED;
		else if (draw <= d + c)
			// move from infected to uncolonized with a probability of c_
			// we already tested for <= d_ so only get here if > d_ but
			// <= c_.
			ret = PersonStatus.COLONIZED;

		return ret;
	}

}
