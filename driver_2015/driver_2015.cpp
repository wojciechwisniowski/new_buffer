#include "driver_2015.h"

#define DOG_ID 5   //id na szynie I2C

int vi_counter = 0; //for display

void setupI2C() {
	Wire.begin();                // join i2c bus as master
}

void initConfig() {
	initConfigTemp();
	initConfigStrefy();
	initConfigWent();
}

void setup(void) {
	int line = 0;
	line = setupGLCD(line);
	initConfig();
	setupDS();
	line = setupRTC(line);
	setupI2C();
	setupBuf();
	setupPompy();
	line = setupSD(line);
	delay(2000);
	setupHttp();
	GLCD.ClearScreen(PIXEL_OFF);
	getKpd().addEventListener(keypadEvent); //add an event listener for this keypad
}

//from Time RTC example
int setupRTC(int line) {
	setSyncProvider(RTC.get);   // the function to get the time from the RTC
	if (timeStatus() != timeSet) {
		print0(line, "RTC sync failed");
	} else {
		print0(line, "RTC sync ok");
	}
	return line + 1;
}

void loop(void) {

	getKpd().getKey();
	//checkKey();

	int h = getHourIncludingNightShift(now());
	int dayOfTheWeek = dayOfWeek(now());

	if (vi_counter % 17 == 0) {
		requestTemperatures();
		checkAndChangeBuffor(h, dayOfTheWeek, setMixingPumpHW, printBottomStatus, setHeaterHW);
	}
	if (vi_counter % 21 == 0) {
		char buf[21];
		getWentString(buf);
		parseRekuperatorMSG(buf);   // Send request to get went's temps and rpms
	}
	if (vi_counter % 41 == 0) { // pat watch dog - sending 8 thrash chars just to say I am alive
		checkDog();
	}
	if (vi_counter % 51 == 0) { // log to file
		logCurrentStatusToFile();
	}
	if (vi_counter % 61 == 0) { // chek wietrzenie
		checkWietrzenie(h, dayOfTheWeek, &setWents);
	}

	printScreen(vi_counter);

	printDigitalClock();

	vi_counter++;
	delay(50);
	if (vi_counter == 1000)
		vi_counter = 0;
	loopServer();
}

void checkDog() {
	int h = hour();
	int m = minute();
	if ((h == 21 && m == 59) || (h == 7 && m == 1)) { //auto reset at 21:59 and 7:01
		delay(60000); //60s sleep
	}
	patDog();
}

void patDog() {
	char buf[10];
	Wire.beginTransmission(DOG_ID); // transmit to device #5
	snprintf(buf, sizeof(buf), "%04d%04dx", getDesiredWentRPM(NEW_WENT) * 10, getDesiredWentRPM(NEW_WENT) * 10); //wyslij smieci
	Wire.write(buf);        // sends 8
	Wire.endTransmission();    // stop transmitting
}

void printConfigFiles() {
	char fileName[13];
	sprintf(fileName, "%04d%02d%02d.log", year(), month(), day());
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.
	File myFile = SD.open(fileName);

	if (myFile) {
		printBottomStatus("opened");
		uint32_t size = myFile.size();
		GLCD.GotoXY(0, 9);
		GLCD.print(fileName);
		GLCD.print(" ");
		GLCD.print(size);
		myFile.close();
	}
}

void printFile() {
	char fileName[13];
	sprintf(fileName, "%04d%02d%02d.log", year(), month(), day());
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.
	File myFile = SD.open(fileName);

	if (myFile) {
		printBottomStatus("opened");

		// read from the file until there's nothing else in it:
		while (myFile.available()) {
			GLCD.PutChar(myFile.read());
		}
		// close the file:
		myFile.close();
	} else {
		// if the file didn't open, print an error:
		printBottomStatus("error opening");
	}
}

//void printDirectory(File dir, int numTabs, int line) {
////gText mTA = gText(0, 9, GLCD.Width - 1, GLCD.Height - 8); // create a text area covering the center 32 pixels of the display
//	GLCD.CursorToXY(0, line);
//	GLCD.SelectFont(Wendy3x5);
//	while (true) {
//		File entry = dir.openNextFile();
//		if (!entry) {
//			GLCD.println("no more files");
//			break;
//		}
//		for (uint8_t i = 0; i < numTabs; i++) {
//			GLCD.print('_');
//		}
//		GLCD.print(entry.name());
//		if (entry.isDirectory()) {
//			GLCD.println("/");
//			printDirectory(entry, numTabs + 1, line + 6);
//		} else {
//			// files have sizes, directories do not
//			GLCD.print("_");
//			GLCD.println(entry.size(), DEC);
//		}
//		delay(30);
//		entry.close();
//	}
//
//}

tmElements_t tm;

const char *monthName[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

bool getTime(const char *str) {
	int Hour, Min, Sec;

	if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
		return false;
	tm.Hour = Hour;
	tm.Minute = Min;
	tm.Second = Sec;
	return true;
}
//from openGLCD digital clock example

void printAddress(DeviceAddress deviceAddress) {

	for (uint8_t i = 0; i < 8; i++) {
		if (deviceAddress[i] < 16)
			GLCD.print("0");
		GLCD.print(deviceAddress[i], HEX);
	}
}

//void printLog() {
//// re-open the file for reading:
//myFile = SD.open("test.txt");
//if (myFile) {
//Serial.println("test.txt:");
//
//// read from the file until there's nothing else in it:
//while (myFile.available()) {
//Serial.write(myFile.read());
//}
//// close the file:
//myFile.close();
//} else {
//// if the file didn't open, print an error:
//Serial.println("error opening test.txt");
//}
//
//}
//sd

//
//
//
//
//
//

