/*
 * driver_buffer_prod.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: wisnia
 */
#include "driver_buffer_prod.h"

void ustawPinyGrzalek(uint8_t stan) {
	digitalWrite(GI_PIN_GRZALEK_1, stan);
	digitalWrite(GI_PIN_GRZALEK_2, stan);
	digitalWrite(GI_PIN_GRZALEK_3, stan);
}

void setupPompy() {
	pinMode(GI_PIN_POMPY_MIESZAJACEJ, OUTPUT);
	pinMode(GI_PIN_POMPY_PODLOGOWEJ, OUTPUT);
}

void setupBuf() {
	pinMode(GI_PIN_GRZALEK_1, OUTPUT);
	pinMode(GI_PIN_GRZALEK_2, OUTPUT);
	pinMode(GI_PIN_GRZALEK_3, OUTPUT);
}

void setMixingPumpHW(bool state) {
	if (state) {
		digitalWrite(GI_PIN_POMPY_MIESZAJACEJ, LOW);
	} else {
		digitalWrite(GI_PIN_POMPY_MIESZAJACEJ, HIGH);
	}
}


void setHeaterHW(bool state) {
	if (state) {
		ustawPinyGrzalek(LOW);
	} else {
		ustawPinyGrzalek(HIGH);
	}
}

