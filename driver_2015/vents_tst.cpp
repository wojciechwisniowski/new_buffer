/*
 * vents_tst.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: wisnia
 */
#include "minunit.h"
#include "vents_tst.h"
#include <string.h>

char * vents_test_init() {
	initConfigWent();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 90 - after init", getDesiredWentRPM(NEW_WENT) == 90); //ci_defaultNewRPM =100
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 70 - after init", getDesiredWentRPM(USED_WENT) == 70); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != 100 - after init", getDesiredAiringVentRPM(NEW_WENT) == 100); //ci_defaultUsedAiringRPM = 160;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != 80 - after init", getDesiredAiringVentRPM(USED_WENT) == 80); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //

	return 0;
}

char * vents_test_checkRPM() {
	mu_assert((char* )"checkRPM(-1)", !checkRPM(-1));
	mu_assert((char* )"checkRPM(0)", checkRPM(0));
	mu_assert((char* )"checkRPM(1)", checkRPM(1));
	mu_assert((char* )"checkRPM(259)", checkRPM(259));
	mu_assert((char* )"checkRPM(260)", checkRPM(260));
	mu_assert((char* )"checkRPM(261)", !checkRPM(261));
	mu_assert((char* )"checkRPM(255)", !checkRPM(255));
	return 0;
}
char * vents_test_getNightWentAdd() {
	mu_assert((char* )"getNightWentAdd()", getNightWentAdd() == 10);
	return 0;
}
char * vents_test_changeWentStep() {
	initConfigWent();
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //
	changeWentStep();
	mu_assert((char* )"getWentStep() != 500 - after changeWentStep", getWentStep() == 500); //
	changeWentStep();
	mu_assert((char* )"getWentStep() != 100 - after changeWentStep", getWentStep() == 100); //
	changeWentStep();
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //
	return 0;
}

char * vents_test_decIncWentUSED() {
	initConfigWent();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 90 - after init", getDesiredWentRPM(NEW_WENT) == 90); //ci_defaultNewRPM =100
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 70 - after init", getDesiredWentRPM(USED_WENT) == 70); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != 100 - after init", getDesiredAiringVentRPM(NEW_WENT) == 100); //ci_defaultUsedAiringRPM = 160;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != 80 - after init", getDesiredAiringVentRPM(USED_WENT) == 80); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //

	decWentUSED(); // should go 1/10 of vent setp down as default is 200 so -20
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 60 - after dec", getDesiredWentRPM(USED_WENT) == (70-20));
	initConfigWent();
	for (int i = 0; i < 3; i++)
		decWentUSED(); // should go 1/10 of vent setp down as default is 200 so -20 *  =60

	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 10 - after 4xdecWentUSED()", getDesiredWentRPM(USED_WENT) == (70-60));
	incWentUSED();
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 30 - after incWentUSED()", getDesiredWentRPM(USED_WENT) == 30);
	for (int i = 0; i < 20; i++)
		incWentUSED();
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 265 - after 20xincWentUSED()", getDesiredWentRPM(USED_WENT) == 250);
	return 0;
}

char * vents_test_decIncWentNEW() {
	initConfigWent();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 90 - after init", getDesiredWentRPM(NEW_WENT) == 90); //ci_defaultNewRPM =100
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 70 - after init", getDesiredWentRPM(USED_WENT) == 70); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != 100 - after init", getDesiredAiringVentRPM(NEW_WENT) == 100); //ci_defaultUsedAiringRPM = 160;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != 80 - after init", getDesiredAiringVentRPM(USED_WENT) == 80); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //

	decWentNew(); // should go 1/10 of vent setp down as default is 200 so -20
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 70 - after dec", getDesiredWentRPM(NEW_WENT) == (90-20));
	for (int i = 0; i < 4; i++)
		decWentNew(); // should go 1/10 of vent setp down as default is 200 so -20 * 4 = 80

	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 10 - after 10xdecWentNew()", getDesiredWentRPM(NEW_WENT) == (90-80));
	incWentNew();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 30 - after incWentNew()", getDesiredWentRPM(NEW_WENT) == 30);
	for (int i = 0; i < 25; i++)
		incWentNew();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 250 - after 25xincWentNEW()", getDesiredWentRPM(NEW_WENT) == 250);
	return 0;
}

