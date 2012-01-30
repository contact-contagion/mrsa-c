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
			'Detailed Place Rule'], 3)
		
		// Add a chooser for showing person movement.
		addChooserWL('showPersonMovement', 'Show Person Movement:',
			['Yes',
			'No'], 1)
		
		// Declare the default data set index.
		int defaultDataSetIndex = 4
				
		// Add a chooser for person input files.
		addChooserWL('personsInputFile', 'Persons Input File:',
			['None',
			'../MRSA Inputs/subsets/60615/60615_people_sample.csv',
			'../MRSA Inputs/subsets/60615/60615_people.csv',
			'../MRSA Inputs/subsets/60615_60653/60615_60653_people.csv',
			'../MRSA Inputs/subsets/60615_60653_60609/60615_60653_60609_people.csv',],
			defaultDataSetIndex)
		
		// Add a chooser for place input files.
		addChooserWL('placesInputFile', 'Places Input File:',
			['None',
			'../MRSA Inputs/subsets/60615/60615_places_sample.csv',
			'../MRSA Inputs/subsets/60615/60615_places.csv',
			'../MRSA Inputs/subsets/60615_60653/60615_60653_places.csv',
			'../MRSA Inputs/subsets/60615_60653_60609/60615_60653_60609_places.csv'],
			defaultDataSetIndex)

		// Add a chooser for place input files.
		addChooserWL('activitiesInputFile', 'Activities Input File:',
			['None',
			'../MRSA Inputs/subsets/60615/60615_activities_sample.csv',
			'../MRSA Inputs/subsets/60615/60615_activities.csv',
			'../MRSA Inputs/subsets/60615_60653/60615_60653_activities.csv',
			'../MRSA Inputs/subsets/60615_60653_60609/60615_60653_60609_activities.csv'],
			defaultDataSetIndex)

		// The transmission parameter ranges are taken from an email
		// from Charles M. Macal with subject
		// "012512 Updated transmission parameters for MRSA ABM"
		// dated January 25, 2012 1:41:47 PM CST.
		//	
		// The central information is as follows:
		//
		// Average duration of an infection:
		// (1) Uniformly distributed over [7,14] days OR (2) (7 + Exponential[1/3.5]) days
		// 
		// Range for A: {1.24598E-5, 6.09131E-5}  Range Factor: 4.88877
		// Range for B: {2.30598E-5, 2.84505E-4}  Range Factor: 12.3377
		// Range for C: {2.37945E-3, 7.86860E-3}  Range Factor: 3.30689
		// Range for D: {1.18973E-2, 2.67532E-2}  Range Factor: 2.24869
		// Range for E: {5.83117E-5, 1.04594E-4}  Range Factor: 1.7937
			
		// A.
		addGlobal("a", 3.00000E-5)// Old Fit: 1.37058E-06 Original from MD&CM: 1.37058E-05
		
		// B.
		addGlobal("b", 1.50000E-4)// Old Fit: 0.000045    Original from MD&CM: 0.000284505

		// C.
		addGlobal("c", 5.00000E-3)// Old Fit: 0.00191008  Original from MD&CM: 0.00309173

		// D.
		addGlobal("d", 2.00000E-2)// Old Fit: 0.00718554  Original from MD&CM: 0.000451166

		// E.
		addGlobal("e", 8.00000E-5)// Old Fit: 0.000111936 Original from MD&CM: 0.000111936
		
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
		addGlobal("minimumInfectionPeriod", 7)
		
		
	}
	
}