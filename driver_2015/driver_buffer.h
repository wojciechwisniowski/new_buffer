/*
 * driver_buffer.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#ifndef DRIVER_BUFFER_H_
#define DRIVER_BUFFER_H_
#include "driver_buffer_prod.h"

#include "termometry.h"
#include "screens_status.h"
#include "power_tariffs.h"


bool isMixingPumpWorking();
bool isMixingPumpDrawing();
bool isFloorPumpWorking();
bool isFloorPumpDrawing();

void setMixingPumpDrawing(bool);
void setFloorPumpDrawing(bool);

void wylaczGrzalki(void (*heaterSetter)(bool));
void wlaczGrzalki(void (*heaterSetter)(bool));
void obslugaPompyMieszajacej(float temp, int h, int dayOfTheWeek,void (*setMixingPump)(bool));//
void wylaczPompaBuf(void (*setMixingPump)(bool));
void wlaczPompaBuf(void (*setMixingPump)(bool));

void checkAndChangeBuffor(int h, int dayOfTheWeek,void (*setMixingPump)(bool),char * (*bottomStatusPrinter) (const char *),void (*heaterSetter)(bool));

bool isBufforHeating();

char * printGrzalkiStatus(int grzejDo, int czekajDo,char* (*bottomStatusPrinter) (const char *));

#endif /* DRIVER_BUFFER_H_ */
