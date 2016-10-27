/*
 * vents.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#ifndef VENTS_H_
#define VENTS_H_
#include <Arduino.h>
#include <EEPROM.h>
#include <Time.h>
#include <Wire.h>
#include "screens_status.h"//no our includes inside
#include "termometry.h"//no our includes inside
#include "power_tariffs.h"//no our includes inside

#define WENTCOUNT 2 //ilość wentylatorów
#define TEMPCOUNT_REKU 4 //termometry requ


#define REKUPERATYOR_ID 4 //id na szynie I2C

#define NEW_WENT 0
#define NEW_WENT_MAX 2700
#define USED_WENT 1
#define USED_WENT_MAX 2650
#define WENT_STEP1 100
#define WENT_STEP2 200
#define WENT_STEP3 500
#define NEW_IN 0
#define NEW_OUT 1
#define USED_IN 2
#define USED_OUT 3


void changeWentStep();

void decWentUSED();
void decWentNew();

void incWentNew();
void incWentUSED();

void setWents();
void startWietrzenie();
void stopWietrzenie();
void checkWietrzenie();
String getWentString();

void initConfigWent();
void parseRekuperatorMSG(String msg);

void incWent(int went, int max);


void decWent(int went, int max);

float getCurrentTemReku(int nr);
int getCurrentVentRPM(int nr);
int getDesiredWentRPM(int nr);
int getWentStep();

boolean isAiring();







#endif /* VENTS_H_ */
