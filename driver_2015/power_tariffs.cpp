/*
 * power_tariffs.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "power_tariffs.h"
#include "power_tariffs_prod.h"
#include "prod.h"

int gi_EE_Hour_Night_Start;
int gi_EE_Hour_Night_End;
int gi_EE_Hour_Day_Start;
int gi_EE_Hour_Day_End;
int gi_EE_Minute_Night_Shift; //difference between real time and the time on the energy meter - NOT IMPLEMENTED

int gi_Hour_Night_Start;
int gi_Hour_Night_End;
int gi_Hour_Day_Start;
int gi_Hour_Day_End;
int gi_Minute_Night_Shift; //difference between real time and the time on the energy meter - NOT IMPLEMENTED

const int ci_defaultHourNightStart = 22; // G12W tariff night hour 22:00-7:00 and 13:00-15:00 and whole Sat and Sun
const int ci_defaultHourNightEnd = 7; // G12W tariff night hour 22:00-7:00 and 13:00-15:00 and whole Sat and Sun
const int ci_defaultHourDayStart = 13; // G12W tariff night hour 22:00-7:00 and 13:00-15:00 and whole Sat and Sun
const int ci_defaultHourDayEnd = 15; // G12W tariff night hour 22:00-7:00 and 13:00-15:00 and whole Sat and Sun
const int ci_defaultMinuteNightShift = 5; //default shift between RTC and meter outside house

int getMinuteNightShift() {
	return gi_Minute_Night_Shift;
}

int getHourDayStart() {
	return gi_Hour_Day_Start;
}
int getHourDayEnd() {
	return gi_Hour_Day_End;
}
int getHourNightStart() {
	return gi_Hour_Night_Start;
}
int getHourNightEnd() {
	return gi_Hour_Night_End;
}

void decDzienStart() {
	writeTemToEprom(&gi_EE_Hour_Day_Start, --gi_Hour_Day_Start); //dec dzien start
}

void incDzienStart() {
	writeTemToEprom(&gi_EE_Hour_Day_Start, ++gi_Hour_Day_Start); //inc dzien start
}

void decDzienEnd() {
	writeTemToEprom(&gi_EE_Hour_Day_End, --gi_Hour_Day_End); //dec dzien end
}

void incDzienEnd() {
	writeTemToEprom(&gi_EE_Hour_Day_End, ++gi_Hour_Day_End); //inc dzien end
}

void decNocStart() {
	writeTemToEprom(&gi_EE_Hour_Night_Start, --gi_Hour_Night_Start); //dec noc start
}

void incNocstart() {
	writeTemToEprom(&gi_EE_Hour_Night_Start, ++gi_Hour_Night_Start); //inc noc start
}

void decNocEnd() {
	writeTemToEprom(&gi_EE_Hour_Night_End, --gi_Hour_Night_End); //dec noc end
}

void incNocEnd() {
	writeTemToEprom(&gi_EE_Hour_Night_End, ++gi_Hour_Night_End); //inc noc end
}

void decMinuteShift() {
	writeTemToEprom(&gi_EE_Minute_Night_Shift, --gi_Minute_Night_Shift); //dec minute shift
}

void incMinuteShift() {
	writeTemToEprom(&gi_EE_Minute_Night_Shift, ++gi_Minute_Night_Shift); //
}

//check if h is between 0 and 24
bool checkValidHour(int h) {
	return h >= 0 && h <= 24;
}

bool checkValidMinute(int m) {
	return m >= 0 && m <= 60;
}

void setHourNightStart(int h) {
	if (checkValidHour(h)) {
		gi_Hour_Night_Start = h;
	}
}

void setHourNightEnd(int h) {
	if (checkValidHour(h)) {
		gi_Hour_Night_End = h;
	}
}

void setHourDayStart(int h) {
	if (checkValidHour(h)) {
		gi_Hour_Day_Start = h;
	}
}

void setHourDayEnd(int h) {
	if (checkValidHour(h)) {
		gi_Hour_Day_End = h;
	}
}

int setMinuteNightShift(int m) {
	if (checkValidMinute(m)) {
		gi_Minute_Night_Shift = m;
	}
}

//wczytaj ustawienia strefy noc/dzien z EE lub ustaw defaulty
void initConfigStrefy() {
	int vi_h = 0;
	vi_h = readTemFromEprom((int*) (&gi_EE_Hour_Night_Start));
	if (checkValidHour(vi_h)) {
		setHourNightStart(vi_h);
	} else {
		setHourNightStart(ci_defaultHourNightStart);
	}

	vi_h = readTemFromEprom((int*) (&gi_EE_Hour_Night_End));
	if (checkValidHour(vi_h)) {
		setHourNightEnd(vi_h);
	} else {
		setHourNightEnd(ci_defaultHourNightEnd);
	}

	vi_h = readTemFromEprom((int*) (&gi_EE_Hour_Day_Start));
	if (checkValidHour(vi_h)) {
		setHourDayStart(vi_h);
	} else {
		setHourDayStart(ci_defaultHourDayStart);
	}

	vi_h = readTemFromEprom((int*) (&gi_EE_Hour_Day_End));
	if (checkValidHour(vi_h)) {
		setHourDayEnd(vi_h);
	} else {
		setHourDayEnd(ci_defaultHourDayEnd);
	}

	int vi_m = readTemFromEprom((int*) (&gi_Minute_Night_Shift));
	if (checkValidMinute(vi_m)) {
		setMinuteNightShift(vi_m);
	} else {
		setMinuteNightShift(ci_defaultMinuteNightShift);
	}
}

// od 13 do 14:59
bool isDayCheapTariff(int h) {
	return (h >= gi_Hour_Day_Start && h < gi_Hour_Day_End); // od 13 do 14:59
}

bool isNightCheapTariff(int h) {
	return (h >= gi_Hour_Night_Start) //22 do 23
	|| (h < gi_Hour_Night_End); //od 00 do 6:59
}
bool isWeekend(int dayOfTheWeek) {
	return dayOfTheWeek == 1 //Sunday
	|| dayOfTheWeek == 7; //Saturday
}

// od 22 do 7 i od 13 do 15 => taryfa nocna
bool isCheapTariff(int h, int dayOfTheWeek) {
	return isWeekend(dayOfTheWeek) || isNightCheapTariff(h) || isDayCheapTariff(h);
}
