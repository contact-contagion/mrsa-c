package mrsa.relogo;

import java.util.ArrayList;

// The activity list class.
public class ActivityList extends ArrayList<Activity> implements Comparable {
	
	// Return case identifier, if defined.
	public String getTucaseid() {
		
		// Check for elements.
		if (this.size() > 0) {
			
			// Compare the case identifier.
			return this.get(0).tucaseid;
			
		} else {
			
			// Signal that there is no element to compare.
			return null;
			
		}
		
	}

	// The comparator method.
	@Override
	public int compareTo(Object obj) {
		
		// Check the object.
		if ((this.getTucaseid() != null) && (obj instanceof ActivityList)) {
			
			// Recast the item to compare.
			ActivityList otherActivity = (ActivityList) obj;
			
			// Compare based on case identifiers.
			return this.getTucaseid().compareTo(otherActivity.getTucaseid());

		} else {
			
			
			// Signal that there is no valid comparison.
			return 0;

		}

	}
	
}
