// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _driver_2015_term_get_H_
#define _driver_2015_term_get_H_
#include "Arduino.h"
//add your includes for the project driver_2015_term_get here

//end of add your includes here

//add your function definitions for the project driver_2015_term_get here
void setupSerial();
void setupDS();
void setupGLCD();
void setupRTC();
void printAddress(DeviceAddress deviceAddress);
void testTerm();

//Do not add code below this line
#endif /* _driver_2015_term_get_H_ */
