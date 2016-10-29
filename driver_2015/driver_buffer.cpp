/*
 * driver_buffer.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "driver_buffer.h"

boolean vb_pompaMieszajacaPracuje = false;
boolean vb_pompaMieszajacaRysuj = true;
boolean vb_pompaPodlogowaPracuje = true;
boolean vb_pompaPodlogowaRysuj = true;
boolean vb_buforGrzeje = false;

boolean isMixingPumpWorking() {
	return vb_pompaMieszajacaPracuje;
}
boolean isMixingPumpDrawing() {
	return vb_pompaMieszajacaRysuj;
}
boolean isFloorPumpWorking() {
	return vb_pompaPodlogowaPracuje;
}
boolean isFloorPumpDrawing() {
	return vb_pompaPodlogowaRysuj;
}

boolean isBufforHeating() {
	return vb_buforGrzeje;
}

void setMixingPumpDrawing(boolean value) {
	vb_pompaMieszajacaRysuj = value;
}
void setFloorPumpDrawing(boolean value) {
	vb_pompaPodlogowaRysuj = value;
}

void setupPompy() {
	pinMode(GI_PIN_POMPY_MIESZAJACEJ, OUTPUT);
	pinMode(GI_PIN_POMPY_PODLOGOWEJ, OUTPUT);
}

void setupBuf() {
	pinMode(GI_PIN_GRZALEK_1, OUTPUT);
	pinMode(GI_PIN_GRZALEK_2, OUTPUT);
	pinMode(GI_PIN_GRZALEK_3, OUTPUT);
}

void checkAndChangeBuffor() {
	float temp = getCurrentTemps(1); //temperatrua z ciut powyzej polowy zbiornika
	int grzejDo;
	int czekajDo;
	if (isNightTariff()) { // noc lub poludnie
		grzejDo = getTempMaxNight();
		czekajDo = getTempMinNight();
	} else { // dzien
		grzejDo = getTempMaxDay();
		czekajDo = getTempMinDay();
	}

	if (vb_buforGrzeje) { // jak grzeje to spradz czy nie nagrzal
		if (temp >= grzejDo) { // nagrzal to wylacz
			wylaczGrzalki();
		}
	} else { //nie grzeje
		if (temp < czekajDo) { // temperatura ponizej ustawionej  -> wlacz grzalki
			wlaczGrzalki();
		}
	}

	printGrzalkiStatus(grzejDo, czekajDo);
	obslugaPompyMieszajacej(temp);
}

void obslugaPompyMieszajacej(float temp) {
	if (!isNightTariff()) {
		wylaczPompaBuf(); //w dzien nie mieszamy
	} else {
		if (vb_buforGrzeje && (temp > getTempMixingStart())) {
			wlaczPompaBuf(); //grzeje i zagrzal juz CWU do gi_temperaturaStartuMieszania wiec grzej dol
		} else {
			wylaczPompaBuf(); // albo nie grzeje albo sie CWU wychlodzilo to nie mieszamy dalej
		}
	}
}

void printGrzalkiStatus(int grzejDo, int czekajDo) {
	char buf[40];
	if (vb_buforGrzeje) {
		sprintf(buf, "Buf Grzeje do:%02d", grzejDo);
		printBottomStatus(buf);
	} else {
		sprintf(buf, "Buf czeka do:%02d", czekajDo);
		printBottomStatus(buf);
	}
}

void wlaczPompaBuf() {
	digitalWrite(GI_PIN_POMPY_MIESZAJACEJ, LOW);
	vb_pompaMieszajacaPracuje = true;
}

void wylaczPompaBuf() {
	digitalWrite(GI_PIN_POMPY_MIESZAJACEJ, HIGH);
	vb_pompaMieszajacaPracuje = false;
}

void wylaczGrzalki() {
	ustawPinyGrzalek(HIGH);
	vb_buforGrzeje = false;
}

void wlaczGrzalki() {
	vb_buforGrzeje = true;
	ustawPinyGrzalek(LOW);
}

void ustawPinyGrzalek(uint8_t stan) {
	digitalWrite(GI_PIN_GRZALEK_1, stan);
	digitalWrite(GI_PIN_GRZALEK_2, stan);
	digitalWrite(GI_PIN_GRZALEK_3, stan);
}

