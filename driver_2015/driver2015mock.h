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

private:
	int hour;
	int minute;
	int day;
	int year;
	int month;
};

void getTMD();
void getTMN();
void getTXD();
void getTXN();



void setTMD(const char* value );
void setTXD(const char* value );
void setTMN(const char* value );
void setTXN(const char* value );
void logError(const char* s);
void  setTime(int hr,int min,int sec,int dy, int mnth, int yr);

#endif /* DRIVER2015MOCK_H_ */
