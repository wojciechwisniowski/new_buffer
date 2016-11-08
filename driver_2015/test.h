/*
 * test.h - this is included into test compilation instead of prod.h
 *
 *  Created on: Nov 7, 2016
 *      Author: wisnia
 */

#ifndef TEST_H_
#define TEST_H_

const bool gb_production = false;


#ifdef DEBUG
#define debug(message) do { printf("%s\n", message); } while (0)
#else
#define debug(message)
#endif

void inline writeTemToEprom(int *addr, int value) {
	//do nothing
}

int inline readTemFromEprom(const int *addr) {
		return 255;
}


#endif /* TEST_H_ */
