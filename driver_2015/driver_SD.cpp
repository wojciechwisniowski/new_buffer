/*
 * driver_SD.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "driver_SD.h"

File myFile2;
boolean gb_fileOpen = false;

int setupSD(int line) {

	print0(line, "Initializing SD card...");
	// On the Ethernet Shield, CS is pin 4. It's set as an output by default.
	// Note that even if it's not used as the CS pin, the hardware SS pin
	// (10 on most Arduino boards, 53 on the Mega) must be left as an output
	// or the SD library functions will not work.
	pinMode(53, OUTPUT);

	if (!SD.begin(4)) {
		print0(line + 1, "initialization failed!");
	} else {
		print0(line + 1, "initialization done.");
	}
	return line + 1;
}


//sd
/*
 SD card read/write
 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 */

//log current status to log file in csv format
//format:
void logCurrentStatusToFile() {
	if (!gb_fileOpen) {
		gb_fileOpen = true;
		char fileName[13];
		char buf[200];
		sprintf(fileName, "%04d%02d%02d.log", year(), month(), day());
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.

		myFile2 = SD.open(fileName, FILE_WRITE);

// if the file opened okay, write to it:
		if (myFile2) {
			sprintf(buf, "opned:%s", fileName);
			printBottomStatus(buf);
			//delay(1000);

			if (myFile2.size() == 0) {
				myFile2.println("hour,minute,second,t0,t1,t2,t3,t4-podlOut,t5-podlIn,buforGrzeje,pompaMiesza,pompaPodlogowa,tNI_r,tNO_r,tUI_r,tUO_r,RPM_NEW,RPM_USED");
			}
			char t0[6], t1[6], t2[6], t3[6], t4[6], t5[6], tNI_r[6], tNO_r[6], tUI_r[6], tUO_r[6];
			dtostrf(gf_currentTemps[0], 3, 2, t0);
			dtostrf(gf_currentTemps[1], 3, 2, t1);
			dtostrf(gf_currentTemps[2], 3, 2, t2);
			dtostrf(gf_currentTemps[3], 3, 2, t3);
			dtostrf(gf_currentTemps[PODLOGA_OUT], 3, 2, t4);
			dtostrf(gf_currentTemps[PODLOGA_IN], 3, 2, t5);
			dtostrf(getCurrentTemReku(NEW_IN), 3, 2, tNI_r);
			dtostrf(getCurrentTemReku(NEW_OUT), 3, 2, tNO_r);
			dtostrf(getCurrentTemReku(USED_IN), 3, 2, tUI_r);
			dtostrf(getCurrentTemReku(USED_OUT), 3, 2, tUO_r);
			//
			sprintf(buf, "%02d:%02d:%02d,%s,%s,%s,%s,%s,%s,%01d,%01d,%01d,%s,%s,%s,%s,%04d,%04d", hour(), minute(), second(),   //
					t0, t1, t2, t3, t4, t5, isBufforHeating() ? "1" : "0",   //
					vb_pompaMieszajacaPracuje ? "1" : "0",   //
					vb_pompaPodlogowaPracuje ? "1" : "0",   //
					tNI_r, tNO_r, tUI_r, tUO_r,   //
					getCurrentVentRPM(NEW_WENT), getCurrentVentRPM(USED_WENT));

			myFile2.println(buf);
			// close the file:
			myFile2.flush();
			myFile2.close();
			sprintf(buf, "closed:%s", fileName);
			printBottomStatus(buf);
		} else {
			// if the file didn't open, print an error:
			sprintf(buf, "error opening:%s", fileName);
			printBottomStatus(buf);
			//delay(1000);
		}
		gb_fileOpen = false;
	}
}

