/*
 * driver_buffer.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "driver_buffer.h"

bool vb_pompaMieszajacaPracuje = false;
bool vb_pompaMieszajacaRysuj = true;
bool vb_pompaPodlogowaPracuje = true;
bool vb_pompaPodlogowaRysuj = true;
bool vb_buforGrzeje = false;

bool isMixingPumpWorking() {
	return vb_pompaMieszajacaPracuje;
}
bool isMixingPumpDrawing() {
	return vb_pompaMieszajacaRysuj;
}
bool isFloorPumpWorking() {
	return vb_pompaPodlogowaPracuje;
}

void setFloorPumpWorking(bool status, void (*floorPumpSetter)(bool)) {
	vb_pompaPodlogowaPracuje = status;
	floorPumpSetter(status);
}

bool isFloorPumpDrawing() {
	return vb_pompaPodlogowaRysuj;
}

bool isBufforHeating() {
	return vb_buforGrzeje;
}

void setMixingPumpDrawing(bool value) {
	vb_pompaMieszajacaRysuj = value;
}
void setFloorPumpDrawing(bool value) {
	vb_pompaPodlogowaRysuj = value;
}

// second algorithm 14h all in cheap tariff
void handleFloorPumpWeekend(int h, void (*floorPumpSetter)(bool)) {
	if ((h >= 4 && h < 10) || //heat for morning bathroom 04:00-09:59 - 6h
			(h >= 12 && h < 14) || // heat using cheap p  12:00-13:59 - 2h
			(h >= 16 && h < 18) || // heat up for 1h      16:00-17:59 - 2h
			(h >= 18 && h < 22)) { // heat f. e. bathroom 18:00-21:59 - 4h
		setFloorPumpWorking(true, floorPumpSetter);
	} else {
		setFloorPumpWorking(false, floorPumpSetter);
	}
}

// second algorithm 11h in all, 6 in cheap 5 in normal
void handleFloorPumpWeekDay(int h, void (*floorPumpSetter)(bool)) {
	if ((h >= 3 && h < 7) || //heat for morning bathroom  03:00-06:59 - 4h
			(h >= 13 && h < 15) || // heat using cheap p  13:00-14:59 - 2h
			(h >= 16 && h < 17) || // heat up for 1h      16:00-16:59 - 1h
			(h >= 18 && h < 22)) { // heat f. e. bathroom 18:00-21:59 - 4h
		setFloorPumpWorking(true, floorPumpSetter);
	} else {
		setFloorPumpWorking(false, floorPumpSetter);
	}
}

//first algorithm 13:00 - 21:59 = 8h && 04:00 - 06:59 = 3h- nadmierne wychłodzenie po południu
void handleFloorPump(int h, int dayOfTheWeek, void (*floorPumpSetter)(bool)) {
//start at 1pm and heat till 21:59 then break and in the morning from 4 till 6:59
	if (isWeekend(dayOfTheWeek)) {
		handleFloorPumpWeekend(h, floorPumpSetter);
	} else {
		handleFloorPumpWeekDay(h, floorPumpSetter);
	}
}

//sprawdza temperatury, pore dnia i włącza lub wyłacza graznie elektryczne
void checkAndChangeBuffor(int h, int dayOfTheWeek, void (*setMixingPump)(bool), char * (*bottomStatusPrinter)(const char *), void (*heaterSetter)(bool),
		void (*floorPumpSetter)(bool)) {
	float temp = getCurrentTemps(1); //temperatrua z ciut powyzej polowy zbiornika

	if (isDayCheapTariff(h) || isMorningTurboHeater(h)) {
		setTempHeatTo(getTempMaxAfternoon());
		setTempWaitTo(getTempMinAfternoon());
	} else if (isCheapTariff(h, dayOfTheWeek)) { // noc lub poludnie lub weekend
		setTempHeatTo(getTempMaxNight());
		setTempWaitTo(getTempMinNight());
	} else { // dzien
		setTempHeatTo(getTempMaxDay());
		setTempWaitTo(getTempMinDay());
	}

	if (vb_buforGrzeje) { // jak grzeje to spradz czy nie nagrzal
		if (temp >= getTempHeatTo()) { // nagrzal to wylacz
			wylaczGrzalki(heaterSetter);
		}
	} else { //nie grzeje
		if (temp < getTempWaitTo()) { // temperatura ponizej ustawionej  -> wlacz grzalki
			wlaczGrzalki(heaterSetter);
		}
	}

	printGrzalkiStatus(getTempHeatTo(), getTempWaitTo(), bottomStatusPrinter);
	obslugaPompyMieszajacej(temp, h, dayOfTheWeek, setMixingPump);
	handleFloorPump(h, dayOfTheWeek, floorPumpSetter);
}

void obslugaPompyMieszajacej(float temp, int h, int dayOfTheWeek, void (*setMixingPump)(bool)) {
	if (!isCheapTariff(h, dayOfTheWeek)) {
		wylaczPompaBuf(setMixingPump); //w dzien nie mieszamy
	} else {
		if (vb_buforGrzeje && (temp > getTempMixingStart())) {
			wlaczPompaBuf(setMixingPump); //grzeje i zagrzal juz CWU do gi_temperaturaStartuMieszania wiec grzej dol
		} else {
			wylaczPompaBuf(setMixingPump); // albo nie grzeje albo sie CWU wychlodzilo to nie mieszamy dalej
		}
	}
}

char * printGrzalkiStatus(int grzejDo, int czekajDo, char* (*bottomStatusPrinter)(const char *)) {
	char buf[40];
	char * status = 0;
	if (isBufforHeating()) {
		sprintf(buf, "Buf Grzeje do:%02d", grzejDo);
		status = bottomStatusPrinter(buf);
	} else {
		sprintf(buf, "Buf czeka do:%02d", czekajDo);
		status = bottomStatusPrinter(buf);
	}
	return status;
}

void wlaczPompaBuf(void (*setMixingPump)(bool)) {
	setMixingPump(true);
	vb_pompaMieszajacaPracuje = true;
}

void wylaczPompaBuf(void (*setMixingPump)(bool)) {
	setMixingPump(false);
	vb_pompaMieszajacaPracuje = false;
}

void wylaczGrzalki(void (*heaterSetter)(bool)) {
	heaterSetter(false);
	vb_buforGrzeje = false;
}

void wlaczGrzalki(void (*heaterSetter)(bool)) {
	heaterSetter(true);
	vb_buforGrzeje = true;
}

