package mrsa.relogo

import com.sun.tools.jdi.JDWP.ReferenceType.Status;

import static repast.simphony.relogo.Utility.*;
import static repast.simphony.relogo.UtilityG.*;
import repast.simphony.relogo.BaseObserver;
import repast.simphony.relogo.Stop;
import repast.simphony.relogo.Utility;
import repast.simphony.relogo.UtilityG;
import au.com.bytecode.opencsv.CSVReader;

class UserObserver extends BaseObserver {
	
	//TODO: Initialize 5 people with MRSA infection (this number should remain stable throughout the regular runs)
	//TODO: Initialize 3% of the people with MRSA colonization (" ")
	//TODO: For differential run 1 (infected people get medical help faster) cut C and D in half for 50% of the people such that people in the same HH have the same C and D
	//TODO: For differential run 2 (infected people get medical help faster) cut C and D in half for 50% of the people such that people in the same HH have the same C and D and the North half of 60615 averages 75% change and the South half averages 25% change
	
	/* This routine configures the model.
	 * 
	 * @author Michael J. North
	 * 
	 */
	def setup(){
		
		// Clear any current configuration.
		clearAll()
		
		// Read the people and initialize their disease status.
		if (!personsInputFile.equalsIgnoreCase('None')) {
			println("Started Reading Persons")
			createTurtlesFromCSVFile(personsInputFile, Person.class,
					'square', 0, Utility.black())
			println("Completed Reading Persons")
		}
		
		// Read the places.
		if (!placesInputFile.equalsIgnoreCase('None')) {
			println("Started Reading Places")
			createTurtlesFromCSVFile(placesInputFile, Place.class,
					'square', 0.0, Utility.black())
			println("Completed Reading Places")
		}
		
		// Match the people with their places.
		println("Starting Matching Persons and Places")
		matchPeopleAndPlaces()
		println("Completed Matching Persons and Places")
		
		// Normalize the place coordinates.
		println("Started Normalizing Place Locations")
		normalizePlaceCoordinates()
		println("Completed Normalizing Place Locations")
		
		// Start the people at their household.
		println("Started Asking Persons to Begin at Home")
		ask (persons()) {
			
			// Start home.
			goToHome()
			
		}
		println("Completed Asking Persons to Begin at Home")
		
		// Initialize the disease status.
		println("Started Initializing the Person's Disease Status")
		initializePersonDiseaseStatus()
		println("Completed Initializing the Disease Status")
		
		// Initialize the household behavior status.
		println("Started Initializing the Household Behavior Status")
		initializeHHBehaviorStatus()
		println("Completed Initializing the Household Behavior Status")
		
		// Read the activities and convert their times to hours.
		if (!activitiesInputFile.equalsIgnoreCase('None')) {
			println("Started Creating Activities")
			createActivitiesFromCSVFile(activitiesInputFile)
			println("Completed Creating Activities")
		}
		
		// Show the initial status
		println("Started Counting People and Places")
		countPersonsAndPlaces()
		println("Completed Counting People and Places")
		
		println("Ready to Run...")
		
		
	}
	
	/* This routine executes the model.
	 * 
	 * @author Michael J. North
	 * 
	 */
	def go(){
		
		// Ask the people to execute their activities.
		ask(persons()){	
			
			// Find the time in minutes since the start of the current day.
			//TODO: Decide how to differentiate weekdays and weekends in the file and model.
			int time = (ticks() % 24)
			
			// Find the next activity.
			currentActivity = null
			for (Activity tempActivity in activityList) {
				if (tempActivity.start_time <= time && time < tempActivity.stop_time) {
					currentActivity = tempActivity
				}
			}
			
			// Go to the place for the activity.
			if (currentActivity == null) {
				goToHH()
			} else if (currentActivity.place_type.equalsIgnoreCase("Household")) {
				goToHH()
			} else if (currentActivity.place_type.equalsIgnoreCase("Work")) {
				goToWork()
			} else if (currentActivity.place_type.equalsIgnoreCase("School")) {
				goToSchool()
			} else if (currentActivity.place_type.equalsIgnoreCase("Group Quarters")) {
				goToGQ()
			} else {
				goToHH()
			}
		}
		
		// Count again.
		countPersonsAndPlaces()
		
		// Expose people to disease transmission risk.
		ask(persons()){
			
			// Activate a transition as requested.
			if (transitionRule.equalsIgnoreCase('None')) {
			} else if (transitionRule.equalsIgnoreCase('Simple')) {
				activateSimpleTransition(currentActivity)
			} else if (transitionRule.equalsIgnoreCase('Detailed')) {
				activateDetailedTransition(currentActivity)
			}
		}
		
		// Move to the next hour.
		tick()
		
	}
	
