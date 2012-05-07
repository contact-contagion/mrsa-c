/**
 * 
 */
package mrsa.relogo

/**
 * A School place.
 */
class School extends Place {

	static class AgeGroup {

		private infected, colonized;
		private List<Person> persons = new ArrayList<Person>();


		/**
		 * Resets this AgeGroup to unoccupied status.
		 */
		void reset() {
			infected = colonized = 0;
			persons.clear();
		}

		/**
		 * Adds a Person to this AgeGroup.
		 */
		void addPerson(Person person) {
			if (person.status == PersonStatus.INFECTED) infected++;
			else if (person.status == PersonStatus.COLONIZED) colonized++;
			persons.add(person);
		}

		/**
		 * Processes People in this AgeGroup.
		 */
		void processPeople(School school) {
			for (Person p : persons) {
				// if the person can transition status
				if (p.canTransition()) {
					// get the new status
					PersonStatus status = school.runTransmission(infected, colonized, p.status, school.risk);
					// colonize, decolonize or infect the person depending on the new status.
					if (status == PersonStatus.UNCOLONIZED) p.decolonize();
					else if (status == PersonStatus.COLONIZED) p.colonize();
					else if (status == PersonStatus.INFECTED) p.infect();
				}
			}
		}
	}

	private Map<Integer, AgeGroup> ageGroups = new HashMap<Integer, AgeGroup>();

	/* (non-Javadoc)
	 * @see mrsa.relogo.Place#addPerson(mrsa.relogo.Person)
	 */
	@Override
	public void addPerson(Person p) {
		AgeGroup group = ageGroups.get(p.age);
		if (group == null) {
			group = new AgeGroup();
			ageGroups.put(p.age, group);
		}
		group.addPerson(p);
	}

	/* (non-Javadoc)
	 * @see mrsa.relogo.Place#reset()
	 */
	@Override
	public void reset() {
		for (AgeGroup group : ageGroups.values()) {
			group.reset();
		}
	}


	public void runTransmission() {
		for (AgeGroup group : ageGroups.values()) {
			group.processPerson(this);
		}
	}

}
