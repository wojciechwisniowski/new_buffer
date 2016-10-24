/*
 * driver2015mock.h
 *
 *  Created on: Oct 6, 2016
 *      Author: wisnia
 */
#include "MyPrint.h"

#ifndef DRIVER2015MOCK_H_
#define DRIVER2015MOCK_H_

class driver_2015_mock {
public:
	driver_2015_mock();
	virtual ~driver_2015_mock();
};

void getTMD(const Print& client);
void getTMN(const Print& client);
void getTXD(const Print& client);
void getTXN(const Print& client);

// get went new rpm
void getWN(const Print& client);
// get went used rpm
void getWU(const Print& client);

// get went airing new rpm
void getWWN(const Print& client);

// get went airing used rpm
void getWWU(const Print& client);

// set went new rpm
void setWN(const char* value, const Print& client);
// set went used rpm
void setWU(const char* value, const Print& client);

// set went airing new rpm
void setWWN(const char* value, const Print& client);

// set went airing used rpm
void setWWU(const char* value, const Print& client);

void setTMD(const char* value, const Print& client);
void setTXD(const char* value, const Print& client);
void setTMN(const char* value, const Print& client);
void setTXN(const char* value, const Print& client);
void logError(const char* s);


#endif /* DRIVER2015MOCK_H_ */
