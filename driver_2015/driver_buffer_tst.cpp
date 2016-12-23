/*
 * driver_buffer_tst.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: wisnia
 */

#include "minunit.h"
#include "driver_buffer_tst.h"
#include "test.h"

char *bottomStatusPrinter1(const char * text) {
	mu_assert((char* )" bottomStatusPrinter1 ", strcmp("Buf czeka do:01", text) == 0);
	return 0;
}

char * driver_buffer_test_init() {
	mu_assert((char* )" isBufforHeating - after init", !isBufforHeating());
	mu_assert((char* )" isMixingPumpWorking - after init", !isMixingPumpWorking());
	char * status = printGrzalkiStatus(1, 1, bottomStatusPrinter1);

	return status;
}

//void obslugaPompyMieszajacej(float temp, int h, int dayOfTheWeek, void (*setMixingPump)(bool)) {
//	if (!isCheapTariff(h, dayOfTheWeek)) {
//		wylaczPompaBuf(setMixingPump); //w dzien nie mieszamy
//	} else {
//		if (vb_buforGrzeje && (temp > getTempMixingStart())) {
//			wlaczPompaBuf(setMixingPump); //grzeje i zagrzal juz CWU do gi_temperaturaStartuMieszania wiec grzej dol
//		} else {
//			wylaczPompaBuf(setMixingPump); // albo nie grzeje albo sie CWU wychlodzilo to nie mieszamy dalej
//		}
//	}
//}

char * driver_buffer_test_obslugaPompyMieszajacej() {
	mu_assert((char* )" isMixingPumpWorking - after init", !isMixingPumpWorking());
	wlaczPompaBuf(setMixingPump_empty);
	mu_assert((char* )" isMixingPumpWorking - after start", isMixingPumpWorking());
	obslugaPompyMieszajacej(1, 11, 4, setMixingPump_empty); //not cheap tariff so it should stop mixing
	mu_assert((char* )" isMixingPumpWorking - at 11:00", !isMixingPumpWorking());

	wlaczPompaBuf(setMixingPump_empty);
	mu_assert((char* )" isMixingPumpWorking - after start", isMixingPumpWorking());
	obslugaPompyMieszajacej(1, 23, 4, setMixingPump_empty); //cheap tariff but buffer is not heating so it should stop mixing
	mu_assert((char* )" isMixingPumpWorking - at 1deg", !isMixingPumpWorking());

	wlaczGrzalki(setHeater_empty); // buf is heating
	mu_assert((char* )" isBufforHeating - after init", isBufforHeating());
	wlaczPompaBuf(setMixingPump_empty); //mixing pump is working
	mu_assert((char* )" isMixingPumpWorking - after start", isMixingPumpWorking());
	obslugaPompyMieszajacej(1, 23, 4, setMixingPump_empty); //cheap tariff, buffer heaating but too cold -> stop mixing
	mu_assert((char* )" isMixingPumpWorking - at 1deg", !isMixingPumpWorking());

	wlaczGrzalki(setHeater_empty); // buf is heating
	mu_assert((char* )" isBufforHeating - after init", isBufforHeating());
	wylaczPompaBuf(setMixingPump_empty); //mixing pump is working
	mu_assert((char* )" isMixingPumpWorking - after stop", !isMixingPumpWorking());
	obslugaPompyMieszajacej(70, 23, 4, setMixingPump_empty); //cheap tariff, buffer heaating , temp ok -> start mixing
	mu_assert((char* )" isMixingPumpWorking - at 1deg", isMixingPumpWorking());
	wylaczGrzalki(setHeater_empty); // buf is not heating
	wylaczPompaBuf(setMixingPump_empty); //mixing pump is working
	return 0;
}

