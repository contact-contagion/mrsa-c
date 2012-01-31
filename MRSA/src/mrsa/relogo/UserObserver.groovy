package mrsa.relogo

import static repast.simphony.relogo.Utility.*
import static repast.simphony.relogo.UtilityG.*
import mrsa.relogo.comparators.PersonWeekdayComparator
import mrsa.relogo.comparators.PersonWeekendComparator
import repast.simphony.relogo.BaseObserver
import repast.simphony.relogo.Utility
import au.com.bytecode.opencsv.CSVReader

// The user observer class.
class UserObserver extends BaseObserver {
	
	// The output tracking file.
	public static PrintWriter outputFile = new PrintWriter(new FileWriter("outputfile.txt"));
	public static PrintStream consoleFile = new PrintStream(new FileOutputStream("consolefile.txt"));
	
	/* This routine configures the model.
	 * 
	 * @author Michael J. North
	 * 
	 */
	def setup(){
		
		// Clear any current configuration.
		clearAll()
		
		// Prepare the console output.
		System.out = consoleFile
		System.err = consoleFile
		
		// Check for a persons file request.
		if (!personsInputFile.equalsIgnoreCase('None')) {
			
			// Note the state.
			println("Started Reading Persons")
			
			// Read the people and initialize their disease status.
			Timer.start()
			createTurtlesFromCSVFile(personsInputFile, Person.class,
					'square', 0, Utility.black())
			Timer.stop()
			Timer.show()
			
			// Note the state.
			println("Completed Reading Persons")

		}
		
		// Check for a places file request.
		if (!placesInputFile.equalsIgnoreCase('None')) {

			// Note the state.
			println("Started Reading Places")

			// Read the places.
			Timer.start()
			createTurtlesFromCSVFile(placesInputFile, Place.class,
					'square', 0.1, Utility.white())
			Timer.stop()
			Timer.show()

			// Note the state.
			println("Completed Reading Places")

		}
		
		// Note the state.
		println("Starting Matching Persons and Places")

		// Match the people with their places.
		Timer.start()
		matchPeopleAndPlaces()
		Timer.stop()
		Timer.show()

		// Note the state.
		println("Completed Matching Persons and Places")

		// Note the state.
		//println("Started Normalizing Place Locations")

		// Normalize the place coordinates.
		//Timer.start()
		//normalizePlaceCoordinates()
		//Timer.stop()
		//Timer.show()

		// Note the state.
		//println("Completed Normalizing Place Locations")

		// Note the state.
		println("Started Asking Persons to Begin at Home")

		// Start the people at their household.
		Timer.start()
		ask (persons()) {
			
			// Start at home.
			goToHome()
			
		}
		Timer.stop()
		Timer.show()

		// Note the state.
		println("Completed Asking Persons to Begin at Home")

		// Note the state.
		println("Started Initializing the Person's Disease Status")

		// Initialize the disease status.
		Timer.start()
		initializePersonDiseaseStatus()
		Timer.stop()
		Timer.show()

		// Note the state.
		println("Completed Initializing the Disease Status")

		// Note the state.
		println("Started Initializing the Household Behavior Status")

		// Initialize the household behavior status.
		Timer.start()
		initializeHHBehaviorStatus()
		Timer.stop()
		Timer.show()

		// Note the state.
		println("Completed Initializing the Household Behavior Status")
		
		// Check for an activities file request.
		if (!activitiesInputFile.equalsIgnoreCase('None')) {

			// Note the state.
			println("Started Creating Activities")
			
			// Read the activities and convert their times to hours.
			Timer.start()
			createActivitiesFromCSVFile(activitiesInputFile)
			Timer.stop()
			Timer.show()
			
			// Note the state.
			println("Completed Creating Activities")
			
		}
		
		// Note the state.
		println("Started Counting People and Places")

		// Show the initial status.
		Timer.start()
		countPersonsAndPlaces()
		Timer.stop()
		Timer.show()

		// Note the state.
		println("Completed Counting People and Places")
		
		// Note the state.
		println("Ready to Run...")
		
		
	}
	
