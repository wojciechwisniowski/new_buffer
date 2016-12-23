/*
 * screens_status.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#ifndef SCREENS_STATUS_H_
#define SCREENS_STATUS_H_
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <openGLCD.h>

char * printBottomStatus(const char * buf);
void printBottomStatusSent(const char * buf);
void print0(int line, char const * str);

#endif /* SCREENS_STATUS_H_ */
