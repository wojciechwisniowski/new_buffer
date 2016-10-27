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

boolean isBufforHeating() {
	return vb_buforGrzeje;
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
	float temp = gf_currentTemps[1]; //temperatrua z ciut powyzej polowy zbiornika
	int grzejDo = 0;
	int czekajDo = 0;
	if (isNightTariff()) { //noc lub południe
		grzejDo = gi_Temp_Max_Night;
		czekajDo = gi_Temp_Min_Night;
		if (vb_buforGrzeje) {
			if (temp >= gi_Temp_Max_Night) {
				wylaczGrzalki();
				vb_buforGrzeje = false;
			}
		} else { //nie grzeje
			if (temp < gi_Temp_Min_Night) {
				vb_buforGrzeje = true;
				wlaczGrzalki();
			}
		}
	} else { //dzień
		grzejDo = gi_Temp_Max_Day;
		czekajDo = gi_Temp_Min_Day;
		if (vb_buforGrzeje) {
			if (temp >= gi_Temp_Max_Day) {
				wylaczGrzalki();
				vb_buforGrzeje = false;
			}
		} else { //nie grzeje
			if (temp < gi_Temp_Min_Day) {
				vb_buforGrzeje = true;
				wlaczGrzalki();
			}
		}
	}
	printGrzalkiStatus(grzejDo, czekajDo);
	obslugaPompyMieszajacej(temp);
}

void obslugaPompyMieszajacej(float temp) {
	if (!isNightTariff()) {
		wylaczPompaBuf(); //w dzien nie mieszamy
	} else {
		if (vb_buforGrzeje && (temp > gi_Temp_Mixing_Start)) {
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
}

void wlaczGrzalki() {
	ustawPinyGrzalek(LOW);
}

void ustawPinyGrzalek(uint8_t stan) {
	digitalWrite(GI_PIN_GRZALEK_1, stan);
	digitalWrite(GI_PIN_GRZALEK_2, stan);
	digitalWrite(GI_PIN_GRZALEK_3, stan);
}

