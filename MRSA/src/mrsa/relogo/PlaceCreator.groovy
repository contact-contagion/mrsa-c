/**
 * 
 */
package mrsa.relogo

import au.com.bytecode.opencsv.CSVReader

/**
 * Create places from a place csv file.
 *
 */
class PlaceCreator {

	private static final int ID_IDX = 0;
	private static final int PLACE_IDX = 1;
	private static final int LAT_IDX = 4;
	private static final int LON_IDX = 5;

	private UserObserver obs;

	public PlaceCreator(UserObserver obs) {
		this.obs = obs;
	}

	// return the appropriate class to create
	// for the specified place type.
	private Class findClass(String placeType) {
		if (placeType.equals("Household")) {
			return Household.class;
		}

		if (placeType.equals("School")) {
			return School.class;
		}

		return SelectiveTransmissionPlace.class;
	}

	public void run(String fileName, String defaultShape, double defaultSize, double defaultColor) {
		List fields = []

		// get the fields in the Place class
		Place.class.getDeclaredFields().each{
			fields << it.getName()
		}

		// Read the given data file.
		CSVReader reader = new CSVReader(new InputStreamReader(new FileInputStream(fileName)));
		// read the header row
		def headers = reader.readNext();
		
		// read the rows and create a place for each one.
		String[] vals = null;
		while ((vals = reader.readNext()) != null) {
			String placeType = vals[PLACE_IDX].trim();
			Class turtleClass = findClass();
			obs.createTurtles(1,{t ->
				t.place_id = vals[ID_IDX].trim();
				t.place_type = placeType;
				String lat = vals[LAT_IDX].trim();
				if (lat.length() > 0) t.latitude = Double.parseDouble(lat);
				String lon = vals[LON_IDX].trim();
				if (lon.length() > 0) t.longitude = Double.parseDouble(lon);
				
				if (placeType.equals("Household")) t.risk = 2;
				else if (placeType.equals("School")) t.risk = 1;
				else if (placeType.equals("Workplace")) {
					t.risk = 0.1;
					t.personsToTry = 4;
				} 
				else {
					t.risk = 1;
					t.personsToTry = 4;
				}
				
				t.setShape(defaultShape)
				
				// Set the size.
				t.setSize(defaultSize)
				
				// Set the default heading.
				t.setHeading(0)
				
				// Set the default color.
				t.setColor(defaultColor)
				

			}, turtleClass.getSimpleName());
		}
	}
}

