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
#include <pins_arduino.h>
#include "termometry_rek.h"
#include "driver_2015_rekuperatora_leonardo.h"
#define ONE_WIRE_BUS 10 //termometry 10 dla went

//wolne piny 6,7,8,9,13

//dla MPC41 - potencjometru cyfrowego
//#define CS 9 //chip select dla MPC41 - potencjometru cyfrowego
//#define SHDN 8   //shutdown pin
//#define RS   7   //reset pin

#define PWM_OUT_NEW 9
#define PWM_OUT_USED 11

#define POTIOMETER_N 0 // potencjometr nawiewu przy ustawieniu 0 jest 10kohm między 01 a 03
#define POTIOMETER_W 1 // potencjometr wywiewu przy ustawieniu 0 jest 10kohm między 17 a 15 (lustrzane odbicie N) przy 255 jest 100ohm
// jezeli miedzy 7 a 4 jest 0ohm to wentylatory stoją
// jezeli miedzy 7 a 4 jest 10kohm to max speed ~2700rpm
#define USED_POT_FACTOR 11
#define NEW_POT_FACTOR 11
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 6, 5, 4, A4, A5);//zmieniamy 11->6
//LiquidCrystal lcd(12, 11, 5, 4, A4, A5);//0 zamiana na A5; 1 zamiana na A4


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

//byte resistance = 0;

//AH_MCP41xxx mcp42010;

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int half_revolutions_n = 0;
int rpm_n = 0;
int half_revolutions_w = 0;
int rpm_w = 0;

unsigned long lastmillis = 0;

char vs_lastEventNew[5];
char vs_lastEventUsed[5];

void setupI2C() {
	Wire.begin(REKUPERATYOR_ID);                // join i2c bus as master
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);
}

//void setupMCP() {
//	mcp42010.init_MCP42xxx (CS, SHDN, RS);  //initialisation
//}

void setupRPM(){
	digitalWrite(0, HIGH);//pull up 0 for rpm counting
	digitalWrite(1, HIGH);//pull up 1 for rpm counting
//	attachInterrupt(0, rpm_fan_n, FALLING);// pin 0 RPM dla nawiewu
//	attachInterrupt(3, rpm_fan_w, FALLING);// pin 1 RPM dla wywiewu
}

void setup() {
	setupRPM();
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	// Print a message to the LCD.
	setupI2C();
//	setupMCP();
	lcd.print("hello, world!");

	initTemps();

	delay(1000);

}

void initTemps() {
//	gf_currentTemps[NEW_IN] = 11.1;
//	gf_currentTemps[NEW_OUT] = 22.2;
//	gf_currentTemps[USED_IN] = 33.3;
//	gf_currentTemps[USED_OUT] = 44.4;
	gi_currentWentRPM[NEW_WENT] = 660;
	gi_currentWentRPM[USED_WENT] = 550;
	gi_desiredWentRPM[NEW_WENT] = 900;
	gi_desiredWentRPM[USED_WENT] = 700;
}

void handleRPMS(int ms){
//		lcd.clear();
//		lcd.setCursor(0, 0);
//		lcd.print("hrpm");

	int time = ms/100;
	detachInterrupt(2);//Disable interrupt when calculating
	detachInterrupt(3);//Disable interrupt when calculating
//        lcd.print("n:");lcd.print(half_revolutions_n);
//        lcd.print("t:");lcd.print(time);
//        delay(2000);
	 rpm_n = (half_revolutions_n*30 / time)*10; // Convert frecuency to RPM, note: this works for one interruption per full rotation. For two interrups per full rotation use half_revolutions * 30.
	 rpm_w = (half_revolutions_w*30 / time)*10;
	 half_revolutions_n = 0; // Restart the RPM counter
	 half_revolutions_w = 0; // Restart the RPM counter
	 lastmillis = millis(); // Uptade lasmillis
     attachInterrupt(2, rpm_fan_n, FALLING);// pin 0 RPM dla nawiewu
     attachInterrupt(3, rpm_fan_w, FALLING);// pin 1 RPM dla wywiewu
 	gi_currentWentRPM[NEW_WENT] = rpm_n;
 	gi_currentWentRPM[USED_WENT] = rpm_w;
}

void loop() {
	unsigned long elapsed = millis() - lastmillis;
	if (elapsed > 4000){ //Uptade every 4 second,
		handleRPMS(elapsed);
	}
	sensors.requestTemperatures();                // Send the command to get temperatures
	delay(100);
	getTemps();
	printTemps();
	delay(2000);
	lcd.clear();
	printRPMS();
	delay(2000);
	lcd.clear();
//	spinResistance(POTIOMETER_N);
//	spinResistance(POTIOMETER_W);
	printLastEvent();
        setNewRPM(gi_desiredWentRPM[NEW_WENT]);
        setUsedRPM(gi_desiredWentRPM[USED_WENT]);
	delay(2000);
	lcd.clear();

}

////metoda testowa
//void spinResistance(int po) {
//	for (int i = 0; i < 255; i++) {
//		lcd.clear();
//		lcd.setCursor(0, 0);
//		lcd.print("rez:");lcd.print(po);
//		lcd.setCursor(0, 1);
//		lcd.print("val:");lcd.print(i);
////		mcp42010.setValue(i,po);  //value range 0-255 (8-bit)
//		delay(500);
////		resistance++;
//	}
//}

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
	lcd.print("lstN:");
	lcd.print(vs_lastEventNew);
	lcd.setCursor(0, 1);
	lcd.print("lstU:");
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

void setNewRPM(int i){
	if(i>2700)i=2700;
int j = i+60;
	if(i<0)i=0;
//	gi_desiredWentRPM[NEW_WENT] = i;
	int val = j/NEW_POT_FACTOR;
        val = 255-val;
//	mcp42010.setValue(val,POTIOMETER_N);  //value range 0-255 (8-bit)
	analogWrite(PWM_OUT_NEW,val);
}

void setUsedRPM(int i){
	if(i>2600)i=2600;//100 bufor zeby nawiewu bylo wiecej niz wywiewu = nadcisnienie zeby kominem nie zaciagac CO
int j=i-120;
	if(j<0)j=0;
//	gi_desiredWentRPM[USED_WENT] = i;
	int val = j/USED_POT_FACTOR;
        val = 255 - val;
//	mcp42010.setValue(val,POTIOMETER_W);  //value range 0-255 (8-bit)
        analogWrite(PWM_OUT_USED,val);
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

void rpm_fan_n(){
 half_revolutions_n++;
}

void rpm_fan_w(){
 half_revolutions_w++;
}


