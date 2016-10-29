/*
 * screens.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "screens.h"

const int NUMBER_OF_SCREENS = 6;
const int SCREEN_MAIN = 0;
const int SCREEN_CONFIG_GODZ = 1;
const int SCREEN_CONFIG_TEMP = 2;
const int SCREEN_CONFIG_WENT = 3;
const int SCREEN_CONFIG_FILE = 4;
const int SCREEN_CONFIG_GODZ2 = 5;

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
	time_t vt_now = now();
	if (gt_prevtime != vt_now) { // if 1 second has gone by, update display
		gt_prevtime = vt_now;   // save the last update time

		// format the time in a buffer
		snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hour(vt_now), minute(vt_now), second(vt_now));

		GLCD.DrawString(buf, gTextfmt_right, gTextfmt_top, eraseTO_EOL);
		int vi_day = day(vt_now);
		if (gt_prevDay != vi_day) {   //next day
			gt_prevDay = vi_day;
			snprintf(buf, sizeof(buf), "%02d.%02d.%04d", vi_day, month(vt_now), year(vt_now));
			// draw the formatted string centered on the display
			GLCD.DrawString(buf, gTextfmt_left, gTextfmt_top);
		}
		GLCD.DrawString(isNightTariff() ? F(" NOC ") : F("DZIEN"), gTextfmt_center, gTextfmt_top);
	}
	GLCD.SelectFont(System5x7);
}

void printScreen(int vi_counter) {
	switch (vi_currentScreen) {
	case SCREEN_MAIN:
		printTemps(vi_counter);
		printWent();
		break;
	case SCREEN_CONFIG_GODZ:
		printConfigGodz(getHourDayStart(), getHourDayEnd(), getHourNightStart(), getHourNightEnd(), getMinuteNightShift());
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
}

void handleKeyOnCurrentScreen(char key) {
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
}

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
		float tempC = getTempC(i);
		if (i < 4) {
			GLCD.CursorToXY(2, 12 + 9 * i);
			GLCD.print(tempC, 1);
		}
		setCurrentTemps(i, tempC);
	}
	GLCD.SelectFont(Wendy3x5);
	GLCD.CursorToXY(28, 10);   //ile idze na podlogowke
	GLCD.print(getCurrentTemps(PODLOGA_OUT), 1);
	GLCD.CursorToXY(28, 42);   //ile wraca z podlogowki
	GLCD.print(getCurrentTemps(PODLOGA_IN), 1);
	GLCD.SelectFont(System5x7);

	if (isFloorPumpWorking() && !isFloorPumpDrawing() && counter % 5) {
		setFloorPumpDrawing(true);
	} else if (counter % 9) {
		setFloorPumpDrawing(false);
	}
	if (isFloorPumpDrawing())
		GLCD.FillTriangle(34, 23, 34, 27, 38, 25, PIXEL_ON);
	else
		GLCD.FillTriangle(34, 23, 34, 27, 38, 25, PIXEL_OFF);

	if (isMixingPumpWorking() && !isMixingPumpDrawing()) {
		setMixingPumpDrawing(true);
	} else {
		setMixingPumpDrawing(false);
	}
	if (isMixingPumpDrawing())
		GLCD.DrawCircle(31, 35, 2, PIXEL_ON);
	//GLCD.FillTriangle(28, 32,  30, 36,32, 32, PIXEL_ON);
	else
		GLCD.DrawCircle(31, 35, 2, PIXEL_OFF);
	//GLCD.FillTriangle(28, 32,  30, 36,32, 32, PIXEL_ON);

	vb_tempsPrinted = true;
}

//draw went and print temps
void printWent() {
	const int x0 = 50;
	const int xmid = 89;
	const int x1 = 128;
	const int y0 = 16;
	const int ymid = 32;
	const int y1 = 47;
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

void printConfigGodz2() {
	char buf[60];
	GLCD.DrawString(F("Czas"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "H: 1,2 M: 3,4 S: 5,6\n y: 7,8 m:9,0 d: A,B\n C - set clock\n%02d.%02d.%04d", day(), month(), year());
	GLCD.DrawString(buf, 0, 17); //, eraseFULL_LINE);
}

void printConfigTemp() {
	char buf[45];
	GLCD.DrawString(F("Temperatury Klawisze"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "D.%02d-%02d 1-2+:3-A+\nN.%02d-%02d 4-5+:6-B+", getTempMinDay(), getTempMaxDay(), getTempMinNight(), getTempMaxNight());
	GLCD.DrawString(buf, 0, 17); //, eraseFULL_LINE);
	//GLCD.DrawString(F("Zmiana klawisze\nD1-2+3-A+ N4-5+6-B+"), 0, 26);
	snprintf(buf, sizeof(buf), "PompMiesz.%02d 7-8+", getTempMixingStart());
	GLCD.DrawString(buf, 0, 35);	//, eraseFULL_LINE);
}

void printConfigWent() {
	char buf[40];
	GLCD.DrawString(F("Wentylatory"), gTextfmt_center, 9);
	snprintf(buf, sizeof(buf), "N %04d U %04d S:%03d", getDesiredWentRPM(NEW_WENT) * 10, getDesiredWentRPM(USED_WENT) * 10, getWentStep());
	GLCD.DrawString(buf, 0, 17);	//, eraseFULL_LINE);
	GLCD.DrawString(F("Zmiana klawisze\nN 1-2+ U 4-5+\nA wysyla B krok\n C sw, Dew"), 0, 26);
}

