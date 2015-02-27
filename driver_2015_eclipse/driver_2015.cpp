#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <openGLCD.h>
#include <SD.h>
#include <Keypad.h>
#include <Time.h>
#include <DS3232RTC.h>  // better RTC

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>  
#include "termometry.h"

#include <EEPROM.h>
#include <avr/eeprom.h>

#include <SPI.h>
#include <Ethernet.h>

#define ONE_WIRE_BUS 40 //termometry 40 dla buf
#define GI_PIN_GRZALEK_1 42
#define GI_PIN_GRZALEK_2 43
#define GI_PIN_GRZALEK_3 44
#define GI_PIN_POMPY 45//pompa mieszajaca bufor

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

boolean vb_taryfaNocna = false;
boolean vb_pompaMieszajacaPracuje = false;
boolean vb_pompaMieszajacaRysuj = true;
boolean vb_pompaPodlogowaPracuje = true;
boolean vb_pompaPodlogowaRysuj = true;
boolean vb_buforGrzeje = false;
boolean vb_tempsPrinted = false; //optymalizacja rysowania
boolean vb_wentPrinted = false; //optymalizacja rysowania

uint8_t gi_EEmind, gi_EEmaxd, gi_EEminn, gi_EEmaxn, gi_EEtemperaturaStartuMieszania;
uint8_t gi_EEnocStart, gi_EEnocEnd, gi_EEdzienStart, gi_EEdzienEnd;

uint8_t gi_mind, gi_maxd, gi_minn, gi_maxn, gi_temperaturaStartuMieszania;
uint8_t gi_nocStart, gi_nocEnd, gi_dzienStart, gi_dzienEnd;

int vi_counter = 0; //for display
int vi_currentScreen = 0;
#define NUMBER_OF_SCREENS 5
#define SCREEN_MAIN 0
#define SCREEN_CONFIG_GODZ 1
#define SCREEN_CONFIG_TEMP 2
#define SCREEN_CONFIG_WENT 3
#define SCREEN_CONFIG_FILE 4

//keypad globals
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four
// Define the Keymap
char keys[ROWS][COLS] = { //
		{ '1', '2', '3', 'A' }, //
				{ '4', '5', '6', 'B' }, //
				{ '7', '8', '9', 'C' }, //
				{ '*', '0', '#', 'D' } };
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { A8, A9, A10, A11 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { A12, A13, A14, A15 };
//Create the Keypad
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//keypad globals

time_t gt_prevtime;
time_t gt_prevDay;
int gi_wentStep;
boolean gb_fileOpen = false;

File myFile;

boolean incoming = 0;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x11 };
IPAddress ip(192, 168, 2, 2); //<<< ENTER YOUR IP ADDRESS HERE!!!

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(8080);

void setupI2C() {
	Wire.begin();                // join i2c bus as master
}

void initConfig() {

	gi_mind = eeprom_read_byte((uint8_t *) &gi_EEmind);
	if (gi_mind == 255) {
		gi_mind = 29;
	}

	gi_maxd = eeprom_read_byte((uint8_t *) &gi_EEmaxd);
	if (gi_maxd == 255) {
		gi_maxd = 31;
	}

	gi_minn = eeprom_read_byte((uint8_t *) &gi_EEminn);
	if (gi_minn == 255) {
		gi_minn = 70;
	}

	gi_maxn = eeprom_read_byte((uint8_t *) &gi_EEmaxn);
	if (gi_maxn == 255) {
		gi_maxn = 86;
	}

	gi_temperaturaStartuMieszania = eeprom_read_byte((uint8_t *) &gi_EEtemperaturaStartuMieszania);
	if (gi_temperaturaStartuMieszania == 255) {
		gi_temperaturaStartuMieszania = 55;
	}

	gi_nocStart = eeprom_read_byte((uint8_t *) &gi_EEnocStart);
	if (gi_nocStart == 255) {
		gi_nocStart = 21;
	}

	gi_nocEnd = eeprom_read_byte((uint8_t *) &gi_EEnocEnd);
	if (gi_nocEnd == 255) {
		gi_nocEnd = 7;
	}

	gi_dzienStart = eeprom_read_byte((uint8_t *) &gi_EEdzienStart);
	if (gi_dzienStart == 255) {
		gi_dzienStart = 13;
	}

	gi_dzienEnd = eeprom_read_byte((uint8_t *) &gi_EEdzienEnd);
	if (gi_dzienEnd == 255) {
		gi_dzienEnd = 15;
	}

}

