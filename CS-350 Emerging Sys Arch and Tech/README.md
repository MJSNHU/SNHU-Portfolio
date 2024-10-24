-   Summarize the project and what problem it was solving.
	The thermostat prototype used input buttons to raise and lower the set temperature and an LED light to simulate the thermostat turning on whenever the set temperature was higher than the ambient temperature.  It used a task scheduler to check the state of the buttons, temperature reading and print data to the UART output at different intervals.  The code I wrote is in file 7-1.c  

-   What did you do particularly well?
	Designing the scheduler to loop through each task in one for loop while only triggering a given task at its individual period was a satisfying challenge. 

-   Where could you improve?

	The state machine I implemented essentially served as a flag, rather than progressing through states during each tick function.

-   What tools and/or resources are you adding to your support network?
	I've included a video demonstration and a flow chart to showcase how the design works.

-   What skills from this project will be particularly transferable to other projects and/or course work?
	Writing software to control hardware components, 

-   How did you make this project maintainable, readable, and adaptable?
	Writing code to initialize and execute the tasks had the same, repeatable steps which makes it easy to expand the number of tasks in the scheduler.  