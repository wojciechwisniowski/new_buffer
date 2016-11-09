/*
 * test.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: wisnia
 */

#include <stdio.h>
#include <string.h>
#include "termometry.h"
#include "minunit.h"
#include "rest_tst.h"
#include "termometry_tst.h"
#include "vents_tst.h"
#include "power_tariffs_tst.h"

int tests_run = 0;
int all_tests_run = 0;

int test_run(char * testFunction(), const char * name) {

	printf("----------------------- STARTING %s TESTS -----------------------\n", name);

	char *result = testFunction();
	printf("Tests run: %d\n", tests_run);
	if (result != 0) {
		printf("Failed:%s\n", result);
	} else {
		printf("----------------------- ALL %s TESTS OK -----------------------\n", name);
	}
	all_tests_run += tests_run;
	tests_run = 0;
	return result != 0;
}

int main(int argc, char **argv) {
	int final_result = 0;
	final_result += test_run(rest_tests, "REST");
	final_result += test_run(termometers_tests, "TEMPS");
	final_result += test_run(vents_tests, "VENTS");
	final_result += test_run(power_tariffs, "POWER_TF");


	printf("All tests run: %d\n", all_tests_run);
	if (final_result != 0) {
		printf("----------------------- NOT ALL TESTS OK failed: %d -----------------------\n", final_result);
	} else {
		printf("----------------------- ALL:%d TESTS OK -----------------------\n", all_tests_run);
	}
}

