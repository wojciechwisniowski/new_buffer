/*
 * screens.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#ifndef SCREENS_H_
#define SCREENS_H_

#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <openGLCD.h>
#include <Time.h>
#include "power_tariffs.h"

#define NUMBER_OF_SCREENS 6
#define SCREEN_MAIN 0
#define SCREEN_CONFIG_GODZ 1
#define SCREEN_CONFIG_TEMP 2
#define SCREEN_CONFIG_WENT 3
#define SCREEN_CONFIG_FILE 4
#define SCREEN_CONFIG_GODZ2 5

extern int vi_currentScreen;
extern boolean vb_tempsPrinted; //optymalizacja rysowania
extern boolean vb_wentsPrinted; //optymalizacja rysowania
extern int gt_prevDay;
extern time_t gt_prevtime;

void nextScreen();
void prevScreen();

void printConfigGodz(uint8_t pi_Hour_Day_Start, uint8_t pi_Hour_Day_End, uint8_t pi_Hour_Night_Start, uint8_t pi_Hour_Night_End, uint8_t pi_Minute_Night_Shift);
void printConfigGodz2();
void printConfigTemp();
void printConfigWent();
void printConfigFiles();

void printDigitalClock();
void printGrzalkiStatus(int grzejDo, int czekajDo);
void printWent();
void printTemps(int counter);

void clearScreenWithoutTime();

int setupGLCD(int line);

#endif /* SCREENS_H_ */
