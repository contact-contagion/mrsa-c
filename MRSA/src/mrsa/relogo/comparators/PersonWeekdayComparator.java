package mrsa.relogo.comparators;

import java.util.Comparator;

import mrsa.relogo.Person;

// The person weekday activity schedule comparator.
class PersonWeekdayComparator implements Comparator<Person> {

	/* This routine compares two people based on their weekday activity schedules.
	 *
	 * @author Michael J. North
	 *
	 */
	@Override
	public int compare(Person o1, Person o2) {

		// Compare the people using the case identifiers.
		return (o1.tucaseid_weekday.compareTo(o2.tucaseid_weekday));
	
	}
	
}
