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
	kpd.addEventListener(keypadEvent); //add an event listener for this keypad
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

	if (vi_counter % 17 == 0) {
		sensors.requestTemperatures();  // Send the command to get temperatures
		checkAndChangeTariff();
		checkAndChangeBuffor();
	}
	if (vi_counter % 21 == 0) {
		String went = getWentString(); // Send request to get went's temps and rpms
		parseRekuperatorMSG(went);
	}
	if (vi_counter % 41 == 0) { // pat watch dog - sending 8 thrash chars just to say I am alive
		if (hour() == 21 && minute() == 59) { //auto reset at 21:59
			delay(60000); //60s sleep
		}
		patDog();
	}
	if (vi_counter % 51 == 0) { // log to file
		logCurrentStatusToFile();
	}
	if (vi_counter % 61 == 0) { // chek wietrzenie
		checkWietrzenie();
	}

//if (vi_counter < 5) {
	switch (vi_currentScreen) {
	case SCREEN_MAIN:
		printTemps(vi_counter);
		printWent();
		break;
	case SCREEN_CONFIG_GODZ:
		void printConfigGodz(uint8_t pi_Hour_Day_Start, uint8_t pi_Hour_Day_End, uint8_t pi_Hour_Night_Start, uint8_t pi_Hour_Night_End, uint8_t pi_Minute_Night_Shift);

		break;
	case SCREEN_CONFIG_GODZ2:
		printConfigGodz2();
		break;

	case SCREEN_CONFIG_TEMP:
		printConfigTemp();
		break;

	case SCREEN_CONFIG_WENT:
		printConfigWent();
		break;
	case SCREEN_CONFIG_FILE:
		printConfigFiles();
	}

	printDigitalClock();
	vi_counter++;
	delay(50);
	if (vi_counter == 1000)
		vi_counter = 0;
	loopServer();
}

