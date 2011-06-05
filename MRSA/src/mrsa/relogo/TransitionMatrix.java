package mrsa.relogo;

public class TransitionMatrix {

	/*
	 * This routine finds the next state.
	 * 
	 * @author Michael J. North
	 */
	public static PersonStatus next(PersonStatus start, PersonStatus other) {
		
		// Find the next state.
		switch (start) {

		case PersonState.UNCOLONIZED:
			switch (other) {
			case PersonState.UNCOLONIZED:
				return chooseOne(1, 0, 0) 
				break
			case PersonState.COLONIZED:
				return chooseOne(E, 1-B-E, B) 
				break
			case PersonState.INFECTED:
				return chooseOne(D, C, 1-C-D) 
				break
			}
			
		case PersonState.COLONIZED:
			switch (other) {
			case PersonState.UNCOLONIZED:
				return chooseOne(1-A, A, 0) 
				break
			case PersonState.COLONIZED:
				return chooseOne(E, 1-B-E, B) 
				break
			case PersonState.INFECTED:
				return chooseOne(D, C, 1-C-D) 
				break
			}

		case PersonState.INFECTED:
			switch (other) {
			case PersonState.UNCOLONIZED:
				return chooseOne(1-2*A, 2*A, 0) 
				break
			case PersonState.COLONIZED:
				return chooseOne(E, 1-B-E, B) 
				break
			case PersonState.INFECTED:
				return chooseOne(D, C, 1-C-D) 
				break
			}
		
		}
		
		return PersonStatus.UNCOLONIZED
	}

	/*
	 * This routine chooses the next state.
	 * 
	 * @author Michael J. North
	 */
	public static PersonStatus chooseOne(double p1, double p2, double p3) {
		
		// Select the next state.
		double draw = randomFloat(1)
		if (draw <= p1) {
			return PersonStatus.UNCOLONIZED
		} else if (draw <= p2) {
			return PersonStatus.COLONIZED
		} else {
			return PersonStatus.INFECTED
		}
		
	}

}
