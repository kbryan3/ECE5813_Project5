## PES Project Five Readme

Student Name: Kyle Bryan

#### Running Code, General description

The program is meant to implement a UART and circular buffer.  It has multiple
run modes with which it runs it, which is explained below.  At a high level
the program takes in inputs from a command prompt and outputs one of two things:
In echo mode the program outputs the character that was inputted.  In application
mode the program keeps a tally of character count for each word that is inputted, the application is meant to have the user input one word at a time followed by a carriage return.

This code is written to be run on the FRDM board using MCUExpresso.  To run the project, you must first enable the UART clock in the clock GUI and then use defines.h to select the mode.

##### Use Defines.h to select run mode


###### Log Type Options
The log types options selects if you want to run in:  
*Test Mode* - Unit Test is run(set to 115200 baud), overwrites all selections and only runs the unit test, then exits.  
*Debug Mode* - prints more detailed info about processes that are running during runtime  
*Status Mode* - only prints the application/echo info

###### Echo vs Application
Application selection selects whether you run in application mode or echo mode:  
*By commenting out ECHO the program runs in application mode.*

###### Interrupt Enable
Interrupts are enabled by uncommenting the interrupt #define.


##### Repository Contents
###### Source Code
*main.c*, *led_control.h*, *led_control.c*, *logger.h*,*logger.c*,
, *testSuite.c/h*, *circularbuffer.c/h*, *uart.c/h*, *defines.h*

The main.c is where the main program is run.  It calls uart functions which in turn leverage the circularbuffer.  The logger
is used to write program info to the console, while the LED-control is used to
control the LED showing status of the program.

There is also the TestSuite.c/h files which are leveraged to run the uCUnit test This requires both the System and uCUnit.h
files to run properly(provided from ucUnit.org).  To run in test mode the terminal needs to be set at 115200 baud.

There was some code taken from online sources and those are called out in the
files they are used in (logger, uart).

###### Extra Project Documents
*echo_a.pdf*, *FMEA.pdf*,

These documents are in the /.bin folder and are other(non-source code) project5
deliverables.  They include
an oscilloscope capture of an 'a' echo and an FMEA for Project5.


#### Observations for Project:
Though I got most of the project working, I ran into a few major issues with this project involving the uart.  One was with regards to the logger and the application running at the same time and the other was with regards to the uart interrupt service routine. Once I start writing logger output to the console my code has trouble recovering.  And with regards to ISR,  it kept clearing my pointers(I tried making them global variables) so I could not update my circular buffers with new values and was unable to figure out a solution.  Unfortunately this means my application portion of the Interrupt mode does not work.