	/* This routine updates the counters.
	 * 
	 * @author Michael J. North
	 * 
	 * 
	 */
	def countPersonsAndPlaces() {
		
		// Reset the counters.
		totalUncolonized = 0
		totalColonized = 0
		totalInfected = 0
		ask (places()) {
			uncolonized = 0
			colonized = 0
			infected = 0
		}
		ask(patches()) {
			uncolonized = 0
			colonized = 0
			infected = 0
		}
		
		// Update the counters.
		ask (persons()) {
			if (status == PersonStatus.UNCOLONIZED) {
				totalUncolonized++
				if (currentPlace != null) currentPlace.uncolonized++
				patchHere().uncolonized++
			} else if (status == PersonStatus.COLONIZED) {
				totalColonized++
				if (currentPlace != null) currentPlace.colonized++
				patchHere().colonized++
			} else if (status == PersonStatus.INFECTED) {
				totalInfected++
				if (currentPlace != null) currentPlace.infected++
				patchHere().infected++
			}
		}
		
		// Report the results.
		if (ticks() <= 0) println("    Hour, Uncolonized, Colonized, Infected, Total")
		println("    " + ((int) ticks()) + ", " + totalUncolonized + ", " +
				totalColonized + ", " + totalInfected + ", " +
				(totalUncolonized + totalColonized + totalInfected))
		
		// Update the map, if needed.
		if ((showPersonMovement.equalsIgnoreCase('Yes')) && (persons().size() > 0)) {
			int maxUncolonized = maxOneOf(patches(), { uncolonized }).uncolonized
			int maxColonized = maxOneOf(patches(), { colonized }).colonized
			int maxInfected = maxOneOf(patches(), { infected }).infected
			ask (patches()) {
				setPcolor(scaleColor(red(), 100*infected + 10*colonized + uncolonized,
					0.0, 100*maxInfected + 10*maxColonized + maxUncolonized))
			}
		}
		
	}
	
	
	/* This routine is a file reader that creates turtles from CSV files.
	 * 
	 * @author Michael J. North
	 * 
	 * @param fileName the path from the default system directory
	 * @param turtleType the class of turtle to create
	 * @param defaultSize the turtle size
	 * @param defaultColor the turtle color
	 * 
	 */
	def createTurtlesFromCSVFile(String fileName, Class turtleType,
		String defaultShape, double defaultSize, double defaultColor) {
		
		// Read the data file.
		List<String[]> rows = new CSVReader(
				new InputStreamReader(new FileInputStream(fileName)))
				.readAll()
		
		// Define the fields lists.
		List fullFieldList
		List matchedFieldList
		
		// Create the agents.
		for (row in rows) {
			
			// Check the fields list.
			if (fullFieldList == null) {
				
				// Fill in the field lists.
				fullFieldList = (List) row
				matchedFieldList = (List) turtleType.fields.collect({it.getName() }).
				intersect((List) row)
			} else {
				
				// Define an index tracker.
				int index
				
				// Create the next agent.
				createTurtles(1, {
					
					// Assign properties from the file.
					for (field in matchedFieldList) {
						index = fullFieldList.indexOf(field)
						if (it."$field" instanceof Integer) {
							if (isParsableToInteger(row[index])) {
								it."$field" = (row[index] as Integer)
							} else {
								it."$field" = 0
							}
						} else if (it."$field" instanceof Double) {
							if (isParsableToDouble(row[index])) {
								it."$field" = (row[index] as Double)
							} else {
								it."$field" = 0d
							}
						} else {
							it."$field" = row[index]
						}
					}
					
					// Set the shape.
					setShape(defaultShape)
					
					// Set the size.
					setSize(defaultSize)
					
					// Set the default heading.
					setHeading(0)
					
					// Set the default color.
					setColor(defaultColor)
					
				}, turtleType.getSimpleName())
			}
		}
	}
	
	
	/* This routine is a initializes the disease status.
	 *
	 * @author Michael J. North
	 *
	 *
	 */
	def initializePersonDiseaseStatus() {
		
		// Convert the colonization percentage to a fraction.
		double initialColonizationFraction = initialColonizationPercentage / 100.0
		
		// Initialize colonization.
		println("    persons.size() == " + persons().size())
		ask (persons()) {
			// Set the initial status.
			if (randomFloat(1.0) <= initialColonizationFraction) {
				status = PersonStatus.COLONIZED
			} else {
				status = PersonStatus.UNCOLONIZED
			}
		}
		println("    persons.size() == " + persons().size())
		
		// Initialize infection.
		ask (nOf(initialInfectedCount, persons())) {
			status = PersonStatus.INFECTED
		}
	}
	