void patDog() {
	char buf[10];
	Wire.beginTransmission(DOG_ID); // transmit to device #5
	snprintf(buf, sizeof(buf), "%04d%04dx", getDesiredWentRPM(NEW_WENT) * 10, getDesiredWentRPM(NEW_WENT) * 10); //wyslij smieci
	Wire.write(buf);        // sends 8
	Wire.endTransmission();    // stop transmitting
}
void printConfigGodz2() {
	char buf[60];
	GLCD.DrawString(F("Czas"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "H: 1,2 M: 3,4 S: 5,6\n y: 7,8 m:9,0 d: A,B\n C - set clock\n%02d.%02d.%04d", day(), month(), year());
	GLCD.DrawString(buf, 0, 17); //, eraseFULL_LINE);
}

void printConfigTemp() {
	char buf[45];
	GLCD.DrawString(F("Temperatury Klawisze"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "D.%02d-%02d 1-2+:3-A+\nN.%02d-%02d 4-5+:6-B+", gi_Temp_Min_Day, gi_Temp_Max_Day, gi_Temp_Min_Night, gi_Temp_Max_Night);
	GLCD.DrawString(buf, 0, 17); //, eraseFULL_LINE);
	//GLCD.DrawString(F("Zmiana klawisze\nD1-2+3-A+ N4-5+6-B+"), 0, 26);
	snprintf(buf, sizeof(buf), "PompMiesz.%02d 7-8+", gi_Temp_Mixing_Start);
	GLCD.DrawString(buf, 0, 35);	//, eraseFULL_LINE);
}

void printConfigWent() {
	char buf[40];
	GLCD.DrawString(F("Wentylatory"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "N %04d U %04d S:%03d", getDesiredWentRPM(NEW_WENT) * 10, getDesiredWentRPM(USED_WENT) * 10, getWentStep());
	GLCD.DrawString(buf, 0, 17);	//, eraseFULL_LINE);
	GLCD.DrawString(F("Zmiana klawisze\nN 1-2+ U 4-5+\nA wysyla B krok\n C sw, Dew"), 0, 26);
}

void printConfigFiles() {
	char fileName[13];
	sprintf(fileName, "%04d%02d%02d.log", year(), month(), day());
	// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.
	myFile = SD.open(fileName);

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
	myFile = SD.open(fileName);

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

//draw buffor and print temps
void printTemps(int counter) {
	if (!vb_tempsPrinted) {
		GLCD.DrawRoundRect(0, 9, 27, 40, 3, PIXEL_ON);   //bufor
		GLCD.DrawHLine(27, 25, 5, PIXEL_ON);   //linia out
		GLCD.DrawVLine(30, 25, 6, PIXEL_ON);   //linia laczaca
		GLCD.DrawHLine(27, 40, 5, PIXEL_ON);   //linia in
		GLCD.DrawCircle(35, 25, 4, PIXEL_ON);   // pompa podlogowa
		GLCD.DrawCircle(31, 35, 4, PIXEL_ON);   // pompa mieszajaca
	}

	for (int i = 0; i < TEMPCOUNT; i++) {
		float tempC = sensors.getTempC((uint8_t*) gDA_sensors[i]);
		if (i < 4) {
			GLCD.CursorToXY(2, 12 + 9 * i);
			GLCD.print(tempC, 1);
		}
		gf_currentTemps[i] = tempC;
	}
	GLCD.SelectFont(Wendy3x5);
	GLCD.CursorToXY(28, 10);   //ile idze na podlogowke
	GLCD.print(gf_currentTemps[PODLOGA_OUT], 1);
	GLCD.CursorToXY(28, 42);   //ile wraca z podlogowki
	GLCD.print(gf_currentTemps[PODLOGA_IN], 1);
	GLCD.SelectFont(System5x7);

	if (vb_pompaPodlogowaPracuje && !vb_pompaPodlogowaRysuj && counter % 5) {
		vb_pompaPodlogowaRysuj = true;
	} else if (counter % 9) {
		vb_pompaPodlogowaRysuj = false;
	}
	if (vb_pompaPodlogowaRysuj)
		GLCD.FillTriangle(34, 23, 34, 27, 38, 25, PIXEL_ON);
	else
		GLCD.FillTriangle(34, 23, 34, 27, 38, 25, PIXEL_OFF);

	if (vb_pompaMieszajacaPracuje && !vb_pompaMieszajacaRysuj) {
		vb_pompaMieszajacaRysuj = true;
	} else {
		vb_pompaMieszajacaRysuj = false;
	}
	if (vb_pompaMieszajacaRysuj)
		GLCD.DrawCircle(31, 35, 2, PIXEL_ON);
	//GLCD.FillTriangle(28, 32,  30, 36,32, 32, PIXEL_ON);
	else
		GLCD.DrawCircle(31, 35, 2, PIXEL_OFF);
	//GLCD.FillTriangle(28, 32,  30, 36,32, 32, PIXEL_ON);

	vb_tempsPrinted = true;
}

//draw went and print temps
void printWent() {
#define x0 50
#define xmid 89
#define x1 128
#define y0 16
#define ymid 32
#define y1 47
	if (!vb_wentsPrinted) {
		GLCD.DrawRoundRect(x0, y0 - 1, x1 - x0, y1 - y0 + 3, 3, PIXEL_ON); //bufor
		GLCD.DrawLine(xmid, y0, xmid - 12, ymid);
		GLCD.DrawLine(xmid, y0, xmid + 12, ymid);
		GLCD.DrawLine(xmid, y1, xmid - 12, ymid);
		GLCD.DrawLine(xmid, y1, xmid + 12, ymid);
		GLCD.DrawLine(xmid + 12, ymid, x1, ymid);
		GLCD.DrawLine(xmid - 12, ymid, x0, ymid);
	}
	GLCD.SelectFont(Wendy3x5);

	GLCD.CursorToXY(x0 + 3, y0 + 8);   //new in
	GLCD.print(getCurrentTemReku(NEW_IN), 1);
	GLCD.CursorToXY(x0 + 2, y0 + 1);
	GLCD.print(F("New->"));

	GLCD.CursorToXY(xmid + 15, y0 + 8);   //used in
	GLCD.print(getCurrentTemReku(USED_IN), 1);
	GLCD.CursorToXY(xmid + 12, y0 + 1);   //used
	GLCD.print(F("<-Used"));

	GLCD.CursorToXY(x0 + 3, ymid + 2);   //USED out
	GLCD.print(getCurrentTemReku(USED_OUT), 1);
	GLCD.CursorToXY(x0 + 3, ymid + 8);
	GLCD.print(F("<-Used"));

	GLCD.CursorToXY(xmid + 15, ymid + 2);   //new out
	GLCD.print(getCurrentTemReku(NEW_OUT), 1);
	GLCD.CursorToXY(xmid + 15, ymid + 8);
	GLCD.print(F("New->"));

	GLCD.CursorToXY(x0 + 15, y0 - 8);   //new rpms
	GLCD.print(getCurrentVentRPM(NEW_WENT));

	GLCD.CursorToXY(xmid + 15, y0 - 8);   //used rpms
	GLCD.print(getCurrentVentRPM(USED_WENT));

	GLCD.SelectFont(System5x7);
	vb_wentsPrinted = true;
}

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

