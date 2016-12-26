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

//first algorithm
void handleFloorPump(int h, int dayOfTheWeek, void (*floorPumpSetter)(bool)) {
//start at 1pm and heat till 21:59 then break and in the morning from 4 till 6:59
	if (h >= getHourDayStart() && h < 22) {
		setFloorPumpWorking(true, floorPumpSetter);
	} else if (h >= 4 && h < 7) {
		setFloorPumpWorking(true, floorPumpSetter);
	} else {
		setFloorPumpWorking(false, floorPumpSetter);
	}
}

//sprawdza temperatury, pore dnia i włącza lub wyłacza graznie elektryczne
void checkAndChangeBuffor(int h, int dayOfTheWeek, void (*setMixingPump)(bool), char * (*bottomStatusPrinter)(const char *), void (*heaterSetter)(bool),
		void (*floorPumpSetter)(bool)) {
	float temp = getCurrentTemps(1); //temperatrua z ciut powyzej polowy zbiornika
	int grzejDo;
	int czekajDo;
	if (isCheapTariff(h, dayOfTheWeek)) { // noc lub poludnie lub weekend
		grzejDo = getTempMaxNight();
		czekajDo = getTempMinNight();
	} else { // dzien
		grzejDo = getTempMaxDay();
		czekajDo = getTempMinDay();
	}

	if (vb_buforGrzeje) { // jak grzeje to spradz czy nie nagrzal
		if (temp >= grzejDo) { // nagrzal to wylacz
			wylaczGrzalki(heaterSetter);
		}
	} else { //nie grzeje
		if (temp < czekajDo) { // temperatura ponizej ustawionej  -> wlacz grzalki
			wlaczGrzalki(heaterSetter);
		}
	}

	printGrzalkiStatus(grzejDo, czekajDo, bottomStatusPrinter);
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

