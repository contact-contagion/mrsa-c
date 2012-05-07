/**
 * 
 */
package mrsa.relogo

/**
 * A Household place.
 */
class Household extends Place {
	
	private int colonized, infected;
	private List<Person> persons = new ArrayList<Person>();
	
	/* (non-Javadoc)
	 * @see mrsa.relogo.Place#addPerson(mrsa.relogo.Person)
	 */
	@Override
	public void addPerson(Person person) {
		if (person.status == PersonStatus.INFECTED) infected++;
		else if (person.status == PersonStatus.COLONIZED) colonized++;
		persons.add(person);
	}
	
	/* (non-Javadoc)
	 * @see mrsa.relogo.Place#reset()
	 */
	@Override
	public void reset() {
		infected = colonized = 0;
		persons.clear();
	}

	/* (non-Javadoc)
	 * @see mrsa.relogo.Place#runTransmission()
	 */
	@Override
	public void runTransmission() {
		for (Person p : persons) {
			// if the person can transition,
			if (p.canTransition()) {
				// get the new status
				PersonStatus status = runTransmission(infected, colonized, p.status, risk);
				// colonize, decolonize or infect the person depending on the new status.
				if (status == PersonStatus.UNCOLONIZED) p.decolonize();
				else if (status == PersonStatus.COLONIZED) p.colonize();
				else if (status == PersonStatus.INFECTED) p.infect();
			}
		}
		
		
	}
}
