/*
 LiquidCrystal Library - scrollDisplayLeft() and scrollDisplayRight()

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD and uses the
 scrollDisplayLeft() and scrollDisplayRight() methods to scroll
 the text.

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/LiquidCrystalScroll

 */

// include the library code:
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "termometry_rek.h"
#define ONE_WIRE_BUS 10 //termometry 10 dla went

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 1, 0);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

char vs_lastEventNew[5];
char vs_lastEventUsed[5];

void setupI2C() {
	Wire.begin(REKUPERATYOR_ID);                // join i2c bus as master
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);
}

void setup() {
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	// Print a message to the LCD.
	setupI2C();
	lcd.print("hello, world!");
	initTemps();
	delay(1000);

}

void initTemps() {
//	gf_currentTemps[NEW_IN] = 11.1;
//	gf_currentTemps[NEW_OUT] = 22.2;
//	gf_currentTemps[USED_IN] = 33.3;
//	gf_currentTemps[USED_OUT] = 44.4;
	gi_currentWentRPM[NEW_WENT] = 1600;
	gi_currentWentRPM[USED_WENT] = 1700;
	gi_desiredWentRPM[NEW_WENT] = 1100;
	gi_desiredWentRPM[USED_WENT] = 1000;
}

void loop() {
	sensors.requestTemperatures();                // Send the command to get temperatures
	delay(100);
	getTemps();
	printTemps();
	delay(2000);
	lcd.clear();
	printRPMS();
	delay(2000);
	lcd.clear();
//	printLastEvent();
//	delay(2000);
//	lcd.clear();

}


void getTemps() {
	for (int i = 0; i < TEMPCOUNT_REKU; i++) {
		float tempC = sensors.getTempC(gDA_sensors_reku[i]);
		gf_currentTemps[i] = tempC;
	}
}
void printRPMS() {
	lcd.setCursor(0, 0);
	lcd.print("Nc:");
	lcd.print(gi_currentWentRPM[NEW_WENT]);
	lcd.print(" e:");
	lcd.print(gi_desiredWentRPM[NEW_WENT]);
	lcd.setCursor(0, 1);
	lcd.print("Uc:");
	lcd.print(gi_currentWentRPM[USED_WENT]);
	lcd.print(" e:");
	lcd.print(gi_desiredWentRPM[USED_WENT]);
}

void printTemps() {
	lcd.setCursor(0, 0);
	lcd.print("Ni:");
	lcd.print(gf_currentTemps[NEW_IN]);
	lcd.print(" o:");
	lcd.print(gf_currentTemps[NEW_OUT]);
	lcd.setCursor(0, 1);
	lcd.print("Ui:");
	lcd.print(gf_currentTemps[USED_IN]);
	lcd.print(" o:");
	lcd.print(gf_currentTemps[USED_OUT]);
}

void printLastEvent() {
	lcd.setCursor(0, 0);
	lcd.print(vs_lastEventNew);
	lcd.setCursor(0, 1);
	lcd.print(vs_lastEventUsed);

}

//master buffer writes to rekuperator xxxxyyyy ie 26002700 new/used
void receiveEvent(int howMany) {
	int i = 0;
	while ((1 < Wire.available()) && i < 9) { // last one is the end of transm
		if (i < 4)
			vs_lastEventNew[i++] = Wire.read(); // receive byte as a character
		else
			vs_lastEventUsed[i++ - 4] = Wire.read(); // receive byte as a character
	}
	while (Wire.available())
		Wire.read();	//read all trash

	gi_desiredWentRPM[NEW_WENT] = atoi(vs_lastEventNew);
	gi_desiredWentRPM[USED_WENT] = atoi(vs_lastEventUsed);
	printLastEvent();
	delay(100);
}

//master buff ask for status
void requestEvent() {

	int newRpm = 2700;
	int usedRpm = 2700;
	char buf[21];
	snprintf(buf, sizeof(buf), "%03d%03d%03d%03d%04d%4d", (int) (gf_currentTemps[NEW_IN] * 10), (int) (gf_currentTemps[NEW_OUT] * 10), (int) (gf_currentTemps[USED_IN] * 10),
			(int) (gf_currentTemps[USED_OUT] * 10), gi_currentWentRPM[NEW_WENT], gi_currentWentRPM[USED_WENT]);
	Wire.write(buf); // send temperatures in order 0,1,2,3 for 24.5 245 will be sent and then rpm-new rpm-used 21chars

//max obroty to 2700 na min
}

