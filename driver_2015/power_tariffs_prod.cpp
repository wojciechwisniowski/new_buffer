/*
 *  power_tariffs_prod.cpp
 *
 *  Created on: Nov 9, 2016
 *      Author: wisnia
 */
#include "power_tariffs.h"
#include "power_tariffs_prod.h"

int getHourIncludingNightShift(time_t current) {
	int h = hour(current - (getMinuteNightShift() * 60UL)); //get hour shifted
	return h;
}

