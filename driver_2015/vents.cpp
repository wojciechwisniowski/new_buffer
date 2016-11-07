/*
 * vents.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */
#include "vents.h"
#include "vents_prod.h"
#include "prod.h"

int gi_EE_Vent_Used_Airing_RPM; //obroty dla used dla wietrzenia (zakodowane na 8 bitach czyli obroty to wartość * 10)
int gi_EE_Vent_New_Airing_RPM; //obroty dla new dla wietrzenia (zakodowane na 8 bitach czyli obroty to wartość * 10)
int gi_EE_Vent_New_Desired_RPM; //obroty dla pozostalego czasu dla new (zakodowane na 8 bitach czyli obroty to wartość * 10)
int gi_EE_Vent_Used_Desired_RPM; //obroty dla pozostalego czasu dla used (zakodowane na 8 bitach czyli obroty to wartość * 10)

const int gi_nightWentAdd = 100; //w taryfie nocnej obroty wentylatorów wieksze o

int gi_currentWentRPM[WENTCOUNT]; //0 new 1 used
int gi_desiredWentRPM[WENTCOUNT]; //0 new 1 used
int gi_desiredOldWentRPM[WENTCOUNT]; //0 new 1 used
int gi_desiredWentWietrzenieRPM[WENTCOUNT]; //0 new 1 used
float gf_currentTemps_reku[TEMPCOUNT_REKU];

int gi_wentStep;

bool vb_wietrzenie = false;

int getNightWentAdd() {
	return gi_nightWentAdd;
}

void changeWentStep() {
	if (gi_wentStep == WENT_STEP1)
		gi_wentStep = WENT_STEP2;
	else if (gi_wentStep == WENT_STEP2)
		gi_wentStep = WENT_STEP3;
	else if (gi_wentStep == WENT_STEP3)
		gi_wentStep = WENT_STEP1;
}

void decWentNew() {
	decWent(NEW_WENT, NEW_WENT_MAX);
	writeTemToEprom(&gi_EE_Vent_New_Airing_RPM, gi_desiredWentRPM[NEW_WENT]);
}

void decWentUSED() {
	decWent(USED_WENT, USED_WENT_MAX);
	writeTemToEprom(&gi_EE_Vent_Used_Airing_RPM, gi_desiredWentRPM[USED_WENT]);
}

//TODO refactor
void incWentNew() {
	incWent(NEW_WENT, NEW_WENT_MAX);
	writeTemToEprom(&gi_EE_Vent_New_Airing_RPM, gi_desiredWentRPM[NEW_WENT]);
}

void incWentUSED() {
	incWent(USED_WENT, USED_WENT_MAX);
	writeTemToEprom(&gi_EE_Vent_Used_Airing_RPM, gi_desiredWentRPM[USED_WENT]);
}

void startWietrzenie(void (* setW)()) {
	if (!vb_wietrzenie) {
		gi_desiredOldWentRPM[USED_WENT] = gi_desiredWentRPM[USED_WENT];
		gi_desiredOldWentRPM[NEW_WENT] = gi_desiredWentRPM[NEW_WENT];

		gi_desiredWentRPM[USED_WENT] = gi_desiredWentWietrzenieRPM[USED_WENT];
		gi_desiredWentRPM[NEW_WENT] = gi_desiredWentWietrzenieRPM[NEW_WENT];
		setW();  // wyslij
		vb_wietrzenie = true;
	}
}

void stopWietrzenie(void (* setW)()) {
	if (vb_wietrzenie) {
		gi_desiredWentRPM[USED_WENT] = gi_desiredOldWentRPM[USED_WENT];
		gi_desiredWentRPM[NEW_WENT] = gi_desiredOldWentRPM[NEW_WENT];
		setW();  // wyslij
		vb_wietrzenie = false;
	}
}

void checkWietrzenie(int h,  void (* setW)()) {

	if (h >= 22 || h == 4 || h == 5 || h == 13 || h == 14) { // jeżeli godzina 22 do 24 i  4 do 5 i 13 do 14
		if (!vb_wietrzenie) {
			startWietrzenie(setW);
		}
	} else {  //godziny inne
		if (vb_wietrzenie) {
			stopWietrzenie(setW);
		}
	}
	//else nie wietrz
}

