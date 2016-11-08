/*
 * prod.h - this file is included into production version of a driver
 *
 *  Created on: Nov 7, 2016
 *      Author: wisnia
 */

#ifndef PROD_H_
#define PROD_H_

#include <Arduino.h>

const bool gb_production = true;

void inline writeTemToEprom(int *addr, int value) {
	eeprom_write_byte((uint8_t*) addr, value);
}

uint8_t inline readTemFromEprom(const int *addr) {
	return eeprom_read_byte((uint8_t*) addr);
}

#endif /* PROD_H_ */