	/* This routine executes the model.
	 * 
	 * @author Michael J. North
	 * 
	 */
	def go(){
		
		// Ask the people to execute their activities.
		println("Ask people to execute their activities")
		Timer.start()
		ask (persons()){	
			
			// Find the time in hours since the start of the current day.
			int time = (ticks() % 24)
			
			// Find the day count.
			int dayCount =  (((int) ticks()) / 24)
			
			// Determine if it is a weekend or weekday. Sundays are day 0,
			// Mondays are day 1, Tuesdays are day 2, etc.
			int day = dayCount % 7
			boolean weekday = true
			if ((day == 0) || (day == 6)) weekday = false
			
			// Select the appropriate activity list.
			ActivityList activityList = null
			if (weekday) {
				activityList = activityList_weekday
			} else {
				activityList = activityList_weekend
			}
			
				
			// Prepare to find the next activity.
			currentActivity = null
			
			// Scan activity list.
			for (Activity tempActivity in activityList) {
				
				// Check the activity scheduled now.
				if (tempActivity.start_time <= time && time < tempActivity.stop_time) {
					
					// Note the activity.
					currentActivity = tempActivity
					
				}
			}
			
			// Go to the place for the activity.
			if (currentActivity == null) {
				
				// Go home by default.
				goToHH()
			
			// Check for home.
			} else if (currentActivity.place_type.equalsIgnoreCase("Household")) {
			
				// Go home.
				goToHH()
			
			// Check for a work place.
			} else if (currentActivity.place_type.equalsIgnoreCase("Work")) {
			
				// Go to work.
				goToWork()
				
			// Check for a school.
			} else if (currentActivity.place_type.equalsIgnoreCase("School")) {
			
				// Go to school.
				goToSchool()
				
			// Check for group quarters.
			} else if (currentActivity.place_type.equalsIgnoreCase("Group Quarters")) {
			
				// Go to the group quarters.
				goToGQ()
				
			// Do the default.
			} else {
			
				// Go home.
				goToHH()
				
			}
			
		}
		Timer.stop()
		Timer.show()
		println("Done with activities")
		
		// Count again.
		println("Count persons and places")
		Timer.start()
		countPersonsAndPlaces()
		Timer.stop()
		Timer.show()
		println("Done counting persons and places")

		// Expose people to disease transmission risk.
		println("Expose people to disease transmission risk")
		Timer.start()
		ask(persons()){
			
			// Activate a transition as requested.
			if (transitionRule.equalsIgnoreCase('None')) {
			
				// Do nothing.
				
			// Check for a simple rule request.
			} else if (transitionRule.equalsIgnoreCase('Simple')) {

				// Perform a simple transition.
				activateSimpleTransition(currentActivity)

			// Check for a detailed activity rule request.
			} else if (transitionRule.equalsIgnoreCase('Detailed Activity Rule')) {
			
				// Perform a detailed activity-based transition.
				activateDetailedActivityTransition(currentActivity)
				
			// Check for a detailed place rule request.
			} else if (transitionRule.equalsIgnoreCase('Detailed Place Rule')) {
			
				// Perform a detailed place-based transition.
				activateDetailedPlaceTransition()
				
			}
			
		}
		Timer.stop()
		Timer.show()
		println("Done exposing people to disease transmission risk")
		
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
		
		// Reset the total uncolonized counter.
		totalUncolonized = 0

		// Reset the total colonized counter.
		totalColonized = 0

		// Reset the total infected counter.
		totalInfected = 0
		
		// Note the graphics request.
		boolean graphics = (showPersonMovement.equalsIgnoreCase('Yes'))
		
		// Setup a new visual display for each place.
		ask (places()) {
			
			// Check the graphics request.
			if (graphics) {
			
				// Check the current place color.
				if (getColor() != Utility.white()) {
					
					// Set a neutral color.
					setColor(Utility.white())
					
				}
				
				// Check the place drawing size.
				if (getSize() != 0.1) {
					
					// Set the default size.
					setSize(0.1)
				}
				
			}
			
			// Reset the local place uncolonized counter.
			uncolonized = 0
			
			// Reset the local place colonized counter.
			colonized = 0
			
			// Reset the local place infected counter.
			infected = 0

		}
		
		// Setup a new visual display for each patch, if requested.
		if ((graphics) && (persons().size() > 0)) {
			
			// Setup a new visual display for each place.
			ask (patches()) {
				
				// Reset the local place uncolonized counter.
				uncolonized = 0
				
				// Reset the local place colonized counter.
				colonized = 0
				
				// Reset the local place infected counter.
				infected = 0

			}
			
		}
		
		// Update the counters.
		ask (persons()) {
			
			// Account for an uncolonized state.
			if (status == PersonStatus.UNCOLONIZED) {
				
				// Increment the observer global uncolonized counter.
				totalUncolonized++
				
				// Increment place uncolonized counter, if appropriate.
				if (currentPlace != null) currentPlace.uncolonized++
				
				// Increment patch uncolonized counter.
				patchHere().uncolonized++
				
			// Account for a colonized state.
			} else if (status == PersonStatus.COLONIZED) {
			
				// Increment the observer global colonized counter.
				totalColonized++
				
				// Increment place colonized counter, if appropriate.
				if (currentPlace != null) currentPlace.colonized++
				
				// Increment patch colonized counter.
				patchHere().colonized++
				
			// Account for an infected state.
			} else if (status == PersonStatus.INFECTED) {
			
				// Increment the observer global infected counter.
				totalInfected++
				
				// Increment place infected counter, if appropriate.
				if (currentPlace != null) currentPlace.infected++
				
				// Increment patch infected counter.
				patchHere().infected++
				
			}

		}
		
		// Send out a header for reporting the results.
		if (ticks() <= 0) println("    Hour, Uncolonized, Colonized, Infected, Total")
		
		// Report the results.
		outputFile.println("    " + ((int) ticks()) + ", " + totalUncolonized + ", " +
				totalColonized + ", " + totalInfected + ", " +
				(totalUncolonized + totalColonized + totalInfected + ", " +
				System.nanoTime()))
		
		// Update the map, if needed.
		if ((graphics) && (persons().size() > 0)) {
			
			// Find the maximum colonization count to prepare for normalization.
			int maxColonized = maxOneOf(patches(), { colonized }).colonized
			
			// Change the display for each patch.
			ask (patches()) {
				
				// Check for infected people.
				if (infected > 0) {
					
					// Select the color to indicate that an infected person is present.
					setPcolor(Utility.yellow())
					
				} else {
				
					// Select the color to indicate that a colonized person is present.
					setPcolor(scaleColor(yellow(), colonized, 0, maxColonized))
					
				}
				
			}
			
			// Change the display for each place.
			ask (places()) {
				
				// Check for infected people.
				if (infected > 0) {
					
					// Select the color to indicate that an infected person is present.
					setColor(Utility.red())
					
					// Select the size to indicate that an infected person is present.
					setSize(0.5)
					
				// Check for c people.
				} else if (colonized > 0) {
				
					// Select the color to indicate that a colonized person is present.
					setColor(Utility.orange())
					
				}

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
				
				// Explore the undiscovered country.
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
		
		// Prepare for non-integers.
		try {
			
			// Try to convert the input to an integer.
			Integer.parseInt(s);
			
			// Note success.
			return true;
			
		// Catch errors.
		} catch (NumberFormatException e) {
		
			// Note failure.
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
		
		// Prepare for non-double precision number.
		try {
			
			// Try to convert the input to a double precision number.
			Double.parseDouble(s);
			
			// Note success.
			return true;
			
		// Catch errors.
		} catch (NumberFormatException e) {
		
			// Note failure.
			return false;
			
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
		
		// Read the given data file.
		List<String[]> rows = new CSVReader(
				new InputStreamReader(new FileInputStream(fileName)))
				.readAll()
		
		// Define the full fields list.
		List fullFieldList
		
		// Define the matched fields list.
		List matchedFieldList
		
		// Create the agents.
		for (row in rows) {
			
			// Check the fields list.
			if (fullFieldList == null) {
				
				// Fill in the field lists.
				fullFieldList = (List) row
				
				// Parse the fields.
				matchedFieldList = (List) turtleType.fields.collect({it.getName() }).intersect((List) row)
				
			} else {
				
				// Define an index tracker.
				int index
				
				// Create the next agent.
				createTurtles(1, {
					
					// Assign properties from the file.
					for (field in matchedFieldList) {
						
						// Check the column index of the next field.
						index = fullFieldList.indexOf(field)
						
						// Check for integers.
						if (it."$field" instanceof Integer) {
							
							// Confirm that this is an integer.
							if (isParsableToInteger(row[index])) {
								
								// Convert the field as an integer.
								it."$field" = (row[index] as Integer)
								
							} else {
							
								// Set the garbled integer to zero.
								it."$field" = 0
								
								// Note the state.
								//println("Error: Garbled integer set to zero in turtle file reader.")

							}
							
						// Check for double precision numbers.
						} else if (it."$field" instanceof Double) {
													
							// Confirm that this is a double precision number.
							if (isParsableToDouble(row[index])) {
								
								// Convert the field as a double precision number.
								it."$field" = (row[index] as Double)
								
							} else {
							
								// Set the garbled double precision number to zero.
								it."$field" = 0d
								
								// Note the state.
								//println("Error: Garbled double precision number set to zero in turtle file reader.")
								
							}
							
						} else {
						
							// Store the input as a literal string.
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
				
				// Give the agent a good name.
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
		
		// Note the state.
		println("    persons().size() == " + persons().size())

		// Initialize colonization.
		ask (persons()) {
			
			// Set the initial status.
			if (randomFloat(1.0) <= initialColonizationFraction) {
				
				// Note a colonized person.
				status = PersonStatus.COLONIZED
			} else {
			
				// Note an uncolonized person.
				status = PersonStatus.UNCOLONIZED
				
			}
			
		}

		// Note the state.
		println("    persons().size() == " + persons().size())
		
		// Initialize infection.
		ask (nOf(initialInfectedCount, persons())) {
			
			// Note an infected person.
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
			ask (places()) {
				
				// Set the initial status, as required.	
				if (place_type.equalsIgnoreCase("Household")) {
					
					// Check to see if clustering (faster response rates) by household is requested. 
					if (behaviorRule.equalsIgnoreCase('Clustered by HH')) {
						
						// Check for a faster response rate.
						if (randomFloat(1.0) <= fasterResponseFraction) {
							
							// Initialize a faster response rate.
							fasterResponse = true
							
						}
						
					// Check to see if clustering (faster response rates) by household and address is requested. 
					} else if (behaviorRule.equalsIgnoreCase('Cluster by HH and Address')) {
					
						// Divide the simulation area in half.
						if (getPycor() > centerLine) {
							
							// Use one fraction for the first half of the geographic area.
							if (randomFloat(1.0) <= fasterResponseFraction) {
								
								// Initialize a faster response rate.
								fasterResponse = true
								
							}
							
						} else {
						
							// Use another fraction for the other half of the geographic area.
							if (randomFloat(1.0) <= (1.0 - fasterResponseFraction)) {
								
								// Initialize a faster response rate.
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
		
		// Define the fields list.
		List fullFieldList

		// Define the matched fields list.
		List matchedFieldList
		
		// Note the state.
		println("    Started Allocating the Activities")

		// Create the activities.
		for (row in rows) {
			
			// Check the fields list.
			if (fullFieldList == null) {
				
				// Fill in the field lists.
				fullFieldList = (List) row
				
				// Parse the fields.
				matchedFieldList = (List) Activity.class.fields.collect({it.getName() }).intersect((List) row)
				
			} else {
				
				// Define an index tracker.
				int index
				
				// Create the next agent.
				Activity newActivity = new Activity()
				
				// Assign properties from the file.
				for (field in matchedFieldList) {
					
					// Check the column index of the next field.
					index = fullFieldList.indexOf(field)
					
					// Check for integers.
					if (newActivity."$field" instanceof Integer) {
						
						// Confirm that this is an integer.
						if (isParsableToInteger(row[index])) {
							
							// Convert the field as an integer.
							newActivity."$field" = (row[index] as Integer)
							
						} else {
						
							// Set the garbled integer to zero.
							newActivity."$field" = 0
							
							// Note the state.
							//println("Error: Garbled integer set to zero in activity file reader.")

						}
						
					// Check for double precision numbers.
					} else if (newActivity."$field" instanceof Double) {
					
						// Confirm that this is a double precision number.
						if (isParsableToDouble(row[index])) {
							
							// Convert the field as a double precision number.
							newActivity."$field" = (row[index] as Double)
							
						} else {
						
							// Set the garbled double precision number to zero.
							newActivity."$field" = 0d
							
							// Note the state.
							//println("Error: Garbled double precision number set to zero in activity file reader.")
							
						}
						
					} else {
					
						// Store the input as a literal string.
						newActivity."$field" = row[index]
						
					}
					
				}
				
				// Convert the start time from minutes to hours.
				newActivity.start_time = newActivity.start_time / 60

				// Convert the end time from minutes to hours.
				newActivity.stop_time = newActivity.stop_time / 60
				
				// Progressively update the master list of lists.
				if ((tempActivityList.getTucaseid() == null) ||
					(newActivity.tucaseid.equals(tempActivityList.getTucaseid()))) {
					
					// Store the new activity in the current sublist.
					tempActivityList.add(newActivity)
					
				} else {
				
					// Add the current sublist to the master list.
					masterListOfActivityLists.add(tempActivityList)
					
					// Clear the sublist.
					tempActivityList = new ActivityList()
					
					// Store the new activity in the current sublist.
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
		println("    Started Sorting the People for Weekdays")
		List sortedPersons = persons().clone()
		Collections.sort(sortedPersons, new PersonWeekdayComparator())
		println("    Completed Sorting the People for Weekdays")
		
		// Prepare to match people with activities.
		println("    Started Matching " + sortedPersons.size() +
				" People to Weekday Activities")

		// Define the scan counter for tracking the number of activities processed.
		int scanCounter = 0
		
		// Define the match counter for tracking the number of persons processed.
		int matchCounter = 0

		// Define the counter for tracking the number of persons killed.
		int deathCounter = 0

		// Prepare the scan the person's list.
		Iterator personIterator = sortedPersons.iterator()
		
		// Make sure that there is at least one person.
		if (personIterator.hasNext()) {
			
			// Match people with activities.
			Person tempPerson = personIterator.next()
			
			// Declare the tracker for the last person.
			boolean lastPersonAssigned = false
			
			// Scan the activities list.
			for (ActivityList nextActivityList in masterListOfActivityLists) {
				
				// Ignore people who do not match.
				while ((nextActivityList.getTucaseid().compareTo(tempPerson.tucaseid_weekday) > 0)
					&& (personIterator.hasNext())) {
				
					// (Nicely) ask people who do not match activities to die.
					ask (tempPerson) {
						
						// Bye!
						die()
						
						// Note the tragedy.
						deathCounter++

					}
					
					// Move on to the next person.
					tempPerson = personIterator.next()
					
				}
				
				// Match a person with an activity.
				while (nextActivityList.getTucaseid().equals(tempPerson.tucaseid_weekday)) {
					
					// Assign the next activity list to a person.
					tempPerson.activityList_weekday = nextActivityList
					
					// Increment the count of people who matched a set of activities.
					matchCounter++
					
					// Check for more people.
					if (personIterator.hasNext()) {
						
						// Move on to the next person.
						tempPerson = personIterator.next()
						
					} else {
					
						// The last person has been assigned activities.
						lastPersonAssigned = true
						break
						
					}
					
				}
				
				// Increment the count of activities that matched a set of activities.
				scanCounter++
				
				// Report on the progress.
				if (scanCounter % 100 == 0) {
					println(
							"        Scanned " + scanCounter +
							" Weekday Activities and Matched " +
							matchCounter + " People")					
				}
				
				// Check to see if we are done.
				if (lastPersonAssigned) break
				
			}
			
		}	
		
		// Note the results.
		println("    Completed Matching " + matchCounter +
				" of " + (sortedPersons.size() + deathCounter) + " People to " +
				scanCounter + " Activities (" + deathCounter +
				" People Died Due to a Lack of Activities)")
		
		// Sort the list of people.
		println("    Started Sorting the People for Weekends")
		Collections.sort(sortedPersons, new PersonWeekendComparator())
		println("    Completed Sorting the People for Weekends")
		
		// Prepare to match people with activities.
		println("    Started Matching " + sortedPersons.size() +
				" People to Weekend Activities")

		// Clear the scan counter for tracking the number of activities processed.
		scanCounter = 0
		
		// Clear the match counter for tracking the number of persons processed.
		matchCounter = 0

		// Clear the counter for tracking the number of persons killed.
		deathCounter = 0

		// Prepare the scan the person's list.
		personIterator = sortedPersons.iterator()
		
		// Make sure that there is at least one person.
		if (personIterator.hasNext()) {
			
			// Match people with activities.
			Person tempPerson = personIterator.next()
			
			// Clear the tracker for the last person.
			boolean lastPersonAssigned = false

			// Scan the activities list.
			for (ActivityList nextActivityList in masterListOfActivityLists) {
				
				// Ignore people who do not match.
				while ((nextActivityList.getTucaseid().compareTo(tempPerson.tucaseid_weekend) > 0)
					&& (personIterator.hasNext())) {
				
					// (Nicely) ask people who do not match activities to die.
					ask (tempPerson) {
						
						// Bye!
						die()

						// Note the tragedy.
						deathCounter++

					}
					
					// Move on to the next person.
					tempPerson = personIterator.next()
					
				}
				
				// Match a person with an activity.
				while (nextActivityList.getTucaseid().equals(tempPerson.tucaseid_weekend)) {
					
					// Assign the next activity list to a person.
					tempPerson.activityList_weekend = nextActivityList
					
					// Increment the count of people who matched a set of activities.
					matchCounter++
					
					// Check for more people.
					if (personIterator.hasNext()) {
						
						// Move on to the next person.
						tempPerson = personIterator.next()
						
					} else {
					
						// The last person has been assigned activities.
						lastPersonAssigned = true
						break
						
					}
					
				}
				
				// Increment the count of activities that matched a set of activities.
				scanCounter++
				
				// Report on the progress.
				if (scanCounter % 100 == 0) {
					println(
							"        Scanned " + scanCounter +
							" Weekend Activities and Matched " +
							matchCounter + " People")
					
				}
	
				// Check to see if we are done.
				if (lastPersonAssigned) break

			}
			
		}	
		
		// Note the results.
		println("    Completed Matching " + matchCounter +
				" of " + (sortedPersons.size() + deathCounter) + " People to " +
				scanCounter + " Activities (" + deathCounter +
				" People Died Due to a Lack of Activities)")
		
	}
	
	/* This routine normalizes the place coordinates.
	 * 
	 * @author Michael J. North
	 * 
	 */
	def normalizePlaceCoordinates() {

		// Note the state.
		println("    places.size() == " + places().size())
		
		// Check for places.
		if (places().size() > 0) {
			
			// Find the minimum X bound.
			def minX = places().min({it.longitude }).longitude
			
			// Find the maximum X bound.
			def maxX = places().max({it.longitude }).longitude
			
			// Find the minimum Y bound.
			def minY = places().min({it.latitude }).latitude
			
			// Find the maximum Y bound.
			def maxY = places().max({it.latitude }).latitude
			
			// Calculate the X range normalization factor.
			def xRange = maxX - minX
			
			// Calculate the Y range normalization factor.
			def yRange = maxY - minY
			
			// Calculate the center X value.
			def centerX = (xRange / 2.0) + minX
			
			// Calculate the center Y value.
			def centerY = (yRange / 2.0) + minY
			
			// Calculate the x normalization factor.
			def xScale = (getMaxPxcor() - getMinPxcor()) / xRange
			
			// Calculate the x normalization factor.
			def yScale = (getMaxPycor() - getMinPycor()) / yRange
			
			// Calculate the normalization factors.
			def scale = Math.min(xScale, yScale)
			
			// Normalize the place coordinates.
			ask (places()) {
				
				// Normalize the next place X coordinate.
				setXcor(scale * (longitude - centerX))
				
				// Normalize the next place Y coordinate.
				setYcor(scale * (latitude - centerY))
				
			}
			
		}
		
	}
		
}	
