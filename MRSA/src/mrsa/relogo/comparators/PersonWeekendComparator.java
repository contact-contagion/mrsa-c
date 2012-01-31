package mrsa.relogo.comparators

import java.util.Comparator

import mrsa.relogo.Person

// The person weekend activity schedule comparator.
class PersonWeekendComparator implements Comparator<Person> {

	/* This routine compares two people based on their weekend activity schedules.
	 *
	 * @author Michael J. North
	 *
	 */
	@Override
	public int compare(Person o1, Person o2) {

		// Compare the people using the case identifiers.
		return (o1.tucaseid_weekend.compareTo(o2.tucaseid_weekend))
	
	}
	
}
