/*
 * driver_buffer_prod.h
 *
 *  Created on: Nov 12, 2016
 *      Author: wisnia
 */

#ifndef DRIVER_BUFFER_PROD_H_
#define DRIVER_BUFFER_PROD_H_
#include <Arduino.h>

#define GI_PIN_GRZALEK_1 42
#define GI_PIN_GRZALEK_2 43
#define GI_PIN_GRZALEK_3 44
#define GI_PIN_POMPY_MIESZAJACEJ 45//pompa mieszajaca bufor
#define GI_PIN_POMPY_PODLOGOWEJ 46//pompa od podłogówki

void ustawPinyGrzalek(uint8_t stan);
void setupBuf();
void setupPompy();
void setMixingPumpHW(bool state);
void setHeaterHW(bool state);

#endif /* DRIVER_BUFFER_PROD_H_ */