	/* This routine is a initializes the HH status.
	 *
	 * @author Michael J. North
	 *
	 *
	 */
	def initializeHHBehaviorStatus() {
		
		// Convert the percentage to a fraction.
		double fasterResponseFraction = fasterResponsePercentage / 100.0
		
		// Initialize the behavior, if needed.
		if (!behaviorRule.equalsIgnoreCase('Uniform')) {
			
			// Note the north/south axis dividing line.
			int centerLine = (int) (getMinPycor() + worldHeight() / 2.0)
			
			// Scan the places.
			println("    places.size() == " + persons().size())
			ask (places()) {
				
				// Set the initial status, as required.	
				if (type.equalsIgnoreCase("Household")) {
					if (behaviorRule.equalsIgnoreCase('Clustered by HH')) {
						if (randomFloat(1.0) <= fasterResponseFraction) {
							fasterResponse = true
							println("HH faster response *********************")
						} else {
							println("HH regular response")
						}				
					} else if (behaviorRule.equalsIgnoreCase('Cluster by HH and Address')) {
						if (getPycor() > centerLine) {
							if (randomFloat(1.0) <= fasterResponseFraction) {
								fasterResponse = true
							}
						} else {
							if (randomFloat(1.0) <= (1.0 - fasterResponseFraction)) {
								fasterResponse = true
							}
						}
					}
				}
			}
		}
	}
	
	/* This routine is a file reader that creates activities from CSV files.
	 *
	 * @author Michael J. North
	 *
	 * @param fileName the path from the default system directory
	 *
	 */
	def createActivitiesFromCSVFile(String fileName) {
		
		// Read the data file.
		List<String[]> rows = new CSVReader(
				new InputStreamReader(new FileInputStream(fileName)))
				.readAll()
		
		// Prepare the temporary master list of activity lists.
		ArrayList<ActivityList> masterListOfActivityLists = new ActivityList()
		
		// Prepare the temporary activity list.
		ActivityList tempActivityList = new ActivityList()
		
		// Define the fields lists.
		List fullFieldList
		List matchedFieldList
		
		// Create the activities.
		println("    Started Allocating the Activities")
		for (row in rows) {
			
			// Check the fields list.
			if (fullFieldList == null) {
				
				// Fill in the field lists.
				fullFieldList = (List) row
				matchedFieldList = (List) Activity.class.fields.collect({it.getName() }).
				intersect((List) row)
			} else {
				
				// Define an index tracker.
				int index
				
				// Create the next agent.
				Activity newActivity = new Activity()
				
				// Assign properties from the file.
				for (field in matchedFieldList) {
					index = fullFieldList.indexOf(field)
					if (newActivity."$field" instanceof Integer) {
						if (isParsableToInteger(row[index])) {
							newActivity."$field" = (row[index] as Integer)
						} else {
							newActivity."$field" = 0
						}
					} else if (newActivity."$field" instanceof Double) {
						if (isParsableToDouble(row[index])) {
							newActivity."$field" = (row[index] as Double)
						} else {
							newActivity."$field" = 0d
						}
					} else {
						newActivity."$field" = row[index]
					}
				}
				
				// Convert the times from minutes to hours.
				newActivity.start_time = newActivity.start_time / 60
				newActivity.stop_time = newActivity.stop_time / 60
				
				// Progressively update the master list of lists.
				if ((tempActivityList.getTucaseid() == null) ||
				(newActivity.tucaseid.equals(tempActivityList.getTucaseid()))) {
					tempActivityList.add(newActivity)
				} else {
					masterListOfActivityLists.add(tempActivityList)
					tempActivityList = new ActivityList()
					tempActivityList.add(newActivity)
				}
			}
		}
		
		// Note the last activity list.
		masterListOfActivityLists.add(tempActivityList)
		println("    Completed Allocating the Activities")
		
		// Sort the master list of lists.
		println("    Started Sorting the Activities")
		Collections.sort(masterListOfActivityLists)
		println("    Completed Sorting the Activities")
		
		// Sort the list of people.
		println("    Started Sorting the People")
		List sortedPersons = Utility.sort(persons())
		println("    Completed Sorting the People")
		
		// Prepare to match people with activities.
		println("    Started Matching " + sortedPersons.size() +
				" People to Activities")
		int scanCounter = 0
		int matchCounter = 0
		Iterator personIterator = sortedPersons.iterator()
		if (personIterator.hasNext()) {
			
			// Match people with activities.
			Person tempPerson = personIterator.next()
			for (ActivityList nextActivityList in masterListOfActivityLists) {
				
				// Ignore people who do not match.
				while ((nextActivityList.getTucaseid().compareTo(tempPerson.tucaseid) > 0) &&
				(personIterator.hasNext())) {
					ask (tempPerson) { die() }
					tempPerson = personIterator.next()
				}
				
				// Match a person with an activity.
				while (nextActivityList.getTucaseid().equals(tempPerson.tucaseid)) {
					tempPerson.activityList = nextActivityList
					matchCounter++
					if (personIterator.hasNext()) {
						tempPerson = personIterator.next()
					} else {
						break
					}
				}
				scanCounter++
				
				// Report on progress.
				if (scanCounter % 100 == 0) {
					println(
							"        Scanned " + scanCounter +
							" Activities and Matched " +
							matchCounter + " People")
				}
			}
		}	
		println("    Completed Matching " + matchCounter +
				" of " + sortedPersons.size() + " People to " +
				scanCounter + " Activities")
	}
	
