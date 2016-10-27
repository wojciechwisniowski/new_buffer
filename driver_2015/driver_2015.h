#ifndef _driver_2015_H_
#define _driver_2015_H_

#include <Time.h>
#include <DS3232RTC.h>  // better RTC
#include <Wire.h>
#include <EEPROM.h>
#include <avr/eeprom.h>
#include <SPI.h>

//#include "ApplicationMonitor.h"

#include "power_tariffs.h"//no our includes inside
#include "screens.h"//no our includes inside
#include "termometry.h"//no our includes inside
#include "screens_status.h"//no our includes inside
#include "power_tariffs.h"//no our includes inside
#include "vents.h"//no our includes inside
#include "driver_net.h"
#include "driver_buffer.h"

#include "driver_SD.h"
//#include "screens_status.h"//no our includes inside

#include "driver_keyboard.h"
//#include "power_tariffs_api.h"//no our includes inside

//#include "screens.h"//no our includes inside





char* getTMD();
char* getTMN();
char* getTXD();
char* getTXN();

// get went new rpm
char* getWN();
// get went used rpm
char* getWU();

// get went airing new rpm
char* getWWN();

// get went airing used rpm
char* getWWU();

// set went new rpm
char* setWN(char* value );
// set went used rpm
char* setWU(char* value );

// set went airing new rpm
char* setWWN(char* value );

// set went airing used rpm
char* setWWU(char* value );

char* setTMD(char* value );
char* setTXD(char* value );
char* setTMN(char* value );
char* setTXN(char* value );
char* logError(char* s);


void checkAndChangeTariff();



int setupRTC(int line);

void patDog();
void logCurrentStatusToFile();


#endif /* _driver_2015_H_ */
