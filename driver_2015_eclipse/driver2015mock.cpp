/*
 * driver2015mock.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: wisnia
 */

#include "driver2015mock.h"
#include "MyPrint.h"

driver_2015_mock::driver_2015_mock() {
	// TODO Auto-generated constructor stub

}

driver_2015_mock::~driver_2015_mock() {
	// TODO Auto-generated destructor stub
}

void getTMD(const Print& client){};
void getTMN(const Print& client){};
void getTXD(const Print& client){};
void getTXN(const Print& client){};

// get went new rpm
void getWN(const Print& client){};
// get went used rpm
void getWU(const Print& client){};

// get went airing new rpm
void getWWN(const Print& client){};

// get went airing used rpm
void getWWU(const Print& client){};

// set went new rpm
void setWN(const char* value, const Print& client){};
// set went used rpm
void setWU(const char* value, const Print& client){};

// set went airing new rpm
void setWWN(const char* value, const Print& client){};

// set went airing used rpm
void setWWU(const char* value, const Print& client){};

void setTMD(const char* value, const Print& client){};
void setTXD(const char* value, const Print& client){};
void setTMN(const char* value, const Print& client){};
void setTXN(const char* value, const Print& client){};
void logError(const char* s){};

