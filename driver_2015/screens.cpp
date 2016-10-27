/*
 * screens.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "screens.h"

int vi_currentScreen = SCREEN_MAIN;
boolean vb_tempsPrinted = false; //optymalizacja rysowania
boolean vb_wentsPrinted = false; //optymalizacja rysowania

int gt_prevDay;
time_t gt_prevtime;

void nextScreen() {
	clearScreenWithoutTime();
	vb_tempsPrinted = false;
	vb_wentsPrinted = false;
	vi_currentScreen++;
	if (vi_currentScreen >= NUMBER_OF_SCREENS)
		vi_currentScreen = 0;
}

void prevScreen() {
	clearScreenWithoutTime();
	vb_tempsPrinted = false;
	vb_wentsPrinted = false;
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

void printConfigGodz(uint8_t pi_Hour_Day_Start, uint8_t pi_Hour_Day_End, uint8_t pi_Hour_Night_Start, uint8_t pi_Hour_Night_End, uint8_t pi_Minute_Night_Shift) {
	char buf[60];
	GLCD.DrawString(F("Strefy czas"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "D%02d-%02d 1-2+:3-A+\nN%02d-%02d 4-5+:6-B+ \nnight shift%02d 7-8+\nCD h+- 90 m+-", pi_Hour_Day_Start, pi_Hour_Day_End, pi_Hour_Night_Start,
			pi_Hour_Night_End, pi_Minute_Night_Shift);
	GLCD.DrawString(buf, 0, 17); //, eraseFULL_LINE);
	//GLCD.DrawString(F("Zmiana klawisze\nD1-2+3-A+ N4-5+6-B+\ns7-8+"), 0, 26);
}

//from openGLCD example Hello world
int setupGLCD(int line) {
	GLCD.Init();   // Initialize the GLCD

// Select the font for the default text area
	GLCD.SelectFont(System5x7);
	GLCD.print(F("System5x7")); // keep string in flash on AVR boards with IDE 1.x
	return line + 1;
}

//force print digital clock
void refreshDigitalClock() {
	gt_prevtime = 0;
	gt_prevDay = 0;
}
//from openGLCD digital clock example
void printDigitalClock() {
	char buf[11];
	GLCD.DrawHLine(0, 6, DISPLAY_WIDTH, PIXEL_ON);
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
		if (isNightTariff())
			GLCD.DrawString(F(" NOC "), gTextfmt_center, gTextfmt_top);
		else
			GLCD.DrawString(F("DZIEN"), gTextfmt_center, gTextfmt_top);
	}
	GLCD.SelectFont(System5x7);
}

