/*
 * termometry.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "termometry.h"
#include "prod.h"

float gf_currentTemps[TEMPCOUNT];
int gi_EE_Temp_Min_Day;
int gi_EE_Temp_Max_Day;
int gi_EE_Temp_Min_Night;
int gi_EE_Temp_Max_Night;
int gi_EE_Temp_Mixing_Start;

int gi_Temp_Min_Day;
int gi_Temp_Max_Day;
int gi_Temp_Min_Night;
int gi_Temp_Max_Night;
int gi_Temp_Mixing_Start;
int gi_Temp_waitTo = 49;
int gi_Temp_heatTo = 50;

const int ci_minTemp = 5;
const int ci_maxTemp = 95;

const int ci_minDayTempDefault = 29;
const int ci_maxDayTempDefault = 31;
const int ci_minNightTempDefault = 60;
const int ci_maxNightTempDefault = 80;
const int ci_mixingPumpStartTempDefault = 55;

void setTempWaitTo(int t){
	gi_Temp_waitTo = t;
}

int getTempWaitTo(){
	return gi_Temp_waitTo;
}

void setTempHeatTo(int t){
	gi_Temp_heatTo = t;
}
int getTempHeatTo(){
	return gi_Temp_heatTo;
}

int getTempMinDay() {
	return gi_Temp_Min_Day;
}
int getTempMaxDay() {
	return gi_Temp_Max_Day;
}
int getTempMinNight() {
	return gi_Temp_Min_Night;
}
int getTempMaxNight() {
	return gi_Temp_Max_Night;
}
int getTempMixingStart() {
	return gi_Temp_Mixing_Start;
}

float getCurrentTemps(int i) {
	return gf_currentTemps[i];
}

void setCurrentTemps(int i, float value) {
	gf_currentTemps[i] = value;
}

//wczytaj ustawienia temperatur z EE lub ustaw default
void initConfigTemp() {
	int vi_t = readTemFromEprom((int*) (&gi_EE_Temp_Min_Day));
	if (checkTemp(vi_t)) {
		setMinDayTemp(vi_t);
	} else {
		setMinDayTemp(ci_minDayTempDefault);
	}

	vi_t = readTemFromEprom((int*) (&gi_EE_Temp_Max_Day));
	if (checkTemp(vi_t)) {
		setMaxDayTemp(vi_t);
	} else {
		setMaxDayTemp(ci_maxDayTempDefault);
	}

	vi_t = readTemFromEprom((int*) (&gi_EE_Temp_Min_Night));
	if (checkTemp(vi_t)) {
		setMinNightTemp(vi_t);
	} else {
		setMinNightTemp(ci_minNightTempDefault);
	}
	vi_t = readTemFromEprom((int*) (&gi_EE_Temp_Max_Night));
	if (checkTemp(vi_t)) {
		setMaxNightTemp(vi_t);
	} else {
		setMaxNightTemp(ci_maxNightTempDefault);
	}
	vi_t = readTemFromEprom((int*) (&gi_EE_Temp_Mixing_Start));
	if (checkTemp(vi_t)) {
		setMixingPumpStartTemp(vi_t);
	} else {
		setMixingPumpStartTemp(ci_mixingPumpStartTempDefault);
	}
}

bool checkTemp(int temp) {
	return temp >= ci_minTemp && temp <= ci_maxTemp;
}

void setMinDayTemp(int temp) {
	if (checkTemp(temp)) {
		gi_Temp_Min_Day = temp;
		writeTemToEprom(&gi_EE_Temp_Min_Day, temp);
	}
}

void setMaxDayTemp(int temp) {
	if (checkTemp(temp)) {
		gi_Temp_Max_Day = temp;
		writeTemToEprom(&gi_EE_Temp_Max_Day, temp);
	}
}

void setMinNightTemp(int temp) {
	if (checkTemp(temp)) {
		gi_Temp_Min_Night = temp;
		writeTemToEprom(&gi_EE_Temp_Min_Night, temp);
	}
}

void setMaxNightTemp(int temp) {
	if (checkTemp(temp)) {
		gi_Temp_Max_Night = temp;
		writeTemToEprom(&gi_EE_Temp_Max_Night, temp);
	}
}

void setMixingPumpStartTemp(int temp) {
	if (checkTemp(temp)) {
		gi_Temp_Mixing_Start = temp;
		writeTemToEprom(&gi_EE_Temp_Mixing_Start, temp);
	}
}

void decMinDzienna() {
	setMinDayTemp(getTempMinDay() - 1);
}
void incMinDzienna() {
	setMinDayTemp(getTempMinDay() + 1);
}
void decMaxDzienna() {
	setMaxDayTemp(getTempMaxDay() - 1);
}
void incMaxDzienna() {
	setMaxDayTemp(getTempMaxDay() + 1);
}
void decMinNocna() {
	setMinNightTemp(getTempMinNight() - 1);
}
void incMinNocna() {
	setMinNightTemp(getTempMinNight() + 1);
}
void decMaxNocna() {
	setMaxNightTemp(getTempMaxNight() - 1);
}
void incMaxNocna() {
	setMaxNightTemp(getTempMaxNight() + 1);
}
void decPompaMiesz() {
	setMixingPumpStartTemp(getTempMixingStart() - 1);

}
void incPompaMiesz() {
	setMixingPumpStartTemp(getTempMixingStart() + 1);
}

