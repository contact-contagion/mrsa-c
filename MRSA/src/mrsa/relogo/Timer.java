package mrsa.relogo;

// This is the performance timing class.
public class Timer {
	
	// The tracking state.
	public boolean counting = false;
	
	// The start time.
	public long startTime = 0;
	
	// The stop time.
	public long stopTime = 0;

	// The starting time tracker.
	public void start() {
		
		// Start counting.
		counting = true;
		
		// Note the start time.
		startTime = System.nanoTime();
		
	}
	
	// The ending time tracker.
	public void stop() {

		// Note the start time.
		stopTime = System.nanoTime();

		// Stop counting.
		counting = false;		
		
	}
	
	// The time reporter.
	public void show() {

		// Show the time.
		show(0);
		
	}
	
	// The time reporter.
	public void show(int indentDepth) {
		
		// Indent as requested.
		for (int i = 0; i< indentDepth; i++) System.out.print(" ");
		
		// Report the time.
		if (counting) {
			System.out.println(System.nanoTime() - startTime);
		} else {
			System.out.println(stopTime - startTime);
		}
		
	}
	
}
