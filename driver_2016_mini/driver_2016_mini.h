// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _driver_2016_mini_H_
#define _driver_2016_mini_H_
#include "Arduino.h"
//add your includes for the project driver_2016_mini here


//end of add your includes here


//add your function definitions for the project driver_2016_mini here
void receiveEvent(int howMany);
void requestEvent();
void ledSignal(int blinks, int ms);
void resetMaster();



//Do not add code below this line
#endif /* _driver_2016_mini_H_ */
