// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _driver_keyboard_H_
#define _driver_keyboard_H_
//includes for the project driver_keyboard here
#include <Keypad.h>
#include <DS3232RTC.h>  // better RTC

#include "power_tariffs.h"//no our includes inside
#include "termometry.h"//no our includes inside
#include "vents.h"//no our includes inside
#include "vents_prod.h"//no our includes inside
#include "screens.h"//no our includes inside

//end of add your includes here

////add your function definitions for the project driver_keyboard here
////keypad globals
//const byte ROWS = 4; // Four rows
//const byte COLS = 4; // Four
//// Define the Keymap
//extern char keys[ROWS][COLS];
//// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
//extern byte rowPins[ROWS];
//// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
//extern byte colPins[COLS];
////Create the Keypad
//extern Keypad kpd;
////keypad globals

void keypadEvent(KeypadEvent key);
Keypad getKpd();
void configWent(char key);
void configTemp(char key);
void configGodz(char key);
void configGodz2(char key);
void checkKey(char key);

//Do not add code below this line
#endif /* _driver_keyboard_H_ */
