/*
 * vents_prod.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: wisnia
 */
#include "vents.h"
#include "vents_prod.h"
#include "prod.h"

//char buf[21];
void getWentString(char * buf) {
	Wire.requestFrom(REKUPERATYOR_ID, 20); // request 6 bytes from slave device #4
	//char buf[21];
	int i = 0;
	while (Wire.available()) { // slave may send less than requested
		char c = Wire.read(); // receive a byte as character
		buf[i++] = c;
	}

	//return in buf "12345678932126002700";
}

void setWents() {
	char buf[10];

	Wire.beginTransmission(REKUPERATYOR_ID); // transmit to device #4
	snprintf(buf, sizeof(buf), "%04d%04dx", getDesiredWentRPM(NEW_WENT) * 10, getDesiredWentRPM(USED_WENT) * 10);
	Wire.write(buf);        // sends 8
	Wire.endTransmission();    // stop transmitting

	printBottomStatusSent(buf);
}


