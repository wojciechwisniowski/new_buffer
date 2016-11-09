/*
 * power_tariffs.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#ifndef POWER_TARIFFS_H_
#define POWER_TARIFFS_H_
#include "power_tariffs_prod.h"

int getHourDayStart();
int getHourDayEnd();
int getHourNightStart();
int getHourNightEnd();
int getMinuteNightShift();

void initConfigStrefy(); //
bool isCheapTariff(int h, int dayOfTheWeek);

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
