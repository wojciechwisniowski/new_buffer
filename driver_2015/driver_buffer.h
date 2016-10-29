/*
 * driver_buffer.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#ifndef DRIVER_BUFFER_H_
#define DRIVER_BUFFER_H_
#include <Arduino.h>
#include "termometry.h"
#include "screens_status.h"
#include "power_tariffs.h"

#define GI_PIN_GRZALEK_1 42
#define GI_PIN_GRZALEK_2 43
#define GI_PIN_GRZALEK_3 44
#define GI_PIN_POMPY_MIESZAJACEJ 45//pompa mieszajaca bufor
#define GI_PIN_POMPY_PODLOGOWEJ 46//pompa od podłogówki

boolean isMixingPumpWorking();
boolean isMixingPumpDrawing();
boolean isFloorPumpWorking();
boolean isFloorPumpDrawing();

void setMixingPumpDrawing(boolean);
void setFloorPumpDrawing(boolean);

void setupBuf();
void setupPompy();
void wylaczGrzalki();
void wlaczGrzalki();
void obslugaPompyMieszajacej(float temp);
void wylaczPompaBuf();
void wlaczPompaBuf();
void ustawPinyGrzalek(uint8_t stan);
void checkAndChangeBuffor();

boolean isBufforHeating();


void printGrzalkiStatus(int grzejDo, int czekajDo);



#endif /* DRIVER_BUFFER_H_ */
