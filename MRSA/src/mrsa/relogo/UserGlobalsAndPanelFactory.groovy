package mrsa.relogo

import repast.simphony.relogo.factories.AbstractReLogoGlobalsAndPanelFactory

public class UserGlobalsAndPanelFactory extends AbstractReLogoGlobalsAndPanelFactory {
	
	public void addGlobalsAndPanelComponents() {
	
		// Show a tick count.
		addReLogoTickCountDisplay()
		
		/**
		 * Place custom panels and globals below, for example:
		 * 
		  		addButtonWL("setup","Setup") 	// Button with label ( method name, button label )
		  		addButton("action1")			// Button without label ( method name )
		  		addToggleButtonWL("go","Go")	// Toggle Button with label ( method name, button label )
		  		addToggleButton("action2")		// Toggle Button without label  ( method name )
		  		addGlobal("globalVariable1")	// Globally accessible variable ( variable name)
		  		// Slider with label ( variable name, slider label, minimum value, increment, maximum value, initial value )
		        addSliderWL("sliderVariable", "Slider Variable", 0, 1, 10, 5)
		        // Slider without label ( variable name, minimum value, increment, maximum value, initial value )
		        addSlider("sliderVariable2", 0.2, 0.1, 0.8, 0.5)
		        // Chooser with label  ( variable name, chooser label, list of choices , zero-based index of initial value )
		        addChooserWL("chooserVariable", "Chooser Variable", ["yes","no","maybe"], 2)
		        // Chooser without label  ( variable name, list of choices , zero-based index of initial value )
		        addChooser("chooserVariable2", [1, 66, "seven"], 0)
		 */

		// Add a setup button.
		addButtonWL('setup', 'Setup')

		// Add a step button.
		addButtonWL('go', 'Step')
		
		// Add a go toggle button.
		addToggleButtonWL('go', 'Go')
		
		//TODO: Change Kenwood to 60615.

		// Add a chooser for person input files.
		addChooserWL('personsInputFile', 'Persons Input File:',
			['None',
			'data/kenwood_persons_sample.csv',
			'data/kenwood_persons.csv'], 1)
		
		// Add a chooser for place input files.
		addChooserWL('placesInputFile', 'Places Input File:',
			['None',
			'data/kenwood_places_sample.csv',
			'data/kenwood_places.csv'], 1)

		// Add a chooser for place input files.
		addChooserWL('activitiesInputFile', 'Activities Input File:',
			['None',
			'data/kenwood_activities_sample.csv',
			'data/kenwood_activities_sample_small.csv'], 2)

		// Add a global maximum risk variable.
		addGlobal("maximumRisk", 4)
		
		// Add a global infection fraction for testing purposes.
		addGlobal("infectionFraction", 0.01)

	}
	
}