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
		
		// Add a chooser for place input files.
		addChooserWL('transitionRule', 'Transition Rule:',
			['None',
			'Simple ',
			'Detailed'], 2)

		// Add a chooser for person input files.
		addChooserWL('personsInputFile', 'Persons Input File:',
			['None',
			'data/60615_persons_sample.csv',
			'data/60615_persons.csv'], 1)
		
		// Add a chooser for place input files.
		addChooserWL('placesInputFile', 'Places Input File:',
			['None',
			'data/60615_places_sample.csv',
			'data/60615_places.csv'], 1)

		// Add a chooser for place input files.
		addChooserWL('activitiesInputFile', 'Activities Input File:',
			['None',
			'data/60615_activities_sample.csv',
			'data/60615_activities_sample_small.csv'], 2)

		// Add a global maximum risk variable.
		addGlobal("maximumRisk", 4)
		
		// Add a global infection fraction for testing purposes.
		addGlobal("infectionFraction", 0.01)

		// A means that 1,000 weeks of exposure yields p(infection)=1 and we will use a simple, time-independent exponential increase function for interpolation.
		addSliderWL("A", "A", 0, 0.01, 1.0, 0.01)
		
		// B will use simple, time-independent exponential decline with 38% of C moving to I after 10 weeks.
		addSliderWL("B", "B", 0, 0.01, 1.0, 0.05)

		// C is a simple exponential function of the time the agent has been infected such that 21% are infected at 3 weeks.
		addSliderWL("C", "C", 0, 0.01, 1.0, 0.05)

		// For D we assume everyone gets antibiotics that work as follows (Kaplan–Meier Survival Function) (We Will Only Use the Median for Now):
		addSliderWL("D", "D", 0, 0.01, 1.0, 0.05)

		// For E we assume everyone gets antibiotics that work as follows (Kaplan–Meier Survival Function) (We Will Only Use the Median for Now):
		addSliderWL("E", "E", 0, 0.01, 1.0, 0.05)

	}
	
}