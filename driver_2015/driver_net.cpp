/*
 * driver_net.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#include "driver_net.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x11 };
IPAddress ip(192, 168, 1, 200); //<<< ENTER YOUR IP ADDRESS HERE!!!

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(8080);

File myFile;

//REST API
/*
 Web Server Demo
 thrown together by Randy Sarafan

 Allows you to turn on and off an LED by entering different urls.

 To turn it on:
 http://your-IP-address/$1

 To turn it off:
 http://your-IP-address/$2

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Connect an LED to pin D2 and put it in series with a 220 ohm resistor to ground

 Based almost entirely upon Web Server by Tom Igoe and David Mellis

 Edit history:
 created 18 Dec 2009
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe

 */

void setupHttp() {
	pinMode(2, OUTPUT);
	// start the Ethernet connection and the server:
	Ethernet.begin(mac, ip);
	server.begin();
}

//from webServerExample

void printHtmlBufor(EthernetClient& client) {
	client.println(F("\"VER\":2.1"));
	for (int i = 0; i < TEMPCOUNT; i++) {

		float tempC = getTempC(i);

		if (i < 4) {
			client.print(",\"BUFOR_");
			client.print(i);
			client.print("\":");
			client.println(tempC);
		}
		//TODO czemu tu jest ustawiane ??????????
		setCurrentTemps(i, tempC);
	}
	client.print(",\"PODLOGA_OUT\":");
	client.println(getCurrentTemps(PODLOGA_OUT));
	client.print(",\"PODLOGA_IN\":");
	client.println(getCurrentTemps(PODLOGA_IN));
}

void printHtmlWent(EthernetClient& client) {
	client.print(",\"NEW_IN\":");
	client.println(getCurrentTemReku(NEW_IN));
	client.print(",\"USED_IN\":");
	client.println(getCurrentTemReku(USED_IN));
	client.print(",\"USED_OUT\":");
	client.println(getCurrentTemReku(USED_OUT));
	client.print(",\"NEW_OUT\":");
	client.println(getCurrentTemReku(NEW_OUT));
	client.print(",\"NEW_WENT\":");
	client.println(getCurrentVentRPM(NEW_WENT));
	client.print(",\"USED_WENT\":");
	client.println(getCurrentVentRPM(USED_WENT));
}
void printTimeStamp(EthernetClient& client) {
	char buf[13];
	time_t vt_now = now();
	client.print(F(",\"TIME\":"));
	// format the time in a buffer
	snprintf(buf, sizeof(buf), "\"%02d:%02d:%02d\"", hour(vt_now), minute(vt_now), second(vt_now));
	client.println(buf);
	client.print(F(",\"DATE\":"));
	snprintf(buf, sizeof(buf), "\"%02d.%02d.%04d\"", day(vt_now), month(vt_now), year(vt_now));
	client.println(buf);
	client.print(F(",\"TS\":"));
	client.println(now());
	client.print(F(",\"TARIFF\":"));
	int vi_hInclShift = getHourIncludingNightShift(vt_now);
	int vi_dayOfTheWeek = dayOfWeek(vt_now);
	if (isCheapTariff(vi_hInclShift, vi_dayOfTheWeek)) {
		client.println(F("\"N\""));
	} else {
		client.println(F("\"D\""));
	}
	client.print(F(",\"TARIFF_DETAIL\":"));

	if (isWeekend(vi_dayOfTheWeek)) {
		client.println(F("\"Weekend\""));
	} else if (isNightCheapTariff(vi_hInclShift)) {
		client.println(F("\"Night\""));
	} else if (isDayCheapTariff(vi_hInclShift)) {
		client.println(F("\"Cheap day\""));
	} else {
		client.println(F("\"Day\""));
	}

}

//void printErrorReport(EthernetClient& client) {
//    client.print(F(",\"ERROR\":\""));
//    ApplicationMonitor.Dump(client, true);
//    client.print(F("\""));
//
//}

void printHtmlConfig(EthernetClient& client) {
	char buf[40];
	client.print(F(",\"Strefy czas\":"));
	snprintf(buf, sizeof(buf), "\"D %02d-%02d N %02d-%02d\"", getHourDayStart(), getHourDayEnd(), getHourNightStart(), getHourNightEnd());
	client.println(buf);

	client.print(F(",\"Temperatury\":"));
	snprintf(buf, sizeof(buf), "\"D %02d-%02d N %02d-%02d\"", getTempMinDay(), getTempMaxDay(), getTempMinNight(), getTempMaxNight());
	client.println(buf);
	snprintf(buf, sizeof(buf), ",\"PompMiesz\":\"%02d\"", getTempMixingStart());
	client.println(buf);

	client.print(F(",\"Wentylatory\":"));
	snprintf(buf, sizeof(buf), "\"N %04d U %04d S:%03d\"", getDesiredWentRPM(NEW_WENT) * 10, getDesiredWentRPM(USED_WENT) * 10, getWentStep());
	client.println(buf);
}

