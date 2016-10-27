/*
 * power_tariffs.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "power_tariffs.h"

uint8_t gi_EE_Hour_Night_Start;
uint8_t gi_EE_Hour_Night_End;
uint8_t gi_EE_Hour_Day_Start;
uint8_t gi_EE_Hour_Day_End;
uint8_t gi_EE_Minute_Night_Shift; //difference between real time and the time on the energy meter - NOT IMPLEMENTED

uint8_t gi_Hour_Night_Start;
uint8_t gi_Hour_Night_End;
uint8_t gi_Hour_Day_Start;
uint8_t gi_Hour_Day_End;
uint8_t gi_Minute_Night_Shift; //difference between real time and the time on the energy meter - NOT IMPLEMENTED
boolean vb_taryfaNocna = false;

void decDzienStart() {
	eeprom_write_byte(&gi_EE_Hour_Day_Start, --gi_Hour_Day_Start); //dec dzien start
}

void incDzienStart() {
	eeprom_write_byte(&gi_EE_Hour_Day_Start, ++gi_Hour_Day_Start); //inc dzien start
}

void decDzienEnd() {
	eeprom_write_byte(&gi_EE_Hour_Day_End, --gi_Hour_Day_End); //dec dzien end
}

void incDzienEnd() {
	eeprom_write_byte(&gi_EE_Hour_Day_End, ++gi_Hour_Day_End); //inc dzien end
}

void decNocStart() {
	eeprom_write_byte(&gi_EE_Hour_Night_Start, --gi_Hour_Night_Start); //dec noc start
}

void incNocstart() {
	eeprom_write_byte(&gi_EE_Hour_Night_Start, ++gi_Hour_Night_Start); //inc noc start
}

void decNocEnd() {
	eeprom_write_byte(&gi_EE_Hour_Night_End, --gi_Hour_Night_End); //dec noc end
}

void incNocEnd() {
	eeprom_write_byte(&gi_EE_Hour_Night_End, ++gi_Hour_Night_End); //inc noc end
}

void decMinuteShift() {
	eeprom_write_byte(&gi_EE_Minute_Night_Shift, --gi_Minute_Night_Shift); //dec minute shift
}

void incMinuteShift() {
	eeprom_write_byte(&gi_EE_Minute_Night_Shift, ++gi_Minute_Night_Shift); //
}

boolean isNightTariff() {
	return vb_taryfaNocna;
}

//wczytaj ustawienia strefy noc/dzien z EE lub ustaw defaulty
void initConfigStrefy() {
	gi_Hour_Night_Start = eeprom_read_byte((uint8_t*) (&gi_EE_Hour_Night_Start));
	if (gi_Hour_Night_Start == 255) {
		gi_Hour_Night_Start = 22;
	}
	gi_Hour_Night_End = eeprom_read_byte((uint8_t*) (&gi_EE_Hour_Night_End));
	if (gi_Hour_Night_End == 255) {
		gi_Hour_Night_End = 7;
	}
	gi_Hour_Day_Start = eeprom_read_byte((uint8_t*) (&gi_EE_Hour_Day_Start));
	if (gi_Hour_Day_Start == 255) {
		gi_Hour_Day_Start = 13;
	}
	gi_Hour_Day_End = eeprom_read_byte((uint8_t*) (&gi_EE_Hour_Day_End));
	if (gi_Hour_Day_End == 255) {
		gi_Hour_Day_End = 15;
	}
	gi_Minute_Night_Shift = eeprom_read_byte((uint8_t*) (&gi_Minute_Night_Shift));
	if (gi_Minute_Night_Shift == 255) {
		gi_Minute_Night_Shift = 5;
	}
}

// od 21 do 7 i od 13 do 15 => taryfa nocna
void checkAndChangeTariff() {
	time_t current = now(); //get current time
	int h = hour(current - (gi_Minute_Night_Shift * SECS_PER_MIN)); //get hour shifted
	//int m = minute();

	vb_taryfaNocna = (h >= gi_Hour_Night_Start) //21 do 23
	|| (h < gi_Hour_Night_End) //od 00 do 6:59
			|| (h >= gi_Hour_Day_Start && h < gi_Hour_Day_End); // od 13 do 14:59
}