//wczytaj ustawienia wentylacji z EE lub wstaw default
void initConfigWent() {
	gi_desiredWentWietrzenieRPM[USED_WENT] = readTemFromEprom((int *) &gi_EE_Vent_Used_Airing_RPM);
	if (gi_desiredWentWietrzenieRPM[USED_WENT] == 255) {
		gi_desiredWentWietrzenieRPM[USED_WENT] = 160;         //1600 w nocy 1800
	}

	gi_desiredWentWietrzenieRPM[NEW_WENT] = readTemFromEprom((int *) &gi_EE_Vent_New_Airing_RPM);
	if (gi_desiredWentWietrzenieRPM[NEW_WENT] == 255) {
		gi_desiredWentWietrzenieRPM[NEW_WENT] = 170;          //1700 w nocy 1900
	}

	gi_desiredWentRPM[NEW_WENT] = readTemFromEprom((int *) &gi_EE_Vent_New_Desired_RPM);
	if (gi_desiredWentRPM[NEW_WENT] == 255) {
		gi_desiredWentRPM[NEW_WENT] = 120;                //1200
	}

	gi_desiredWentRPM[USED_WENT] = readTemFromEprom((int *) &gi_EE_Vent_Used_Desired_RPM);
	if (gi_desiredWentRPM[USED_WENT] == 255) {
		gi_desiredWentRPM[USED_WENT] = 110;                //1100
	}

	gi_wentStep = WENT_STEP2;

}

////new_in new_out used_in used_out new_went used_went
//void parseRekuperatorMSG(String msg) {
//	String t0 = msg.substring(0, 3);
//	String t1 = msg.substring(3, 6);
//	String t2 = msg.substring(6, 9);
//	String t3 = msg.substring(9, 12);
//	String rpm1 = msg.substring(12, 16);
//	String rpm2 = msg.substring(16, 20);
//	gf_currentTemps_reku[NEW_IN] = t0.toFloat() / 10.0;
//	gf_currentTemps_reku[NEW_OUT] = t1.toFloat() / 10.0;
//	gf_currentTemps_reku[USED_IN] = t2.toFloat() / 10.0;
//	gf_currentTemps_reku[USED_OUT] = t3.toFloat() / 10.0;
//	gi_currentWentRPM[NEW_WENT] = rpm1.toInt();
//	gi_currentWentRPM[USED_WENT] = rpm2.toInt();
//}

//new_in new_out used_in used_out new_went used_went
void parseRekuperatorMSG(const char* msg) {
	char buf[5];
	int j = 0;
	for (int i = 0; i < 3; i++)//0-3
		buf[i] = msg[j++];
	buf[3] = '\0';
	gf_currentTemps_reku[NEW_IN] = atof(buf) / 10.0;

	for (int i = 0; i < 3; i++)//3-6
		buf[i] = msg[j++];
	buf[3] = '\0';
	gf_currentTemps_reku[NEW_OUT] = atof(buf) / 10.0;

	for (int i = 0; i < 3; i++)//6-9
		buf[i] = msg[j++];
	buf[3] = '\0';
	gf_currentTemps_reku[USED_IN] = atof(buf) / 10.0;

	for (int i = 0; i < 3; i++)//9-12
		buf[i] = msg[j++];
	buf[3] = '\0';
	gf_currentTemps_reku[USED_OUT] = atof(buf) / 10.0;

	for (int i = 0; i < 4; i++)//12-16
		buf[i] = msg[j++];
	buf[4] = '\0';
	gi_currentWentRPM[NEW_WENT] = atoi(buf);

	for (int i = 0; i < 4; i++)//16-20
		buf[i] = msg[j++];
	buf[4] = '\0';
	gi_currentWentRPM[USED_WENT] = atof(buf);
}

void incWent(int went, int max) {
	int old = gi_desiredWentRPM[went];
	gi_desiredWentRPM[went] = old + (gi_wentStep / 10);
	if (gi_desiredWentRPM[went] > max)
		gi_desiredWentRPM[went] = max;

}

void decWent(int went, int max) {
	int old = gi_desiredWentRPM[went];
	if (old > (gi_wentStep / 10))
		gi_desiredWentRPM[went] = old - (gi_wentStep / 10);
}

float getCurrentTemReku(int nr) {
	return gf_currentTemps_reku[nr];
}

int getCurrentVentRPM(int nr) {
	return gi_currentWentRPM[nr];
}

int getDesiredWentRPM(int nr) {
	return gi_desiredWentRPM[nr];
}

int getWentStep() {
	return gi_wentStep;
}

bool isAiring() {
	return vb_wietrzenie;
}
