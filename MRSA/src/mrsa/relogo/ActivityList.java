package mrsa.relogo;

import java.util.ArrayList;

// The activity list class.
public class ActivityList extends ArrayList<Activity> implements Comparable {
	
	// Return case identifier, if defined.
	public String getTucaseid() {
		if (this.size() > 0) {
			return this.get(0).tucaseid;
		} else {
			return null;
		}
	}

	// The comparator
	@Override
	public int compareTo(Object obj) {
		
		// Check the object.
		if (obj instanceof ActivityList) {
			ActivityList otherActivity = (ActivityList) obj;
			return this.getTucaseid().compareTo(otherActivity.getTucaseid());
		} else {
			return 0;
		}
	}
	
}
