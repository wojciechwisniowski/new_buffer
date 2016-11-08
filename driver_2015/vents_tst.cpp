/*
 * vents_tst.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: wisnia
 */
#include "minunit.h"
#include "vents_tst.h"
#include <string.h>

char * vents_test_defaults(){
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 100 - after init", getDesiredWentRPM(NEW_WENT) == 100); //ci_defaultNewRPM =100
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 90 - after init", getDesiredWentRPM(USED_WENT) == 90); //ci_defaultUsedRPM = 90;
	mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != 140 - after init", getDesiredAiringVentRPM(USED_WENT) == 140); //ci_defaultUsedAiringRPM = 140;
	mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != 160 - after init", getDesiredAiringVentRPM(NEW_WENT) == 160); //ci_defaultUsedAiringRPM = 160;
	mu_assert((char* )"getWentStep() != 200 - after init", getWentStep() == 200); //

}

char * vents_test_init() {
	initConfigWent();
	vents_test_defaults();
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
	mu_assert((char* )"getNightWentAdd()", getNightWentAdd() == 100);
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
	vents_test_defaults();
	decWentUSED();// should go 1/10 of vent setp down as default is 200 so -20
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 70 - after dec", getDesiredWentRPM(USED_WENT) == 70);
	for(int i=0;i<10;i++)decWentUSED();// should go 1/10 of vent setp down as default is 200 so -20 * 50 -100

	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 10 - after 10xdecWentUSED()", getDesiredWentRPM(USED_WENT) == 10);
	incWentUSED();
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 30 - after incWentUSED()", getDesiredWentRPM(USED_WENT) == 30);
	for(int i=0;i<20;i++)incWentUSED();
	mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 265 - after 20xincWentUSED()", getDesiredWentRPM(USED_WENT) == 265);
	return 0;
}

char * vents_test_decIncWentNEW() {
	initConfigWent();
	vents_test_defaults();
	decWentNew();// should go 1/10 of vent setp down as default is 200 so -20
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 80 - after dec", getDesiredWentRPM(NEW_WENT) == 80);
	for(int i=0;i<10;i++)decWentNew();// should go 1/10 of vent setp down as default is 200 so -20 * 50 -100

	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 20 - after 10xdecWentNew()", getDesiredWentRPM(NEW_WENT) == 20);
	incWentNew();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 40 - after incWentNew()", getDesiredWentRPM(NEW_WENT) == 40);
	for(int i=0;i<20;i++)incWentNew();
	mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 270 - after 20xincWentNEW()", getDesiredWentRPM(NEW_WENT) == 270);
	return 0;
}





char * vents_tests() {
	mu_run_test(vents_test_init);
	mu_run_test(vents_test_checkRPM);
	mu_run_test(vents_test_getNightWentAdd);
	mu_run_test(vents_test_changeWentStep);
	mu_run_test(vents_test_decIncWentUSED);
	mu_run_test(vents_test_decIncWentNEW);
	return 0;
}
//void getWentString(char * buf) {
//	strcpy(buf,"12345678932126002700");
//	//return in buf "12345678932126002700";
//}

void setWentsTST() {

}

