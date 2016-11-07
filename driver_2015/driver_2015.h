#ifndef _driver_2015_H_
#define _driver_2015_H_

#include "driver_net.h"
#include "driver_SD.h"
#include "driver_keyboard.h"
#include "termometry_prod.h"
#include "vents_prod.h"

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
char* setWN(char* value);
// set went used rpm
char* setWU(char* value);

// set went airing new rpm
char* setWWN(char* value);

// set went airing used rpm
char* setWWU(char* value);

char* setTMD(char* value);
char* setTXD(char* value);
char* setTMN(char* value);
char* setTXN(char* value);
char* logError(char* s);

void checkAndChangeTariff();

int setupRTC(int line);

void checkDog();
void patDog();
void logCurrentStatusToFile();

#endif /* _driver_2015_H_ */
