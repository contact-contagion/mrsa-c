package mrsa.relogo

import repast.simphony.relogo.factories.AbstractReLogoGlobalsAndPanelFactory

// The user panel configuration class.
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
			'Detailed Activity Rule',
			'Detailed Place Rule'], 2)
		
		// Add a chooser for showing person movement.
		addChooserWL('showPersonMovement', 'Show Person Movement:',
			['Yes',
			'No'], 0)
				
		// Add a chooser for person input files.
		addChooserWL('personsInputFile', 'Persons Input File:',
			['None',
			'../MRSA_Model_Inputs/sample_60615_people.csv',
			'../MRSA_Model_Inputs/all_60615_people.csv'], 2)
		
		// Add a chooser for place input files.
		addChooserWL('placesInputFile', 'Places Input File:',
			['None',
			'../MRSA_Model_Inputs/sample_60615_places.csv',
			'../MRSA_Model_Inputs/all_60615_places.csv'], 2)

		// Add a chooser for place input files.
		addChooserWL('activitiesInputFile', 'Activities Input File:',
			['None',
			'../MRSA_Model_Inputs/sample_60615_activities.csv',
			'../MRSA_Model_Inputs/all_60615_activities.csv'], 2)

		// A.
		addGlobal("a", 1.37058E-06) // Original from MD&CM: 1.37058E-05
		
		// B.
		addGlobal("b", 0.000045) // Original from MD&CM: 0.000284505

		// C.
		addGlobal("c", 0.00191008) // Original from MD&CM: 0.00309173

		// D.
		addGlobal("d", 0.00718554) // Original from MD&CM: 0.000451166

		// E.
		addGlobal("e", 0.000111936)
		
		// The C and D scaling factor for faster infection response.
		addGlobal("fasterResponseScalingFactor", 2.0)

		// Add a global maximum risk variable.
		addGlobal("maximumRisk", 4)
				
		// The total number of people who are currently uncolonized.
		addGlobal("totalUncolonized", 0)

		// The total number of people who are currently colonized.
		addGlobal("totalColonized", 0)

		// The total number of people who are currently infected.
		addGlobal("totalInfected", 0)

		// Minimum infection period in days.
		addGlobal("minimumInfectionPeriod", 5)
		
		
	}
	
}