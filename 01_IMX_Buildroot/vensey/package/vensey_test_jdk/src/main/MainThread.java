package main;

public class MainThread {

	public static void main(String[] args) {
		
		/*
		 * Constructing the main thread... 
		 */
		main_instance = new MainThread();
       
	}
	
	/**
	 * Create the application.
	 */
	public MainThread() { 
		writeConsole("Hello World, JDK");		
	}
	
	/**
	 * Debug outputs
	 */ 	
	private void writeConsole(String msg){
    		System.out.println(this.getClass().getName() + ": " + msg);
    	}

}
