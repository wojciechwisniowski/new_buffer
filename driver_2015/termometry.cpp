/*
 * termometry.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "termometry.h"

float gf_currentTemps[TEMPCOUNT];
uint8_t gi_EE_Temp_Min_Day;
uint8_t gi_EE_Temp_Max_Day;
uint8_t gi_EE_Temp_Min_Night;
uint8_t gi_EE_Temp_Max_Night;
uint8_t gi_EE_Temp_Mixing_Start;

uint8_t gi_Temp_Min_Day;
uint8_t gi_Temp_Max_Day;
uint8_t gi_Temp_Min_Night;
uint8_t gi_Temp_Max_Night;
uint8_t gi_Temp_Mixing_Start;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(GI_PIN_ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

DeviceAddress gDA_sensors[TEMPCOUNT] = { //identyfikatory czujników

		{ 0x28, 0xFF, 0x72, 0x34, 0x63, 0x14, 0x03, 0x94 } //0
				, { 0x28, 0xFF, 0xEE, 0x61, 0x63, 0x14, 0x03, 0x5B } //1
				, { 0x28, 0xFF, 0x61, 0x19, 0x62, 0x14, 0x03, 0xE0 } //2
				, { 0x28, 0xFF, 0x35, 0x3F, 0x62, 0x14, 0x03, 0x13 } //3
				, { 0x28, 0xFF, 0xF4, 0x99, 0x54, 0x14, 0x00, 0xB1 } //4 podOut
				, { 0x28, 0xFF, 0x4C, 0x8D, 0x54, 0x14, 0x00, 0xF6 } //5 podIn
		};



//wczytaj ustawienia temperatur z EE lub ustaw default
void initConfigTemp() {
	gi_Temp_Min_Day = eeprom_read_byte((uint8_t*) (&gi_EE_Temp_Min_Day));
	if (gi_Temp_Min_Day == 255) {
		gi_Temp_Min_Day = 29;
	}
	gi_Temp_Max_Day = eeprom_read_byte((uint8_t*) (&gi_EE_Temp_Max_Day));
	if (gi_Temp_Max_Day == 255) {
		gi_Temp_Max_Day = 31;
	}
	gi_Temp_Min_Night = eeprom_read_byte((uint8_t*) (&gi_EE_Temp_Min_Night));
	if (gi_Temp_Min_Night == 255) {
		gi_Temp_Min_Night = 60;
	}
	gi_Temp_Max_Night = eeprom_read_byte((uint8_t*) (&gi_EE_Temp_Max_Night));
	if (gi_Temp_Max_Night == 255) {
		gi_Temp_Max_Night = 80;
	}
	gi_Temp_Mixing_Start = eeprom_read_byte(
			(uint8_t*) (&gi_EE_Temp_Mixing_Start));
	if (gi_Temp_Mixing_Start == 255) {
		gi_Temp_Mixing_Start = 55;
	}
}


//from Dallas Temp example Simple
void setupDS() {
	sensors.begin();   // Start up the library
}




void decMinDzienna() {
	eeprom_write_byte(&gi_EE_Temp_Min_Day, --gi_Temp_Min_Day); //decMinDzienna
}
void incMinDzienna() {
	eeprom_write_byte(&gi_EE_Temp_Min_Day, ++gi_Temp_Min_Day); //incMinDzienna
}
void decMaxDzienna() {
	eeprom_write_byte(&gi_EE_Temp_Max_Day, --gi_Temp_Max_Day); //decMaxDzienna
}
void incMaxDzienna() {
	eeprom_write_byte(&gi_EE_Temp_Max_Day, ++gi_Temp_Max_Day); //incMaxDzienna
}
void decMinNocna() {
	eeprom_write_byte(&gi_EE_Temp_Min_Night, --gi_Temp_Min_Night); //decMinNocna
}
void incMinNocna() {
	eeprom_write_byte(&gi_EE_Temp_Min_Night, ++gi_Temp_Min_Night); //incMinNocna
}
void decMaxNocna() {
	eeprom_write_byte(&gi_EE_Temp_Max_Night, --gi_Temp_Max_Night); //decMaxNocna
}
void incMaxNocna() {
	eeprom_write_byte(&gi_EE_Temp_Max_Night, ++gi_Temp_Max_Night); //incMaxNocna
}
void decPompaMiesz() {
	eeprom_write_byte(&gi_EE_Temp_Mixing_Start, --gi_Temp_Mixing_Start); //dec PompaMiesz
}
void incPompaMiesz() {
	eeprom_write_byte(&gi_EE_Temp_Mixing_Start, ++gi_Temp_Mixing_Start); //inc PompaMiesz
}

