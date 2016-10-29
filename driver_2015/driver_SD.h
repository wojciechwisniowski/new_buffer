/*
 * driver_SD.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#ifndef DRIVER_SD_H_
#define DRIVER_SD_H_

#include <Arduino.h>
#include <SD.h>
#include <Time.h>
#include "screens_status.h"//no our includes inside
#include "termometry.h"
#include "vents.h"
#include "driver_buffer.h"

int setupSD(int line);



#endif /* DRIVER_SD_H_ */