void setup(void) {
	int line = 0;
	initConfig();
	line = setupGLCD(line);
	setupDS();
	line = setupRTC(line);
	setupI2C();
	setupBuf();
	setupPompa();
	line = setupSD(line);
	setupWent();
	delay(2000);
	setupHttp();
	GLCD.ClearScreen(PIXEL_OFF);
}

int setupWent() {
	gi_desiredWentRPM[NEW_WENT] = 1110;
	gi_desiredWentRPM[USED_WENT] = 1010;
	gi_wentStep = WENT_STEP2;
}
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

void setupPompa() {
	pinMode(GI_PIN_POMPY, OUTPUT);
}

void setupBuf() {
	pinMode(GI_PIN_GRZALEK_1, OUTPUT);
	pinMode(GI_PIN_GRZALEK_2, OUTPUT);
	pinMode(GI_PIN_GRZALEK_3, OUTPUT);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
#ifdef REKUPERATOR
void requestEvent() {
	int newRpm = 2700;
	int usedRpm = 2700;
	char buf[21];
	snprintf(buf, sizeof(buf), "%03d%03d%03d%03d%04d%4d", (int) (gf_currentTemps[NEW_IN] * 10), (int) (gf_currentTemps[NEW_OUT] * 10), (int) (gf_currentTemps[USED_IN] * 10),
			(int) (gf_currentTemps[USED_OUT] * 10), gi_currentWentRPM[NEW_WENT], gi_currentWentRPM[USED_WENT]);
	Wire.write(buf); // send temperatures in order 0,1,2,3 for 24.5 245 will be sent and then rpm-new rpm-used 21chars
//GLCD.DrawString(buf,  gTextfmt_center, gTextfmt_bottom);

//max obroty to 2700 na min
}

#endif

//new_in new_out used_in used_out new_went used_went
void parseRekuperatorMSG(String msg) {
	String t0 = msg.substring(0, 3);
	String t1 = msg.substring(3, 6);
	String t2 = msg.substring(6, 9);
	String t3 = msg.substring(9, 12);
	String rpm1 = msg.substring(12, 16);
	String rpm2 = msg.substring(16, 20);
	gf_currentTemps_reku[NEW_IN] = t0.toFloat() / 10.0;
	gf_currentTemps_reku[NEW_OUT] = t1.toFloat() / 10.0;
	gf_currentTemps_reku[USED_IN] = t2.toFloat() / 10.0;
	gf_currentTemps_reku[USED_OUT] = t3.toFloat() / 10.0;
	gi_currentWentRPM[NEW_WENT] = rpm1.toInt();
	gi_currentWentRPM[USED_WENT] = rpm2.toInt();
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

//from Dallas Temp example Simple
void setupDS() {
	sensors.begin();   // Start up the library
}

//from openGLCD example Hello world
int setupGLCD(int line) {
	GLCD.Init();   // Initialize the GLCD

// Select the font for the default text area
	GLCD.SelectFont(System5x7);
	GLCD.print(F("System5x7"));   // keep string in flash on AVR boards with IDE 1.x
	return line + 1;
}

void configTemp(char key) {
	switch (key) {
	case '1':
		eeprom_write_byte(&gi_EEmind, --gi_mind);   //decMinDzienna
		break;
	case '2':
		eeprom_write_byte(&gi_EEmind, ++gi_mind);   //decMinDzienna
		break;
	case '3':
		eeprom_write_byte(&gi_EEmaxd, --gi_maxd);   //decMaxDzienna
		break;
	case 'A':
		eeprom_write_byte(&gi_EEmaxd, ++gi_maxd);   //incMaxDzienna
		break;
	case '4':
		eeprom_write_byte(&gi_EEminn, --gi_minn);   //decMinNocna
		break;
	case '5':
		eeprom_write_byte(&gi_EEminn, ++gi_minn);   //incMinNocna
		break;
	case '6':
		eeprom_write_byte(&gi_EEmaxn, --gi_maxn);   //decMaxNocna
		break;
	case 'B':
		eeprom_write_byte(&gi_EEmaxn, ++gi_maxn);   //incMaxNocna
		break;
	case '7':
		eeprom_write_byte(&gi_EEtemperaturaStartuMieszania, --gi_temperaturaStartuMieszania);   //dec PompaMiesz
		break;
	case '8':
		eeprom_write_byte(&gi_EEtemperaturaStartuMieszania, ++gi_temperaturaStartuMieszania);   //inc PompaMiesz
		break;

	}
}

void configWent(char key) {
	switch (key) {
	case '1':
		decWentNew();
		break;
	case '2':
		incWentNew();
		break;
	case '4':
		decWentUSED();
		break;
	case '5':
		incWentUSED();
		break;
	case 'A':
		setWents();
		break;
	case 'B':
		changeWentStep();
		break;
	}
}

void configGodz(char key) {
	switch (key) {
	case '1':
		eeprom_write_byte(&gi_EEdzienStart, --gi_dzienStart);   //dec dzien start
		break;
	case '2':
		eeprom_write_byte(&gi_EEdzienStart, ++gi_dzienStart);   //inc dzien start
		break;
	case '3':
		eeprom_write_byte(&gi_EEdzienEnd, --gi_dzienEnd);   //dec dzien end
		break;
	case 'A':
		eeprom_write_byte(&gi_EEdzienEnd, ++gi_dzienEnd);   //dec dzien end
		break;
	case '4':
		eeprom_write_byte(&gi_EEnocStart, --gi_nocStart);   //dec noc start
		break;
	case '5':
		eeprom_write_byte(&gi_EEnocStart, ++gi_nocStart);   //inc noc start
		break;
	case '6':
		eeprom_write_byte(&gi_EEnocEnd, --gi_nocEnd);   //dec noc end
		break;
	case 'B':
		eeprom_write_byte(&gi_EEnocEnd, ++gi_nocEnd);   //dec noc end
		break;

	}
}

void loop(void) {
	char key = kpd.getKey();
	if (key) {  // Check for a valid key.
		switch (key) {
		case '#':
			nextScreen();
			break;
		case '*':
			prevScreen();
			break;
		default:
			switch (vi_currentScreen) {
			case SCREEN_CONFIG_WENT:
				configWent(key);
				break;
			case SCREEN_CONFIG_TEMP:
				configTemp(key);
				break;
			case SCREEN_CONFIG_GODZ:
				configGodz(key);
				break;
			}
			break;
		}
	}

//if (vi_counter % 3 == 0) {
	if (vi_counter % 17 == 0) {
		sensors.requestTemperatures();  // Send the command to get temperatures
		checkAndChangeTariff();
		checkAndChangeBuffor();
	}
	if (vi_counter % 21 == 0) {
		String went = getWentString();
		parseRekuperatorMSG(went);
	}
	if (vi_counter % 51 == 0) {
		log();
	}

//}

//	if (vi_counter < 5) {
	switch (vi_currentScreen) {
	case SCREEN_MAIN:
		printTemps();
		printWent();
		break;
	case SCREEN_CONFIG_GODZ:
		printConfigGodz();
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

void changeWentStep() {
	if (gi_wentStep == WENT_STEP1)
		gi_wentStep = WENT_STEP2;
	else if (gi_wentStep == WENT_STEP2)
		gi_wentStep = WENT_STEP3;
	else if (gi_wentStep == WENT_STEP3)
		gi_wentStep = WENT_STEP1;
}
void incWentNew() {
	incWent(NEW_WENT, NEW_WENT_MAX);
}

void incWentUSED() {
	incWent(USED_WENT, USED_WENT_MAX);
}

void decWentNew() {
	decWent(NEW_WENT, NEW_WENT_MAX);
}

void decWentUSED() {
	decWent(USED_WENT, USED_WENT_MAX);
}

void incWent(int went, int max) {
	int old = gi_desiredWentRPM[went];
	gi_desiredWentRPM[went] = old + gi_wentStep;
	if (gi_desiredWentRPM[went] > max)
		gi_desiredWentRPM[went] = max;
}

void decWent(int went, int max) {
	int old = gi_desiredWentRPM[went];
	if (old > gi_wentStep)
		gi_desiredWentRPM[went] = old - gi_wentStep;
}

void setWents() {
	char buf[10];

	Wire.beginTransmission(REKUPERATYOR_ID); // transmit to device #4
	snprintf(buf, sizeof(buf), "%04d%04dx", gi_desiredWentRPM[NEW_WENT], gi_desiredWentRPM[USED_WENT]);
	Wire.write(buf);        // sends 8
	Wire.endTransmission();    // stop transmitting

	GLCD.DrawString(F("sent"), gTextfmt_left, gTextfmt_bottom, eraseTO_EOL);
	GLCD.DrawString(buf, 60, gTextfmt_bottom, eraseTO_EOL);
}

void nextScreen() {
	clearScreenWithoutTime();
	vb_tempsPrinted = false;
	vb_wentPrinted = false;
	vi_currentScreen++;
	if (vi_currentScreen >= NUMBER_OF_SCREENS)
		vi_currentScreen = 0;
}

void prevScreen() {
	clearScreenWithoutTime();
	vb_tempsPrinted = false;
	vb_wentPrinted = false;
	vi_currentScreen--;
	if (vi_currentScreen < 0)
		vi_currentScreen = NUMBER_OF_SCREENS - 1;
}

void clearScreenWithoutTime() {
	for (uint8_t row = 8; row < GLCD.Height; row += 8) {
		GLCD.GotoXY(0, row);
		for (uint8_t col = 0; col < GLCD.Width; col++) {
			GLCD.WriteData(PIXEL_OFF);
		}
	}
}

String getWentString() {
	Wire.requestFrom(REKUPERATYOR_ID, 20);    // request 6 bytes from slave device #4
	char buf[21];
	int i = 0;
	while (Wire.available())    // slave may send less than requested
	{
		char c = Wire.read(); // receive a byte as character
		buf[i++] = c;
	}
	return buf;
	//return "12345678932126002700";
}

void checkAndChangeBuffor() {
	float temp = gf_currentTemps[1]; //temperatrua z ciut powyzej polowy zbiornika
	int grzejDo = 0;
	int czekajDo = 0;
	if (vb_taryfaNocna) { //noc lub południe
		grzejDo = gi_maxn;
		czekajDo = gi_minn;
		if (vb_buforGrzeje) {
			if (temp >= gi_maxn) {
				wylaczGrzalki();
				vb_buforGrzeje = false;
			}
		} else { //nie grzeje
			if (temp < gi_minn) {
				vb_buforGrzeje = true;
				wlaczGrzalki();
			}
		}
	} else { //dzień
		grzejDo = gi_maxd;
		czekajDo = gi_mind;
		if (vb_buforGrzeje) {
			if (temp >= gi_maxd) {
				wylaczGrzalki();
				vb_buforGrzeje = false;
			}
		} else { //nie grzeje
			if (temp < gi_mind) {
				vb_buforGrzeje = true;
				wlaczGrzalki();
			}
		}
	}
	printGrzalkiStatus(grzejDo, czekajDo);
	obslugaPompyMieszajacej(temp);
}

void obslugaPompyMieszajacej(float temp) {
	if (!vb_taryfaNocna) {
		wylaczPompaBuf(); //w dzien nie mieszamy
	} else {
		if (vb_buforGrzeje && (temp > gi_temperaturaStartuMieszania)) {
			wlaczPompaBuf(); //grzeje i zagrzal juz CWU do gi_temperaturaStartuMieszania wiec grzej dol
		} else {
			wylaczPompaBuf(); // albo nie grzeje albo sie CWU wychlodzilo to nie mieszamy dalej
		}
	}
}

void printGrzalkiStatus(int grzejDo, int czekajDo) {
	char buf[40];
	if (vb_buforGrzeje) {
		sprintf(buf, "Buf Grzeje do:%02d", grzejDo);
		printBottomStatus(buf);
	} else {
		sprintf(buf, "Buf czeka do:%02d", czekajDo);
		printBottomStatus(buf);
	}
}

void printBottomStatus(char* s) {
	GLCD.DrawString(s, gTextfmt_left, gTextfmt_bottom, eraseFULL_LINE);
}

void wlaczPompaBuf() {
	digitalWrite(GI_PIN_POMPY, LOW);
	vb_pompaMieszajacaPracuje = true;
}

void wylaczPompaBuf() {
	digitalWrite(GI_PIN_POMPY, HIGH);
	vb_pompaMieszajacaPracuje = false;
}

void wylaczGrzalki() {
	ustawPinyGrzalek(HIGH);
}

void wlaczGrzalki() {
	ustawPinyGrzalek(LOW);
}

void ustawPinyGrzalek(uint8_t stan) {
	digitalWrite(GI_PIN_GRZALEK_1, stan);
	digitalWrite(GI_PIN_GRZALEK_2, stan);
	digitalWrite(GI_PIN_GRZALEK_3, stan);
}

// od 21 do 7 i od 13 do 15 => taryfa nocna
void checkAndChangeTariff() {
	int h = hour();
	vb_taryfaNocna = (h >= gi_nocStart) //21 do 23
	|| (h < gi_nocEnd) //od 00 do 6:59
			|| (h >= gi_dzienStart && h < gi_dzienEnd); // od 13 do 14:59
}
void print0(int line, char* str) {
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
	GLCD.CursorTo(0, line);
// print the number of seconds since reset:
	GLCD.print(str);
}

void print0(int line, float f) {
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
	GLCD.CursorTo(0, line);
// print the number of seconds since reset:
	GLCD.print(f);
}

void printConfigGodz() {
	char buf[40];
	GLCD.DrawString(F("Strefy czas"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "D %02d-%02d N %02d-%02d", gi_dzienStart, gi_dzienEnd, gi_nocStart, gi_nocEnd);
	GLCD.DrawString(buf, 0, 18, eraseFULL_LINE);
	GLCD.DrawString(F("Zmiana klawisze\nD1-2+3-A+ N4-5+6-B+"), 0, 27);
}

void printConfigTemp() {
	char buf[40];
	GLCD.DrawString(F("Temperatury"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "D %02d-%02d N %02d-%02d", gi_mind, gi_maxd, gi_minn, gi_maxn);
	GLCD.DrawString(buf, 0, 18, eraseFULL_LINE);
	GLCD.DrawString(F("Zmiana klawisze\nD1-2+3-A+ N4-5+6-B+"), 0, 27);
	snprintf(buf, sizeof(buf), "PompMiesz %02d 7-8+", gi_temperaturaStartuMieszania);
	GLCD.DrawString(buf, 0, 43, eraseFULL_LINE);
}

void printConfigWent() {
	char buf[40];
	GLCD.DrawString(F("Wentylatory"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "N %04d U %04d S:%03d", gi_desiredWentRPM[NEW_WENT], gi_desiredWentRPM[USED_WENT], gi_wentStep);
	GLCD.DrawString(buf, 0, 18, eraseFULL_LINE);
	GLCD.DrawString(F("Zmiana klawisze\nN 1-2+ U 4-5+\nA wysyla B krok"), 0, 27);
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

void printDirectory(File dir, int numTabs, int line) {
//	gText mTA = gText(0, 9, GLCD.Width - 1, GLCD.Height - 8); // create a text area covering the center 32 pixels of the display
	GLCD.CursorToXY(0, line);
	GLCD.SelectFont(Wendy3x5);
	while (true) {
		File entry = dir.openNextFile();
		if (!entry) {
			GLCD.println("no more files");
			break;
		}
		for (uint8_t i = 0; i < numTabs; i++) {
			GLCD.print('_');
		}
		GLCD.print(entry.name());
		if (entry.isDirectory()) {
			GLCD.println("/");
			printDirectory(entry, numTabs + 1, line + 6);
		} else {
			// files have sizes, directories do not
			GLCD.print("_");
			GLCD.println(entry.size(), DEC);
		}
		delay(30);
		entry.close();
	}

}

//from openGLCD digital clock example
void printDigitalClock() {
	char buf[11];
	GLCD.DrawHLine(0, 6, DISPLAY_WIDTH, PIXEL_ON);
	//GLCD.DrawHLine(0, 7, DISPLAY_WIDTH, PIXEL_OFF);
	GLCD.SelectFont(Wendy3x5);
	if (gt_prevtime != now()) { // if 1 second has gone by, update display
		gt_prevtime = now();   // save the last update time

		// format the time in a buffer
		snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hour(), minute(), second());

		GLCD.DrawString(buf, gTextfmt_right, gTextfmt_top, eraseTO_EOL);

		if (gt_prevDay != day()) {   //next day
			gt_prevDay = day();
			snprintf(buf, sizeof(buf), "%02d.%02d.%04d", day(), month(), year());
			// draw the formatted string centered on the display
			GLCD.DrawString(buf, gTextfmt_left, gTextfmt_top);
		}
		if (vb_taryfaNocna)
			GLCD.DrawString(F(" NOC "), gTextfmt_center, gTextfmt_top);
		else
			GLCD.DrawString(F("DZIEN"), gTextfmt_center, gTextfmt_top);
	}
	GLCD.SelectFont(System5x7);
}

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
void printTemps() {
	if (!vb_tempsPrinted) {
		GLCD.DrawRoundRect(0, 9, 27, 40, 3, PIXEL_ON);   //bufor
		GLCD.DrawHLine(27, 25, 5, PIXEL_ON);   //linia out
		GLCD.DrawVLine(30, 25, 6, PIXEL_ON);   //linia laczaca
		GLCD.DrawHLine(27, 40, 5, PIXEL_ON);   //linia in
		GLCD.DrawCircle(35, 25, 4, PIXEL_ON);   // pompa podlogowa
		GLCD.DrawCircle(31, 35, 4, PIXEL_ON);   // pompa mieszajaca
	}
	char buf[6];
	for (int i = 0; i < TEMPCOUNT; i++) {
		float tempC = sensors.getTempC(gDA_sensors[i]);
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

	if (vb_pompaPodlogowaPracuje && !vb_pompaPodlogowaRysuj) {
		vb_pompaPodlogowaRysuj = true;
	} else {
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
#define y0 17
#define ymid 32
#define y1 47
	if (!vb_wentPrinted) {
		GLCD.DrawRoundRect(x0, y0 - 1, x1 - x0, y1 - y0 + 3, 3, PIXEL_ON);   //bufor
		GLCD.DrawLine(xmid, y0, xmid - 12, ymid);
		GLCD.DrawLine(xmid, y0, xmid + 12, ymid);
		GLCD.DrawLine(xmid, y1, xmid - 12, ymid);
		GLCD.DrawLine(xmid, y1, xmid + 12, ymid);
		GLCD.DrawLine(xmid + 12, ymid, x1, ymid);
		GLCD.DrawLine(xmid - 12, ymid, x0, ymid);
	}
	GLCD.SelectFont(Wendy3x5);

	GLCD.CursorToXY(x0 + 3, y0 + 8);   //new in
	GLCD.print(gf_currentTemps_reku[NEW_IN], 1);
	GLCD.CursorToXY(x0 + 2, y0 + 1);
	GLCD.print(F("New->"));

	GLCD.CursorToXY(xmid + 15, y0 + 8);   //used in
	GLCD.print(gf_currentTemps_reku[USED_IN], 1);
	GLCD.CursorToXY(xmid + 12, y0 + 1);   //used
	GLCD.print(F("<-Used"));

	GLCD.CursorToXY(x0 + 3, ymid + 2);   //USED out
	GLCD.print(gf_currentTemps_reku[USED_OUT], 1);
	GLCD.CursorToXY(x0 + 3, ymid + 8);
	GLCD.print(F("<-Used"));

	GLCD.CursorToXY(xmid + 15, ymid + 2);   //new out
	GLCD.print(gf_currentTemps_reku[NEW_OUT], 1);
	GLCD.CursorToXY(xmid + 15, ymid + 8);
	GLCD.print(F("New->"));

	GLCD.CursorToXY(x0 + 15, y0 - 8);   //new rpms
	GLCD.print(gi_currentWentRPM[NEW_WENT]);

	GLCD.CursorToXY(xmid + 15, y0 - 8);   //used rpms
	GLCD.print(gi_currentWentRPM[USED_WENT]);

	GLCD.SelectFont(System5x7);
	vb_wentPrinted = true;
}

void printAddress(DeviceAddress deviceAddress) {

	for (uint8_t i = 0; i < 8; i++) {
		if (deviceAddress[i] < 16)
			GLCD.print("0");
		GLCD.print(deviceAddress[i], HEX);
	}
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
void log() {
	char buff[100];
	if (!gb_fileOpen) {
		gb_fileOpen = true;
		char fileName[13];
		char buf[200];
		sprintf(fileName, "%04d%02d%02d.log", year(), month(), day());
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.

		myFile = SD.open(fileName, FILE_WRITE);

// if the file opened okay, write to it:
		if (myFile) {
			sprintf(buf, "opned:%s", fileName);
			printBottomStatus(buf);
			//delay(1000);

			if (myFile.size() == 0) {
				myFile.println("hour,minute,second,t0,t1,t2,t3,t4-podlOut,t5-podlIn,buforGrzeje,pompaMiesza,pompaPodlogowa,tNI_r,tNO_r,tUI_r,tUO_r,RPM_NEW,RPM_USED");
			}
			char t0[6], t1[6], t2[6], t3[6], t4[6], t5[6], tNI_r[6], tNO_r[6], tUI_r[6], tUO_r[6];
			dtostrf(gf_currentTemps[0], 3, 2, t0);
			dtostrf(gf_currentTemps[1], 3, 2, t1);
			dtostrf(gf_currentTemps[2], 3, 2, t2);
			dtostrf(gf_currentTemps[3], 3, 2, t3);
			dtostrf(gf_currentTemps[PODLOGA_OUT], 3, 2, t4);
			dtostrf(gf_currentTemps[PODLOGA_IN], 3, 2, t5);
			dtostrf(gf_currentTemps_reku[NEW_IN], 3, 2, tNI_r);
			dtostrf(gf_currentTemps_reku[NEW_OUT], 3, 2, tNO_r);
			dtostrf(gf_currentTemps_reku[USED_IN], 3, 2, tUI_r);
			dtostrf(gf_currentTemps_reku[USED_OUT], 3, 2, tUO_r);
			sprintf(buf, "%02d:%02d:%02d,%s,%s,%s,%s,%s,%s,%01d,%01d,%s,%s,%s,%s,%04d,%04d", hour(), minute(), second(),   //
					t0, t1, t2, t3, t4, t5, vb_buforGrzeje ? "1" : "0",   //
					vb_pompaMieszajacaPracuje ? "1" : "0",   //
					vb_pompaPodlogowaPracuje ? "1" : "0",   //
					tNI_r, tNO_r, tUI_r, tUO_r,   //
					gi_currentWentRPM[NEW_WENT], gi_currentWentRPM[USED_WENT]);

			myFile.println(buf);
			// close the file:
			myFile.flush();
			myFile.close();
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

//void printLog() {
//// re-open the file for reading:
//	myFile = SD.open("test.txt");
//	if (myFile) {
//		Serial.println("test.txt:");
//
//		// read from the file until there's nothing else in it:
//		while (myFile.available()) {
//			Serial.write(myFile.read());
//		}
//		// close the file:
//		myFile.close();
//	} else {
//		// if the file didn't open, print an error:
//		Serial.println("error opening test.txt");
//	}
//
//}
//sd

//
//
//
//
//
//

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

void printHtmlBufor(EthernetClient client) {
	for (int i = 0; i < TEMPCOUNT; i++) {
		float tempC = sensors.getTempC(gDA_sensors[i]);
		if (i < 4) {
			client.print(",\"BUFOR_");
			client.print(i);
			client.print("\":");
			client.println(tempC);
		}
		gf_currentTemps[i] = tempC;
	}
	client.print(",\"PODLOGA_OUT\":");
	client.println(gf_currentTemps[PODLOGA_OUT]);
	client.print(",\"PODLOGA_IN\":");
	client.println(gf_currentTemps[PODLOGA_IN]);
}

void printHtmlWent(EthernetClient client) {
	client.print(",\"NEW_IN\":");
	client.println(gf_currentTemps_reku[NEW_IN]);
	client.print(",\"USED_IN\":");
	client.println(gf_currentTemps_reku[USED_IN]);
	client.print(",\"USED_OUT\":");
	client.println(gf_currentTemps_reku[USED_OUT]);
	client.print(",\"NEW_OUT\":");
	client.println(gf_currentTemps_reku[NEW_OUT]);
	client.print(",\"NEW_WENT\":");
	client.println(gi_currentWentRPM[NEW_WENT]);
	client.print(",\"USED_WENT\":");
	client.println(gi_currentWentRPM[USED_WENT]);
}

void printHtmlConfig(EthernetClient client) {
	char buf[40];
	client.println(F("\"Strefy czas\":"));
	snprintf(buf, sizeof(buf), "\"D %02d-%02d N %02d-%02d\",", gi_dzienStart, gi_dzienEnd, gi_nocStart, gi_nocEnd);
	client.println(buf);

	client.println(F("\"Temperatury\":"));
	snprintf(buf, sizeof(buf), "\"D %02d-%02d N %02d-%02d\",", gi_mind, gi_maxd, gi_minn, gi_maxn);
	client.println(buf);
	snprintf(buf, sizeof(buf), "\"PompMiesz\":\"%02d\",", gi_temperaturaStartuMieszania);
	client.println(buf);

	client.println(F("\"Wentylatory\":"));
	snprintf(buf, sizeof(buf), "\"N %04d U %04d S:%03d\",", gi_desiredWentRPM[NEW_WENT], gi_desiredWentRPM[USED_WENT], gi_wentStep);
	client.println(buf);
}

void printHtmlLogFileList(EthernetClient client) {
	File root;
	root = SD.open("/");
	client.println(F("\"LogFiles\":["));
	printDirectory(client, root, 0);
	client.println(F("]"));
}

void printDirectory(EthernetClient client, File dir, int numTabs) {
	while (true) {

		File entry = dir.openNextFile();
		if (!entry) {
			// no more files
			break;
		}
//		for (uint8_t i = 0; i < numTabs; i++) {
//			client.print('\t');
//		}
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
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: application/json");
	client.println("Connection: close"); // the connection will be closed after completion of the response
	//client.println("Refresh: 5"); // refresh the page automatically every 5 sec
	client.println();
//	client.println("<!DOCTYPE HTML>");
//	client.println("<html>");
	client.println("{");
}

void printHTTPHeaderLOG(EthernetClient& client) {
	// send a standard http response header
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/csv");
	client.println("Connection: close"); // the connection will be closed after completion of the response
	client.println();
}

void printRestStatus(EthernetClient& client) {
	// send a standard http response header
	printHTTPHeader(client);
	// output the value of each analog input pin
	printHtmlBufor(client);
	printHtmlWent(client);
	printHtmlConfig(client);
	printHtmlLogFileList(client);
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

//TODO przetestowac czy dziala pobieranie pliku z logami
//TODO dopisac pobranie listy plikow z logami?
void loopServer() {
	// listen for incoming clients
	EthernetClient client = server.available();
	if (client) {
		//Serial.println("new client");
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		boolean reading = false;
		char readString[14]; //string for fetching data from address
		int i = 0;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				//Serial.write(c);
				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if (c == '\n' && currentLineIsBlank) {	// send a standard http response header
					printRestStatus(client);
					break;
				}
				if (c == '\n') {	// you're starting a new line
					currentLineIsBlank = true;
				} else if (c != '\r') {	// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
				//get log file
				if (reading && c == ' ') {
					reading = false;
				}
				if (reading) {
					readString[i++] = c;
				}
				if (c == '?')
					reading = true;
				if (!reading && i > 0) {
					printLogFile(client, readString);
				}
				//get log file

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

