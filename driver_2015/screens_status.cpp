/*
 * screens_status.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "screens_status.h"

//void printBottomStatus(const char * buf) {
//	GLCD.DrawString(buf, gTextfmt_center, gTextfmt_bottom, eraseTO_EOL);
//}

char * printBottomStatus(const char* s) {
	GLCD.SelectFont(Wendy3x5);
	GLCD.DrawString(s, gTextfmt_left, gTextfmt_bottom, eraseFULL_LINE);
	GLCD.SelectFont(System5x7);
	return 0;
}

//TODO refactor
void printBottomStatusSent(const char * buf) {
	GLCD.DrawString(F("sent"), gTextfmt_left, gTextfmt_bottom, eraseTO_EOL);
	GLCD.DrawString(buf, 60, gTextfmt_bottom, eraseTO_EOL);
}

void print0(int line, char const * str) {
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
	GLCD.CursorTo(0, line);
// print the number of seconds since reset:
	GLCD.print(str);
}

void print0(int line, float f) {
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
	GLCD.CursorTo(0, line);
// print the number of seconds since reset:
	GLCD.print(f);
}

