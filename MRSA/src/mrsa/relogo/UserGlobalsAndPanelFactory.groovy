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
		
		// Add a chooser for changing behavior rules.
		addChooserWL('behaviorRule', 'Behavior Rule:',
			['Uniform',
			'Clustered by HH',
			'Cluster by HH and Address'], 0)
	
		// The behavior rule threshold.
		addSliderWL("fasterResponsePercentage", "Faster Response Percentage (>CL)", 0, 1, 100, 50)

		// The initial infected count.
		addSliderWL("initialInfectedCount", "Initial Infected Count", 0, 1, 1000, 5)

		// The initial colonization percentage.
		addSliderWL("initialColonizationPercentage", "Initial Colonization Percentage", 0, 1, 100, 3)
				
		// Add a chooser for selecting transition rules.
		addChooserWL('transitionRule', 'Transition Rule:',
			['None',
			'Simple',
			'Detailed'], 2)
		
		// Add a chooser for showing person movement.
		addChooserWL('showPersonMovement', 'Show Person Movement:',
			['Yes',
			'No'], 1)
				
		// Add a chooser for person input files.
		addChooserWL('personsInputFile', 'Persons Input File:',
			['None',
			'../Prototype_Model_Inputs/60615/sample_60615_people.csv',
			'../Prototype_Model_Inputs/60615/all_60615_people.csv'], 2)
		
		// Add a chooser for place input files.
		addChooserWL('placesInputFile', 'Places Input File:',
			['None',
			'../Prototype_Model_Inputs/60615/sample_60615_places.csv',
			'../Prototype_Model_Inputs/60615/all_60615_places.csv'], 2)

		// Add a chooser for place input files.
		addChooserWL('activitiesInputFile', 'Activities Input File:',
			['None',
			'../Prototype_Model_Inputs/60615/sample_60615_activities.csv',
			'../Prototype_Model_Inputs/60615/all_60615_activities.csv'], 2)

		// A.
		addGlobal("a", 0.0000137058)
		
		// B.
		// 0.000284505 Original value, rapid infection growth (~30 people after 7 days)
		// 0.0001      Slight infection growth (~10 people after 7 days)
		addGlobal("b", 0.00009)

		// C.
		addGlobal("c", 0.00309173)

		// D.
		addGlobal("d", 0.000451166)

		// E.
		addGlobal("e", 0.000111936)
		
		// Add a global maximum risk variable.
		addGlobal("maximumRisk", 4)
				
		// The total number of people who are currently uncolonized.
		addGlobal("totalUncolonized", 0)

		// The total number of people who are currently colonized.
		addGlobal("totalColonized", 0)

		// The total number of people who are currently infected.
		addGlobal("totalInfected", 0)

	}
	
}