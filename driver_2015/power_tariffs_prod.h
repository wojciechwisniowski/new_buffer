/*
 * power_tariffs_prod.h
 *
 *  Created on: Nov 9, 2016
 *      Author: wisnia
 */

#ifndef POWER_TARIFFS_PROD_H_
#define POWER_TARIFFS_PROD_H_
#include <Arduino.h>
#include <Time.h>


int getHourIncludingNightShift(time_t current);


#endif /* POWER_TARIFFS_PROD_H_ */