void printHtmlStatus(EthernetClient& client) {
	char buf[40];
	client.print(F(",\"Buf grzeje\":"));
	snprintf(buf, sizeof(buf), "\"%02d\"", isBufforHeating());
	client.println(buf);

	client.print(F(",\"Bur_wiatTo\":"));
	snprintf(buf, sizeof(buf), "\"%02d\"", getTempWaitTo());
	client.println(buf);

	client.print(F(",\"Bur_heatTo\":"));
	snprintf(buf, sizeof(buf), "\"%02d\"", getTempHeatTo());
	client.println(buf);

	client.print(F(",\"pompaMieszPracuje\":"));
	snprintf(buf, sizeof(buf), "\"%02d\"", isMixingPumpWorking());
	client.println(buf);

	client.print(F(",\"pompaPodlPracuje\":"));
	snprintf(buf, sizeof(buf), "\"%02d\"", isFloorPumpWorking());
	client.println(buf);

	client.print(F(",\"wietrzenie\":"));
	snprintf(buf, sizeof(buf), "\"%02d\"", isAiring());
	client.println(buf);
}

void printHtmlInput(EthernetClient& client, char* input) {
	client.print(F(",\"input\":\""));
	client.print(input);
	client.println(F("\""));
}

//void printHtmlLogFileList(EthernetClient& client) {
//	File root;
//	root = SD.open("/");
//	client.println(F("\"LogFiles\":["));
//	printDirectory(client, root, 0);
//	client.println(F("]"));
//}

void printDirectory(EthernetClient& client, File dir, int numTabs) {
	while (true) {

		File entry = dir.openNextFile();
		if (!entry) {
			// no more files
			break;
		}
//for (uint8_t i = 0; i < numTabs; i++) {
//client.print('\t');
//}
		client.print("{\"name\":\"");
		client.print(entry.name());
		if (entry.isDirectory()) {
			client.println("/");
			printDirectory(client, entry, numTabs + 1);
		} else {
			// files have sizes, directories do not
			client.print("\"size\":");
			client.print(entry.size(), DEC);
			client.print(",");
		}
		client.print("},");
		entry.close();
	}
}

void printHTTPHeader(EthernetClient& client) {
	// send a standard http response header
	client.println(F("HTTP/1.1 200 OK"));
	client.println(F("Content-Type: application/json"));
	client.println(F("Connection: close")); // the connection will be closed after completion of the response
	//client.println("Refresh: 5"); // refresh the page automatically every 5 sec
	client.println();
//client.println("<!DOCTYPE HTML>");
//client.println("<html>");
	client.println("{");
}

void printHTTPHeaderLOG(EthernetClient& client) {
	// send a standard http response header
	client.println(F("HTTP/1.1 200 OK"));
	client.println(F("Content-Type: text/csv"));
	client.println(F("Connection: close")); // the connection will be closed after completion of the response
	client.println();
}

void printRestStatus(EthernetClient& client, Request *a) {
	client.print(F(",\"IS_ERROR\":\""));
	client.print(a->isError());
	client.print(F("\",\"ERROR\":\""));
	client.print(a->getError());
	client.print(F("\",\"RESOURCE\":\""));
	client.print(a->getResource());
	client.print(F("\",\"VALUE\":\""));
	client.print(a->getValue());
	client.print(F("\",\"METHOD\":\""));
	client.print(a->getMethod());
	client.print(F("\""));
}

void printRestStatus(EthernetClient& client, char* input, Request *a) {
	// send a standard http response header
	printHTTPHeader(client);
	// output the value of each analog input pin
	printHtmlBufor(client);
	printHtmlWent(client);
	printHtmlConfig(client);
	printTimeStamp(client);
	printHtmlStatus(client);
	printHtmlInput(client, input);
	//printErrorReport(client);
	//printHtmlLogFileList(client);
	printRestStatus(client, a);
	client.println("}");
}

void printLogFile(EthernetClient& client, char* fileName) {
	printHTTPHeaderLOG(client);
	myFile = SD.open(fileName);

	if (myFile) {
		// read from the file until there's nothing else in it:
		while (myFile.available()) {
			client.print(myFile.read());
		}
		// close the file:
		myFile.close();
	} else {
		// if the file didn't open, print an error:
		client.print("error opening");
	}
}

void parseInpt(String input) {
}

//TODO przetestowac czy dziala pobieranie pliku z logami
//TODO dopisac pobranie listy plikow z logami?
void loopServer() {
	// listen for incoming clients
	EthernetClient client = server.available();
	if (client) {
		char readString[100];
		int i = 0;
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				if (i < 100) {
					readString[i++] = c;
				}

				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if (c == '\n' && currentLineIsBlank) {				// send a standard http response header
					//parseInput(readString);
					Request *a = new Request(readString);
					printRestStatus(client, readString, a);
					//client.println(a->getResponse());
					delete a;
					break;
				}
				if (c == '\n') {				// you're starting a new line
					currentLineIsBlank = true;
				} else if (c != '\r') {	// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}
		// give the web browser time to receive the data
		delay(1);
		// close the connection:
		client.stop();
		//Serial.println("client disconnected");
	}
}

// give the web browser time to receive the data
//from webServerExample
//REST API

