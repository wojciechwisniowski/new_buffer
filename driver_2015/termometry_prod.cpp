/*
 * termometry.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "termometry.h"
#include "termometry_prod.h"
#include "prod.h"

const int GI_PIN_ONE_WIRE_BUS = 40; //termometry 40 dla buf

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

float getTempC(int nr) {
	return sensors.getTempC((uint8_t*) gDA_sensors[nr]);
}

void requestTemperatures() {
	sensors.requestTemperatures();  // Send the command to get temperatures
}

//from Dallas Temp example Simple
void setupDS() {
	sensors.begin();   // Start up the library
}