	/* This routine normalizes the place coordinates.
	 * 
	 * @author Michael J. North
	 * 
	 */
	def normalizePlaceCoordinates() {
		
		// Check for places.
		if (places().size() > 0) {
			
			// Find the bounds.
			def minX = places().min({it.longitude }).longitude
			def maxX = places().max({it.longitude }).longitude
			def minY = places().min({it.latitude }).latitude
			def maxY = places().max({it.latitude }).latitude
			
			// Calculate the normalization factors.
			def xRange = maxX - minX
			def yRange = maxY - minY
			def centerX = (xRange / 2.0) + minX
			def centerY = (yRange / 2.0) + minY
			def xScale = (getMaxPxcor() - getMinPxcor()) / xRange
			def yScale = (getMaxPycor() - getMinPycor()) / yRange
			def scale = Math.min(xScale, yScale)
			
			// Normalize the place coordinates.
			ask (places()) {
				setXcor(scale * (longitude - centerX))
				setYcor(scale * (latitude - centerY))
			}
		}
	}
	
	/* This routine matches people to places.
	 * 
	 * @author Michael J. North
	 * 
	 */
	def matchPeopleAndPlaces() {
		
		// Setup the place map.
		def placesMap = [:]
		ask (places()) {
			placesMap.putAt(place_id, it)
		}
		
		// Link the people to places.
		ask (persons()) {
			
			// Set the household location.
			hh = safePlaceLookup(placesMap, hh_id)
			
			// Set the group quarters location.
			gq = safePlaceLookup(placesMap, gq_id)
			
			// Set the work location.
			work = safePlaceLookup(placesMap, work_id)
			
			// Set the school location.
			school = safePlaceLookup(placesMap, school_id)
			
			// Eliminate unassigned people.
			if ((hh == null) && (gq == null) && (work == null) && (school == null)) {
				die()
			}
		}
	}
	
	/* This routine safely looks up places.
	 *
	 * @author Michael J. North
	 *
	 * @param placesMap the map of places
	 * @param place_id the place to link to
	 *
	 */
	Place safePlaceLookup(def placesMap, String place_id) {
		
		// Declare the return value.
		Place place = null;
		
		// Check the place identifier.
		if ((place_id != null) && (!place_id.trim().equals(''))) {
			
			// Find the designated place.
			place = placesMap.getAt(place_id)
			
		}
		
		// Return the results.
		return place
		
	}	

	/* This routine checks to see if a string contains an integer.
	 *
	 * @author Michael J. North
	 *
	 * @param s the string to check
	 * @return returns true if the string passes otherwise false
	 */
	public boolean isParsableToInteger(String s) {
		
		try {
			Integer.parseInt(s);
			return true;
		} catch (NumberFormatException e) {
			return false;
		}
	}
	
	/* This routine checks to see if a string contains a double.
	 *
	 * @author Michael J. North
	 *
	 * @param s the string to check
	 * @return returns true if the string passes otherwise false
	 */
	public boolean isParsableToDouble(String s) {
		
		try {
			Double.parseDouble(s);
			return true;
		} catch (NumberFormatException e) {
			return false;
		}
	}
}