//void checkAndChangeBuffor(int h, int dayOfTheWeek, void (*setMixingPump)(bool), char * (*bottomStatusPrinter)(char *),void (*heaterSetter)(bool)) {
//	float temp = getCurrentTemps(1); //temperatrua z ciut powyzej polowy zbiornika
//	int grzejDo;
//	int czekajDo;
//	if (isCheapTariff(h, dayOfTheWeek)) { // noc lub poludnie lub weekend
//		grzejDo = getTempMaxNight();
//		czekajDo = getTempMinNight();
//	} else { // dzien
//		grzejDo = getTempMaxDay();
//		czekajDo = getTempMinDay();
//	}
//
//	if (vb_buforGrzeje) { // jak grzeje to spradz czy nie nagrzal
//		if (temp >= grzejDo) { // nagrzal to wylacz
//			wylaczGrzalki(heaterSetter);
//		}
//	} else { //nie grzeje
//		if (temp < czekajDo) { // temperatura ponizej ustawionej  -> wlacz grzalki
//			wlaczGrzalki(heaterSetter);
//		}
//	}
//
//	printGrzalkiStatus(grzejDo, czekajDo, bottomStatusPrinter);
//	obslugaPompyMieszajacej(temp, h, dayOfTheWeek, setMixingPump);
//}
char *bottomStatusPrinter2(const char * text) {
	mu_assert((char* )" bottomStatusPrinter2 ", strcmp("Buf czeka do:02", text) == 0);
	return 0;
}

char * driver_buffer_test_checkAndChangeBuffor() {
	char *ret = 0;
	mu_assert((char* )" isBufforHeating - after init", !isBufforHeating());
	mu_assert((char* )" isMixingPumpWorking - after init", !isMixingPumpWorking());
	ret = printGrzalkiStatus(1, 2, bottomStatusPrinter2);
	mu_assert(ret, ret == 0);
	//week day - day
	initConfigTemp();
	setCurrentTemps(1, 30); //min day 29 max day 31
	checkAndChangeBuffor(16, 4, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekday at 16 at 30", !isBufforHeating());

	setCurrentTemps(1, 28); //min day 29 max day 31
	checkAndChangeBuffor(16, 4, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekday at 16 at 28", isBufforHeating());

	setCurrentTemps(1, 30); //min day 29 max day 31 - still heating until 31
	checkAndChangeBuffor(16, 4, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekday at 16 at 30", isBufforHeating());

	setCurrentTemps(1, 31); //min day 29 max day 31 - stop at 31
	checkAndChangeBuffor(16, 4, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekday at 16 at 31", !isBufforHeating());

	//week day - night
	initConfigTemp();
	setCurrentTemps(1, 61); //min night 60 max night 80
	checkAndChangeBuffor(23, 4, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekday at 23 at 61", !isBufforHeating());

	setCurrentTemps(1, 59); //min night 60 max night 80
	checkAndChangeBuffor(23, 4, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekday at 23 at 59", isBufforHeating());

	setCurrentTemps(1, 79); //min night 60 max night 80 - still heating until 31
	checkAndChangeBuffor(23, 4, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekday at 23 at 79", isBufforHeating());

	setCurrentTemps(1, 81); //min night 60 max night 80 - stop at 81
	checkAndChangeBuffor(23, 4, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekday at 23 at 81", !isBufforHeating());


	//weekend
	setCurrentTemps(1, 61); //minn 60 maxn 80
	checkAndChangeBuffor(16, 1, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekend at 16 at 61", !isBufforHeating());

	setCurrentTemps(1, 59); //minn 60 maxn 80
	checkAndChangeBuffor(16, 1, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekend at 16 at 61", isBufforHeating());

	setCurrentTemps(1, 79); //minn 60 maxn 80
	checkAndChangeBuffor(16, 1, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekend at 16 at 79", isBufforHeating());

	setCurrentTemps(1, 80); //minn 60 maxn 80
	checkAndChangeBuffor(16, 1, setMixingPump_empty, bottomStatusPrinter2, setHeater_empty);
	mu_assert((char* )" isBufforHeating - weekend at 16 at 80", !isBufforHeating());


	return ret;
}

char * driver_buffer() {
	mu_run_test(driver_buffer_test_init);
	mu_run_test(driver_buffer_test_obslugaPompyMieszajacej);
	mu_run_test(driver_buffer_test_checkAndChangeBuffor);

	return 0;
}

void setMixingPump_empty(bool state) {
}
void setMixingPumpTST(bool state) {
	if (state) {
		printf("setMixingPumpTST - true");
	} else {
		printf("setMixingPumpTST - false");
	}
}
void setHeater_empty(bool state) {
}
void setHeaterTST(bool state) {
	if (state) {
		printf("setHeaterTST - true");
	} else {
		printf("setHeaterTST - false");
	}
}

