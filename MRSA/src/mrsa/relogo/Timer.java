package mrsa.relogo;

// This is the performance timing class.
public class Timer {
	
	// The tracking state.
	public static boolean counting = false;
	
	// The start time.
	public static long startTime = 0;
	
	// The stop time.
	public static long stopTime = 0;

	// The starting time tracker.
	public static void start() {
		
		// Start counting.
		counting = true;
		
		// Note the start time.
		startTime = System.nanoTime();
		
	}
	
	// The ending time tracker.
	public static void stop() {

		// Note the start time.
		stopTime = System.nanoTime();

		// Stop counting.
		counting = false;		
		
	}
	
	// The time reporter.
	public static void show() {

		// Show the time.
		show(0);
		
	}
	
	// The time reporter.
	public static void show(int indentDepth) {
		
		// Indent as requested.
		for (int i = 0; i< indentDepth; i++) System.out.print(" ");
		
		// Report the time.
		if (counting) {
			System.out.println((double) (System.nanoTime() - startTime));
		} else {
			System.out.println((double) (stopTime - startTime));
		}
		
	}
	
}
