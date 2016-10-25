// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _driver_2015_rekuperatora_leonardo_H_
#define _driver_2015_rekuperatora_leonardo_H_
#include "Arduino.h"
//add your includes for the project driver_2015_rekuperatora_leonardo here


//end of add your includes here


//add your function definitions for the project driver_2015_rekuperatora_leonardo here
void receiveEvent(int howMany);
void requestEvent();

void getTemps();
void initTemps();
void printLastEvent();
void printRPMS();
void printTemps();

void rpm_fan_n();
void rpm_fan_w();

void setNewRPM(int i);
void setUsedRPM(int i);



//Do not add code below this line
#endif /* _driver_2015_rekuperatora_leonardo_H_ */
