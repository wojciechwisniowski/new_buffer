/*
 * driver_buffer_tst.h
 *
 *  Created on: Nov 12, 2016
 *      Author: wisnia
 */

#ifndef DRIVER_BUFFER_TST_H_
#define DRIVER_BUFFER_TST_H_

#include "driver_buffer.h"

char * driver_buffer();

void setMixingPump_empty(bool state);
void setMixingPumpTST(bool state);
void setHeater_empty(bool state);
void setHeaterTST(bool state);
void setTimeRTC();

#endif /* DRIVER_BUFFER_TST_H_ */
