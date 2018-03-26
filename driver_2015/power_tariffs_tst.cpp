/*
 * power_tariffs_tst.cpp
 *
 *  Created on: Nov 9, 2016
 *      Author: wisnia
 */
#include "minunit.h"
#include "power_tariffs_tst.h"

char * power_tariffs_test_init() {
	initConfigStrefy();
	mu_assert((char* )"getHourDayStart() - after init", getHourDayStart() == 13);
	mu_assert((char* )"getHourDayEnd() - after init", getHourDayEnd() == 15);
	mu_assert((char* )"getHourNightStart() - after init", getHourNightStart() == 22);
	mu_assert((char* )"getHourNightEnd() - after init", getHourNightEnd() == 6);
	mu_assert((char* )"getMinuteNightShift() - after init", getMinuteNightShift() == 5);
	return 0;
}

char * power_tariffs_test_checkAndChangeTariff() {
	for (int i = 0; i < 6; i++) {
		mu_assert((char* )"isCheapTariff(0-6);", isCheapTariff(i,5));
		mu_assert((char* )"isNightCheapTariff(0-6);", isNightCheapTariff(i));
		mu_assert((char* )"isDayCheapTariff(0-6);", !isDayCheapTariff(i));
	}
	for (int i = 7; i < 13; i++) {
		mu_assert((char* )"isCheapTariff(7-12);", !isCheapTariff(i,5));
		mu_assert((char* )"isNightCheapTariff(7-12);", !isNightCheapTariff(i));
		mu_assert((char* )"isDayCheapTariff(7-12);", !isDayCheapTariff(i));
	}
	for (int i = 13; i < 15; i++) {
		mu_assert((char* )"isCheapTariff(13-15);", isCheapTariff(i,5));
		mu_assert((char* )"isNightCheapTariff(13-15);", !isNightCheapTariff(i));
		mu_assert((char* )"isDayCheapTariff(13-15);", isDayCheapTariff(i));
	}
	for (int i = 15; i < 22; i++) {
		mu_assert((char* )"isCheapTariff(15-22);", !isCheapTariff(i,5));
		mu_assert((char* )"isNightCheapTariff(15-22);", !isNightCheapTariff(i));
		mu_assert((char* )"isDayCheapTariff(15-22);", !isDayCheapTariff(i));
	}
	for (int i = 22; i < 25; i++) {
		mu_assert((char* )"isCheapTariff(22-25);", isCheapTariff(i,5));
		mu_assert((char* )"isNightCheapTariff(22-25);", isNightCheapTariff(i));
		mu_assert((char* )"isDayCheapTariff(22-25);", !isDayCheapTariff(i));

	}
//TODO what to do with such hours
	mu_assert((char* )"checkAndChangeTariff(-1);", isCheapTariff(-1,5));
	mu_assert((char* )"checkAndChangeTariff(25);", isCheapTariff(25,5));


	mu_assert((char* )"isCheapTariff - sat;", isCheapTariff(17,7));
	mu_assert((char* )"isCheapTariff - sun;", isCheapTariff(17,1));
	mu_assert((char* )"isCheapTariff - wed;", !isCheapTariff(17,4));

	mu_assert((char* )"isWeekend(- sat;", isWeekend(7));
	mu_assert((char* )"isWeekend( - sun;", isWeekend(1));
	mu_assert((char* )"isWeekend( - wed;", !isWeekend(4));
}

char * power_tariffs_test_checkMorningTurboBoost() {
	mu_assert((char* )"isMorningTurboHeater(3);", !isMorningTurboHeater(3));
	mu_assert((char* )"isMorningTurboHeater(4);", isMorningTurboHeater(4));
	mu_assert((char* )"isMorningTurboHeater(5);", isMorningTurboHeater(5));
	mu_assert((char* )"!isMorningTurboHeater(6);", !isMorningTurboHeater(6));
	mu_assert((char* )"!isMorningTurboHeater(6);", !isMorningTurboHeater(7));
}


char * power_tariffs() {
	mu_run_test(power_tariffs_test_init);
	mu_run_test(power_tariffs_test_checkAndChangeTariff);
	mu_run_test(power_tariffs_test_checkMorningTurboBoost);

	return 0;
}

