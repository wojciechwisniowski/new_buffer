/*
 * vents_prod.h
 *
 *  Created on: Nov 7, 2016
 *      Author: wisnia
 */

#ifndef VENTS_PROD_H_
#define VENTS_PROD_H_
#include <Arduino.h>
#include <EEPROM.h>
#include <Time.h>
#include <Wire.h>

void getWentString(char * buf);
void setWents();



#endif /* VENTS_PROD_H_ */
