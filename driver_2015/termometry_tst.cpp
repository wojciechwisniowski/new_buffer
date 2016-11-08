/*
 * termometry_tst.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: wisnia
 */
#include "minunit.h"
#include "termometry_tst.h"

char * test_default() {
	mu_assert((char* )"Set default min day temp failed", getTempMinDay() == 29); //ci_minDayTempDefault);
	mu_assert((char* )"Set default max day temp failed", getTempMaxDay() == 31); //ci_maxDayTempDefault);
	mu_assert((char* )"Set default max night temp failed", getTempMaxNight() == 80); //ci_maxNightTempDefault);
	mu_assert((char* )"Set default min night temp failed", getTempMinNight() == 60); //ci_minNightTempDefault);
	mu_assert((char* )"Set default mixing temp failed", getTempMixingStart() == 55); //ci_mixingPumpStartTempDefault);
}

char * test_init() {
	initConfigTemp();

	setMinDayTemp(100); //100 is over the limit
	setMaxDayTemp(100); //100 is over the limit
	setMinNightTemp(100); //100 is over the limit
	setMaxNightTemp(100); //100 is over the limit
	setMixingPumpStartTemp(100);
	test_default();
}

char * test_init2() {
	initConfigTemp();

	setMinDayTemp(0); //100 is over the limit
	setMaxDayTemp(0); //100 is over the limit
	setMinNightTemp(0); //100 is over the limit
	setMaxNightTemp(0); //100 is over the limit
	setMixingPumpStartTemp(0);
	test_default();
}

char * test_set50() {
	initConfigTemp();
	setMinDayTemp(50); //50 is in the limit
	setMaxDayTemp(50);
	setMinNightTemp(50);
	setMaxNightTemp(50);
	setMixingPumpStartTemp(50);
	mu_assert((char* )"set min day temp failed", getTempMinDay() == 50);
	mu_assert((char* )"set max day temp failed", getTempMaxDay() == 50);
	mu_assert((char* )"set max night temp failed", getTempMaxNight() == 50);
	mu_assert((char* )"set min night temp failed", getTempMinNight() == 50);
	mu_assert((char* )"set mixing temp failed", getTempMixingStart() == 50);
}

char * test_inc() {
	test_set50();
	incMinDzienna();
	incMaxDzienna();
	incMinNocna();
	incMaxNocna();
	incPompaMiesz();
	mu_assert((char* )"min day temp not inc", getTempMinDay() == 51);
	mu_assert((char* )"max day temp not inc", getTempMaxDay() == 51);
	mu_assert((char* )"max night temp not inc", getTempMaxNight() == 51);
	mu_assert((char* )"min night temp not inc", getTempMinNight() == 51);
	mu_assert((char* )"mixing temp not inc", getTempMixingStart() == 51);
}

char * test_dec() {
	test_set50();
	decMinDzienna();
	decMaxDzienna();
	decMinNocna();
	decMaxNocna();
	decPompaMiesz();
	mu_assert((char* )"min day temp not dec", getTempMinDay() == 49);
	mu_assert((char* )"max day temp not dec", getTempMaxDay() == 49);
	mu_assert((char* )"max night temp not dec", getTempMaxNight() == 49);
	mu_assert((char* )"min night temp not dec", getTempMinNight() == 49);
	mu_assert((char* )"mixing temp not dec", getTempMixingStart() == 49);
}

//bool checkTemp(int  temp); //check if the tem is between const uint8_t ci_minTemp = 5; const uint8_t ci_maxTemp = 95;
char * test_checkTemp() {
//const int ci_minTemp = 5;
//const int ci_maxTemp = 95;
	mu_assert((char* )"checkTemp:-1", !checkTemp(-1));
	mu_assert((char* )"checkTemp:0", !checkTemp(0));
	mu_assert((char* )"checkTemp:4", !checkTemp(4));
	mu_assert((char* )"checkTemp:5", checkTemp(5));
	mu_assert((char* )"checkTemp:6", checkTemp(6));
	mu_assert((char* )"checkTemp:95", checkTemp(95));
	mu_assert((char* )"checkTemp:96", !checkTemp(96));
}

char * test_currentTemps() {
//#define TEMPCOUNT 6 //ilosc termometrów
	for (int i = 0; i < TEMPCOUNT; i++)
		setCurrentTemps(i, 12.34f);
	for (int i = 0; i < TEMPCOUNT; i++) {
		mu_assert((char* )"getCurrentTemps[i]", getCurrentTemps(i) == 12.34f);
	}

}

char * termometers_tests() {
	mu_run_test(test_init);
	mu_run_test(test_init2);
	mu_run_test(test_set50);
	mu_run_test(test_inc);
	mu_run_test(test_dec);
	mu_run_test(test_checkTemp);
	mu_run_test(test_currentTemps);

	return 0;
}
