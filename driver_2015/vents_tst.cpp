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
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != ci_defaultNewRPM - after init", getDesiredWentRPM(NEW_WENT) == ci_defaultNewRPM); //ci_defaultNewRPM =130
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != ci_defaultUsedRPM - after init", getDesiredWentRPM(USED_WENT) == ci_defaultUsedRPM); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != ci_defaultNewAiringRPM - after init", getDesiredAiringVentRPM(NEW_WENT) == ci_defaultNewAiringRPM); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != ci_defaultUsedAiringRPM - after init", getDesiredAiringVentRPM(USED_WENT) == ci_defaultUsedAiringRPM); //ci_defaultUsedAiringRPM = 100;
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
	mu_assert((char* )"getNightWentAdd()", getNightWentAdd() == ci_nightWentAdd);
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
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != ci_defaultNewRPM - after init", getDesiredWentRPM(NEW_WENT) == ci_defaultNewRPM); //ci_defaultNewRPM =130
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != ci_defaultUsedRPM - after init", getDesiredWentRPM(USED_WENT) == ci_defaultUsedRPM); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != ci_defaultNewAiringRPM - after init", getDesiredAiringVentRPM(NEW_WENT) == ci_defaultNewAiringRPM); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != ci_defaultUsedAiringRPM - after init", getDesiredAiringVentRPM(USED_WENT) == ci_defaultUsedAiringRPM); //ci_defaultUsedAiringRPM = 100;
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //

	decWentUSED(); // should go 1/10 of vent setp down as default is 200 so -20
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 70 - after dec", getDesiredWentRPM(USED_WENT) == (ci_defaultUsedRPM-20));


	initConfigWent();
	for (int i = 0; i < 2; i++)
		decWentUSED(); // should go 1/10 of vent setp down
	int expected_result1 = (ci_defaultUsedRPM-2*getWentStep()/10);
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) after 4xdecWentUSED()", (getDesiredWentRPM(USED_WENT) == expected_result1));

	incWentUSED();
	expected_result1 = expected_result1 + getWentStep() /10;
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) after incWentUSED()", getDesiredWentRPM(USED_WENT) == expected_result1);

//	initConfigWent();
//	for (int i = 0; i < 10; i++)
//		incWentUSED();
//	expected_result1 = (ci_defaultUsedRPM+10*getWentStep()/10);
//	mu_assert((char* )"getDesiredWentRPM(USED_WENT) 20xincWentUSED()", getDesiredWentRPM(USED_WENT) == expected_result1);
	return 0;
}

char * vents_test_decIncWentNEW() {
	initConfigWent();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != ci_defaultNewRPM - after init", getDesiredWentRPM(NEW_WENT) == ci_defaultNewRPM); //ci_defaultNewRPM =130
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != ci_defaultUsedRPM - after init", getDesiredWentRPM(USED_WENT) == ci_defaultUsedRPM); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != ci_defaultNewAiringRPM - after init", getDesiredAiringVentRPM(NEW_WENT) == ci_defaultNewAiringRPM); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != ci_defaultUsedAiringRPM - after init", getDesiredAiringVentRPM(USED_WENT) == ci_defaultUsedAiringRPM); //ci_defaultUsedAiringRPM = 100;
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //

	decWentNew(); // should go 1/10 of vent setp down as default is 200 so -20
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 110 - after dec", getDesiredWentRPM(NEW_WENT) == (ci_defaultNewRPM-20));

	initConfigWent();//
	for (int i = 0; i < 4; i++)
		decWentNew(); //
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) - after 4xdecWentNew()", getDesiredWentRPM(NEW_WENT) == (ci_defaultNewRPM-4*20));

	return 0;
}

char * vents_test_parseRekuperatorMSG() {
	initConfigWent();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != ci_defaultNewRPM - after init", getDesiredWentRPM(NEW_WENT) == ci_defaultNewRPM); //ci_defaultNewRPM =130
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != ci_defaultUsedRPM - after init", getDesiredWentRPM(USED_WENT) == ci_defaultUsedRPM); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != ci_defaultNewAiringRPM - after init", getDesiredAiringVentRPM(NEW_WENT) == ci_defaultNewAiringRPM); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != ci_defaultUsedAiringRPM - after init", getDesiredAiringVentRPM(USED_WENT) == ci_defaultUsedAiringRPM); //ci_defaultUsedAiringRPM = 100;
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
	gi_setWCalled = 0;
    mu_assert((char* )"setW called0 - at 22", gi_setWCalled == 0);
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != ci_defaultNewRPM - after init", getDesiredWentRPM(NEW_WENT) == ci_defaultNewRPM); //ci_defaultNewRPM =130
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != ci_defaultUsedRPM - after init", getDesiredWentRPM(USED_WENT) == ci_defaultUsedRPM); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != ci_defaultNewAiringRPM - after init", getDesiredAiringVentRPM(NEW_WENT) == ci_defaultNewAiringRPM); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != ci_defaultUsedAiringRPM - after init", getDesiredAiringVentRPM(USED_WENT) == ci_defaultUsedAiringRPM); //ci_defaultUsedAiringRPM = 100;
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //

	mu_assert((char* )"is airing - init", !isAiring());
	//if (h >= 22 || h == 4 || h == 5 || h == 13 || h == 14) { // jeżeli godzina 22 do 24 i  4 do 5 i 13 do 14
	checkWietrzenie(22, 4, emptySetW);
	mu_assert((char* )"is airing - at 22", isAiring());
	mu_assert((char* )"setW called1 - at 22", gi_setWCalled == 1);
	checkWietrzenie(22, 4, emptySetW);
	mu_assert((char* )"setW called2 - at 22", gi_setWCalled == 1); //doesn't call setw twice if already airing

	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 150 - when airing at 22", getDesiredWentRPM(NEW_WENT) == ci_defaultNewAiringRPM + ci_nightWentAdd); //not 140 because night adds 10

	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 110 - when airing at 22", getDesiredWentRPM(USED_WENT) == ci_defaultUsedAiringRPM + ci_nightWentAdd); //not 100 because night adds 10

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
void setWents(){
    emptySetW();
    printf("setWents");
}

void setWentsTST() {

}