char * vents_test_parseRekuperatorMSG() {
	initConfigWent();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 90 - after init", getDesiredWentRPM(NEW_WENT) == 90); //ci_defaultNewRPM =100
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 70 - after init", getDesiredWentRPM(USED_WENT) == 70); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != 100 - after init", getDesiredAiringVentRPM(NEW_WENT) == 100); //ci_defaultUsedAiringRPM = 160;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != 80 - after init", getDesiredAiringVentRPM(USED_WENT) == 80); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //

	parseRekuperatorMSG("12045078032026002700");
//	printf("NEW_IN:%f\n", getCurrentTemReku(NEW_IN));
//	printf("NEW_OUT :%f\n", getCurrentTemReku(NEW_OUT));
//	printf("USED_IN:%f\n", getCurrentTemReku(USED_IN));
//	printf("USED_OUT:%f\n", getCurrentTemReku(USED_OUT));

	mu_assert((char* )"getCurrentTemReku(NEW_IN) - parse", getCurrentTemReku(NEW_IN) == 12.0f);
	mu_assert((char* )"getCurrentTemReku(NEW_OUT) - parse", getCurrentTemReku(NEW_OUT) == 45.0f);
	mu_assert((char* )"getCurrentTemReku(USED_IN) - parse", getCurrentTemReku(USED_IN) == 78.0f);
	mu_assert((char* )"getCurrentTemReku(USED_OUT) - parse", getCurrentTemReku(USED_OUT) == 32.0f);
	mu_assert((char* )"getCurrentVentRPM(NEW_WENT) - parse", getCurrentVentRPM(NEW_WENT) == 2600);
	mu_assert((char* )"getCurrentVentRPM(USED_WENT) - parse", getCurrentVentRPM(USED_WENT) == 2700);
	return 0;
}

int gi_setWCalled = 0;
void emptySetW() {
	gi_setWCalled++;
}

char * vents_test_checkWietrzenie() {
	initConfigWent();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 90 - after init", getDesiredWentRPM(NEW_WENT) == 90); //ci_defaultNewRPM =100
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 70 - after init", getDesiredWentRPM(USED_WENT) == 70); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != 100 - after init", getDesiredAiringVentRPM(NEW_WENT) == 100); //ci_defaultUsedAiringRPM = 160;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != 80 - after init", getDesiredAiringVentRPM(USED_WENT) == 80); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //

	mu_assert((char* )"is airing - init", !isAiring());
	//if (h >= 22 || h == 4 || h == 5 || h == 13 || h == 14) { // jeżeli godzina 22 do 24 i  4 do 5 i 13 do 14
	checkWietrzenie(22, 4, emptySetW);
	mu_assert((char* )"is airing - at 22", isAiring());
	mu_assert((char* )"setW called - at 22", gi_setWCalled == 1);
	checkWietrzenie(22, 4, emptySetW);
	mu_assert((char* )"setW called - at 22", gi_setWCalled == 1); //doesn't call setw twice if already airing

	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 110 - when airing at 22", getDesiredWentRPM(NEW_WENT) == 110); //not 160 because night adds 10
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 90 - when airing at 22", getDesiredWentRPM(USED_WENT) == 90); //not 140 because night adds 10

	return 0;
}

char * vents_tests() {
	mu_run_test(vents_test_init);
	mu_run_test(vents_test_checkRPM);
	mu_run_test(vents_test_getNightWentAdd);
	mu_run_test(vents_test_changeWentStep);
	mu_run_test(vents_test_decIncWentUSED);
	mu_run_test(vents_test_decIncWentNEW);
	mu_run_test(vents_test_parseRekuperatorMSG);
	mu_run_test(vents_test_checkWietrzenie);
	return 0;
}
//void getWentString(char * buf) {
//	strcpy(buf,"12345678932126002700");
//	//return in buf "12345678932126002700";
//}

void setWentsTST() {

}

