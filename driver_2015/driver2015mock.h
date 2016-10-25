/*
 * driver2015mock.h
 *
 *  Created on: Oct 6, 2016
 *      Author: wisnia
 */


#ifndef DRIVER2015MOCK_H_
#define DRIVER2015MOCK_H_

class driver_2015_mock {
public:
	driver_2015_mock();
	virtual ~driver_2015_mock();
};

void getTMD();
void getTMN();
void getTXD();
void getTXN();

// get went new rpm
void getWN();
// get went used rpm
void getWU();

// get went airing new rpm
void getWWN();

// get went airing used rpm
void getWWU();

// set went new rpm
void setWN(const char* value );
// set went used rpm
void setWU(const char* value);

// set went airing new rpm
void setWWN(const char* value );

// set went airing used rpm
void setWWU(const char* value );

void setTMD(const char* value );
void setTXD(const char* value );
void setTMN(const char* value );
void setTXN(const char* value );
void logError(const char* s);


#endif /* DRIVER2015MOCK_H_ */
