/*
 * power_tariffs.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#pragma once
#ifndef POWER_TARIFFS_H_
#define POWER_TARIFFS_H_

#include <Arduino.h>
#include <Time.h>

//extern uint8_t gi_EE_Hour_Night_Start;
//extern uint8_t gi_EE_Hour_Night_End;
//extern uint8_t gi_EE_Hour_Day_Start;
//extern uint8_t gi_EE_Hour_Day_End;
//extern uint8_t gi_EE_Minute_Night_Shift; //difference between real time and the time on the energy meter - NOT IMPLEMENTED
//
//extern uint8_t gi_Hour_Night_Start;
//extern uint8_t gi_Hour_Night_End;
//extern uint8_t gi_Hour_Day_Start;
//extern uint8_t gi_Hour_Day_End;
//extern uint8_t gi_Minute_Night_Shift; //difference between real time and the time on the energy meter - NOT IMPLEMENTED
//extern boolean vb_taryfaNocna;

uint8_t getHourDayStart();
uint8_t getHourDayEnd();
uint8_t getHourNightStart();
uint8_t getHourNightEnd();
uint8_t getMinuteNightShift();

void initConfigStrefy();
void checkAndChangeTariff();
boolean isNightTariff();

void decDzienStart();
void incDzienStart();
void decDzienEnd();
void incDzienEnd();

void decNocStart();
void incNocstart();
void decNocEnd();
void incNocEnd();

void decMinuteShift();
void incMinuteShift();




#endif /* POWER_TARIFFS_H_ */
