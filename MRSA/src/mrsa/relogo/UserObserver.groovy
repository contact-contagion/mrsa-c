package mrsa.relogo

import static repast.simphony.relogo.Utility.*;
import static repast.simphony.relogo.UtilityG.*;
import repast.simphony.relogo.BaseObserver;
import repast.simphony.relogo.Stop;
import repast.simphony.relogo.Utility;
import repast.simphony.relogo.UtilityG;
import au.com.bytecode.opencsv.CSVReader;

class UserObserver extends BaseObserver {
	
	//TODO Initialize 5 people with MRSA infection (this number should remain stable throughout the regular runs)
	//TODO Initialize 3% of the people with MRSA colonization (" ")
	//TODO For differential run 1 cut C and D in half for 50% of the people such that people in the same HH have the same C and D
	//TODO For differential run 2 cut C and D in half for 50% of the people such that people in the same HH have the same C and D and the North half of 60615 averages 75% change and the South half averages 25% change

	/* This routine configures the model.
	 * 
	 * @author Michael J. North
	 * 
	 */
	def setup(){
		
		// Clear any current configuration.
		clearAll()
		
		// Read the people.
		if (!personsInputFile.equalsIgnoreCase('None')) {
			println("Started Reading Persons")
			createTurtlesFromCSVFile(personsInputFile, Person.class,
					'person', 0.5, Utility.blue())
			println("Completed Reading Persons")
		}
		
		// Read the places.
		if (!placesInputFile.equalsIgnoreCase('None')) {
			println("Started Reading Places")
			createTurtlesFromCSVFile(placesInputFile, Place.class,
					'square', 0.2, Utility.gray())
			println("Completed Reading Places")
		}
		
		// Set the default person drawing styles.
		setDefaultPersonStyles()
		
		// Set the default place drawing styles.
		setDefaultPlaceStyles()
		
		// Link the people to their places.
		println("Starting Linking Persons and Places")
		linkPeopleAndPlaces()
		println("Completed Linking Persons and Places")
		
		// Check for places.
		if (places().size() > 0) {
			
			// Normalize the place coordinates.
			println("Started Normalizing Place Locations")
			normalizePlaceCoordinates()
			println("Completed Normalizing Place Locations")
			
			// Read the activities and convert their times to hours.
			if (!activitiesInputFile.equalsIgnoreCase('None')) {
				println("Started Creating Activities")
				createActivitiesFromCSVFile(activitiesInputFile)
				println("Completed Creating Activities")
			}
			
			// Start the people at their household.
			println("Started Asking Persons to Go Home")
			ask (persons()) {
				
				// Go home.
				goToHHorGQ()
				
			}
			println("Completed Asking Persons to Go Home")
			println("Ready to Run...")
		}
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
			int time = (ticks() % 24)
			
			// Find the next activity.
			Activity nextActivity = null
			for (Activity tempActivity in activityList) {
				if (tempActivity.start_time <= time && time < tempActivity.stop_time) {
					nextActivity = tempActivity
				}
			}
			
			// Go to the place for the activity.
			if (nextActivity == null) {
				goToHH()
			} else if (nextActivity.place_type.equalsIgnoreCase("Household")) {
				goToHH()
			} else if (nextActivity.place_type.equalsIgnoreCase("Work")) {
				goToWork()
			} else if (nextActivity.place_type.equalsIgnoreCase("School")) {
				goToSchool()
			} else if (nextActivity.place_type.equalsIgnoreCase("Group Quarters")) {
				goToGQ()
			} else {
				goToHH()
			}
			
			// Activate a transition.
			if (transitionRule.equalsIgnoreCase('None')) {
			} else if (transitionRule.equalsIgnoreCase('Simple')) {
				activateSimpleTransition(nextActivity)
			} else if (transitionRule.equalsIgnoreCase('Detailed')) {
				activateDetailedTransition(nextActivity)
			}
		}	
		
		// Move to next hour.
		tick()
		
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
				matchedFieldList = (List) turtleType.fields.collect({it.getName()}).
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
				matchedFieldList = (List) Activity.class.fields.collect({it.getName()}).
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
					ask (tempPerson) {
						die()
					}
					tempPerson = personIterator.next()
				}
							
				// Match a person with an activity.
				while (nextActivityList.getTucaseid().equals(tempPerson.tucaseid)) {
					tempPerson.activityList = nextActivityList
					if (personIterator.hasNext()) {
						tempPerson = personIterator.next()
					} else {
						break
					}
					matchCounter++
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
		
		// Find the bounds.
		def minX = places().min({it.longitude}).longitude
		def maxX = places().max({it.longitude}).longitude
		def minY = places().min({it.latitude}).latitude
		def maxY = places().max({it.latitude}).latitude
		
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
	
	/* This routine links people to places.
	 * 
	 * @author Michael J. North
	 * 
	 */
	def linkPeopleAndPlaces() {
		
		// Setup the place map.
		def placesMap = [:]
		ask (places()) {
			placesMap.putAt(place_id, it)
		}
		
		// Link the people to places.
		ask (persons()) {
			
			// Set the household location.
			it.hh = safePlaceLookup(placesMap, it.hh_id)
			
			// Set the group quarters location.
			it.gq = safePlaceLookup(placesMap, it.gq_id)
			
			// Set the work location.
			it.work = safePlaceLookup(placesMap, it.work_id)
			
			// Set the school location.
			it.school = safePlaceLookup(placesMap, it.school_id)
			
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
		
	/* This routine assigns default place drawing styles.
	 *
	 * @author Michael J. North
	 *
	 */
	def setDefaultPersonStyles() {
		
		// Set the place style based on the type.
		ask(persons()) {
			
			// Check the type and assign a style.
			if (status == PersonStatus.UNCOLONIZED) {
				decolonize()
			} else if (status == PersonStatus.COLONIZED) {
				colonize()
			} else if (status == PersonStatus.INFECTED) {
				infect()
			}
		}
	}
	
	/* This routine assigns default place drawing styles.
	 *
	 * @author Michael J. North
	 *
	 */
	def setDefaultPlaceStyles() {
		
		// Set the place style based on the type.
		ask(places()) {
			
			// Check the type and assign a style.
			if (type.equalsIgnoreCase("College")) {
				setColor(Utility.yellow())
				setSize(0.3)
				setShape('circle')
			} else if (type.equalsIgnoreCase("School")) {
				setColor(Utility.orange())
				setSize(0.3)
				setShape('circle')
			} else if (type.equalsIgnoreCase("Household")) {
			} else if (type.equalsIgnoreCase("Work")) {
				setColor(Utility.blue())
				setSize(0.3)
				setShape('triangle')
			} else if (type.equalsIgnoreCase("Group Quarters")) {
				setColor(Utility.white())
				setSize(0.3)
				setShape('circle')
			}
		}
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