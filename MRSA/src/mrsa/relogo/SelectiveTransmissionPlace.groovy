/**
 * 
 */
package mrsa.relogo

import repast.simphony.random.RandomHelper;

/**
 * A place that randomly selects some number of uncolonized persons
 * for a potential colonization / infection. 
 */
class SelectiveTransmissionPlace extends Place {

	// colonized and infected counts.
	private int colonized, infected;

	// the number of uncolonized persons to run through the
	// transmission algorithm.
	int personsToTry;
	// list of uncolonized persons
	private List<Person> uncolonized = new ArrayList<Person>();
	// everybody else
	private List<Person> others = new ArrayList<Person>();


	/* (non-Javadoc)
	 * @see mrsa.relogo.Place#addPerson(mrsa.relogo.Person)
	 */
	@Override
	public void addPerson(Person person) {
		PersonStatus status = person.status;
		if (status == PersonStatus.UNCOLONIZED) {
			uncolonized.add(person);

		} else 	if (status == PersonStatus.INFECTED) {
			infected++;
			others.add(person);

		} else if (status == PersonStatus.COLONIZED) {
			others.add(person);
			colonized++;
		}
	}

	/* (non-Javadoc)
	 * @see mrsa.relogo.Place#reset()
	 */
	@Override
	public void reset() {
		uncolonized.clear();
		others.clear();
		colonized = infected = 0;
	}

	private void processPerson(Person p) {
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

	/* (non-Javadoc)
	 * @see mrsa.relogo.Place#runTransmission()
	 */
	@Override
	public void runTransmission() {
		// run the transmission algorithm on each non-uncolonized person
		for (Person p : others) {
			processPerson(p);
		}

		if (uncolonized.size() <= personsToTry) {
			for (Person p : uncolonized) {
				processPerson(p);
			}
		} else {
			int limit = uncolonized.size() - 1;
			Set<Person> alreadyProcessed = new HashSet<Person>();
			// select "personsToTry" number of persons and run the transmission algorithm on them.
			for (int i = 0; i < personsToTry; ++i) {
				Person p = uncolonized.get(RandomHelper.nextIntFromTo(0, limit));
				// if p has already been processed then keep drawing
				// until we find an unprocessed p.
				while (alreadyProcessed.contains(p)) {
					p = uncolonized.get(RandomHelper.nextIntFromTo(0, limit));
				}

				processPerson(p);
			}
		}
	}
}
