/*
 * vents.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#ifndef VENTS_H_
#define VENTS_H_

#include "screens_status.h"//no our includes inside
#include "power_tariffs.h"//no our includes inside
#include <stdio.h>
#include <stdlib.h>

#define WENTCOUNT 2 //ilość wentylatorów
#define TEMPCOUNT_REKU 4 //termometry requ

#define REKUPERATYOR_ID 4 //id na szynie I2C

#define NEW_WENT 0
#define NEW_WENT_MAX 270
#define USED_WENT 1
#define USED_WENT_MAX 265
#define WENT_STEP1 100
#define WENT_STEP2 200
#define WENT_STEP3 500
#define NEW_IN 0
#define NEW_OUT 1
#define USED_IN 2
#define USED_OUT 3

int getNightWentAdd(); //

void changeWentStep(); //

void decWentUSED(); //
void decWentNew(); //

void incWentNew(); //
void incWentUSED(); //

void startWietrzenie(int h, int dayOfTheWeek, void (*)());
void stopWietrzenie(void (*)());
void checkWietrzenie(int,int, void (*)());

void initConfigWent(); //
void parseRekuperatorMSG(const char * msg); //

void incWent(int went); //
void decWent(int went); //

float getCurrentTemReku(int nr);
int getCurrentVentRPM(int nr);

int getDesiredWentRPM(int nr);
void setDesiredVentRPM(int nr, int rpm);

int getDesiredAiringVentRPM(int nr);
void setDesiredAiringVentRPM(int nr, int rpm);

int getWentStep();

bool isAiring();

bool checkRPM(int rpm);

#endif /* VENTS_H_ */
