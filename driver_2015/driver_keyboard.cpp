#include "driver_keyboard.h"

//add your function definitions for the project driver_keyboard here
//keypad globals

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
	case 'C':
		startWietrzenie();
		break;
	case 'D':
		stopWietrzenie();
		break;
	}
}

void configTemp(char key) {
	switch (key) {
	case '1':
		decMinDzienna();
		break;
	case '2':
		incMinDzienna();

		break;
	case '3':
		decMaxDzienna();

		break;
	case 'A':
		incMaxDzienna();

		break;
	case '4':
		decMinNocna();

		break;
	case '5':
		incMinNocna();

		break;
	case '6':
		decMaxNocna();

		break;
	case 'B':
		incMaxNocna();

		break;
	case '7':
		decPompaMiesz();

		break;
	case '8':
		incPompaMiesz();

		break;

	}
}

void configGodz(char key) {
	switch (key) {
	case '1':
		decDzienStart();
		break;
	case '2':
		void incDzienStart();

		break;
	case '3':
		void decDzienEnd();
		break;
	case 'A':
		void incDzienEnd();

		break;
	case '4':
		void decNocStart();

		break;
	case '5':
		void incNocstart();

		break;
	case '6':
		void decNocEnd();
		break;
	case 'B':
		void incNocEnd();

		break;
	case '7':
		void decMinuteShift();
		break;
	case '8':
		void incMinuteShift();

		break;
	case 'C':
		//void  setTime(int hr,int min,int sec,int dy, int mnth, int yr){
		setTime(hour() + 1, minute(), second(), day(), month(), year());
		break;
	case 'D':
		//void  setTime(int hr,int min,int sec,int dy, int mnth, int yr){
		setTime(hour() - 1, minute(), second(), day(), month(), year());
		break;
	case '9':
		//void  setTime(int hr,int min,int sec,int dy, int mnth, int yr){
		setTime(hour(), minute() + 1, second(), day(), month(), year());
		break;
	case '0':
		//void  setTime(int hr,int min,int sec,int dy, int mnth, int yr){
		setTime(hour(), minute() - 1, second(), day(), month(), year());
		break;

	}
}

void configGodz2(char key) {
	switch (key) {
	case '1':
		//void  setTime(int hr,int min,int sec,int dy, int mnth, int yr){
		setTime(hour() + 1, minute(), second(), day(), month(), year());
		break;
	case '2':
		setTime(hour() - 1, minute(), second(), day(), month(), year());
		break;
	case '3':
		setTime(hour(), minute() + 1, second(), day(), month(), year());
		break;
	case '4':
		setTime(hour(), minute() - 1, second(), day(), month(), year());
		break;
	case '5':
		setTime(hour(), minute(), second() + 1, day(), month(), year());
		break;
	case '6':
		setTime(hour(), minute(), second() - 1, day(), month(), year());
		break;
	case '7':
		setTime(hour(), minute(), second(), day(), month(), year() + 1);
		break;
	case '8':
		setTime(hour(), minute(), second(), day(), month(), year() - 1);
		break;
	case '9':
		setTime(hour(), minute(), second(), day(), month() + 1, year());
		break;
	case '0':
		setTime(hour(), minute(), second(), day(), month() - 1, year());
		break;
	case 'A':
		setTime(hour(), minute(), second(), day() + 1, month(), year());
		break;
	case 'B':
		setTime(hour(), minute(), second(), day() - 1, month(), year());
		break;
	case 'C':
		RTC.set(now());
		GLCD.DrawString(F("RTC - set"), gTextfmt_left, gTextfmt_bottom, eraseTO_EOL);
		break;
	}
}

void checkKey(char key) {
	//char key = kpd.getKey();
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
			case SCREEN_CONFIG_GODZ2:
				configGodz2(key);
				break;

			}
			break;
		}
	}
}

//TODO move implementation to the event handling
//take care of some special events
void keypadEvent(KeypadEvent key) {
	switch (kpd.getState()) {
	case PRESSED:
		checkKey(key);
//		switch (key) {
//		case '#':
//			nextScreen();
//			break;
//		case '*':
//			prevScreen();
//			break;
//		}
		break;
	case RELEASED:
		switch (key) {
		case '*':
			;
			break;
		}
		break;
	case HOLD:
		switch (key) {
		case '*':
			;
			break;
		}
		break;
	case IDLE:
		break;
	}
}

