#include "rest.h"
#include "test.h"

#include "driver2015mock.h"
#include "vents_tst.h"

#include "minunit.h"
#include <stdio.h>
#include <string.h>



const char*request2 = "POST /WW/MM HTTP/1.1\n\rhost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*request1 = "GET /ala/ola HTTP/1.1\n\rhost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
//const char*request3 = "GET /WN HTTP/1.1
//Host: 192.168.1.200:8080
//Authorization: Basic d3c6d3c=
//User-Agent: insomnia/5.0.F"
const char*request3 = "GET /WN HTTP/1.1\nHost: 192.168.1.200:8080\nAuthorization: Basic d3c6d3c=\nUser-Agent: insomnia/5.0.F";


const char*requestGetWN = "GET /WN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestGetWU = "GET /WU HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

const char*requestGetWWN = "GET /WWN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestGetWWU = "GET /WWU HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

const char*requestSetWN1 = "POST /WN/1900 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestSetWN2 = "POST /WN/100 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

const char*requestSetWU1 = "POST /WU/1900 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestSetWU2 = "POST /WU/100 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

const char*requestSetWWN1 = "POST /WWN/1500 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestSetWWU1 = "POST /WWU/1100 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

const char*requestGetTMD = "GET /TMD HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestGetTXD = "GET /TXD HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestGetTMN = "GET /TMN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestGetTXN = "GET /TXN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

const char*requestSetTMD = "POST /TMD/19 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestSetTXD = "POST /TXD/21 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestSetTMN = "POST /TMN/65 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestSetTXN = "POST /TXN/85 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

//afternoon hours temps
const char*requestGetTMA = "GET /TMA HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestGetTXA = "GET /TXA HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

const char*requestSetTMA = "POST /TMA/76 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*requestSetTXA = "POST /TXA/86 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

//YYYYMMDDHHmm
const char*requestSetTIME = "POST /CLO/202001161355 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
//const char*requestSetTIME = "POST /CLO/2020 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";


void restDebug(Request *a){
    printf("%s\n", "request parsed");
    printf("IS_ERROR:%d\n", a->isError());
    printf("ERROR:%s\n", a->getError());
    printf("RESOURCE:%s\n", a->getResource());
    printf("VALUE:%s\n", a->getValue());
    printf("METHOD:%s\n", a->getMethod());

}


 char * test_requestError() {
	 debug("test_requestError()");
    Request *a = new Request(request1);
    a->setError("TESTERROR");
    mu_assert((char*)"Set errorFlag on request failed", a->isError());
    mu_assert((char*)"Set error on request failed", !strcmp("TESTERROR", a->getError()));
    delete a;
}

 char * test_request1() {
	debug("test_request1()");
    Request *b = new Request(request1);
    mu_assert((char* )b->getError() , !b->isError() ); //
    debug("request parsed");

    mu_assert((char*)"Parsing request failed - bad method  :", !strcmp("GET", b->getMethod()));
    mu_assert((char*)"Parsing request failed - bad resource:", !strcmp("ala", b->getResource()));
    mu_assert((char*)"Parsing request failed - bad value   :", !strcmp("ola", b->getValue()));
    delete b;
}

 char * test_request2() {
	 debug("test_request2()");
    Request *a = new Request(request2);
    mu_assert((char* )a->getError() , !a->isError() ); //
    mu_assert((char*)"Parsing request failed - bad method   ", !strcmp("POST", a->getMethod()));
    mu_assert((char*)"Parsing request failed - bad resource ", !strcmp("WW", a->getResource()));
    mu_assert((char*)"Parsing request failed - bad value    ", !strcmp("MM", a->getValue()));
    delete a;
}

 char * test_request3() {
	 debug("test_request3()");
    Request *a = new Request(request3);
    mu_assert((char* )a->getError() , !a->isError() ); //
    mu_assert((char*)"Parsing request failed - bad method   ", !strcmp("GET", a->getMethod()));
    mu_assert((char*)"Parsing request failed - bad resource ", !strcmp("WN", a->getResource()));
    delete a;
}

 char * test_request_set_new_vent_speed1() {
	vents_test_init();
    mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 90 - after init", getDesiredWentRPM(NEW_WENT) == 90); //ci_defaultNewRPM =100
    Request *a =new Request(requestSetWN1);
    mu_assert((char* )a->getError() , !a->isError() ); //
    //warning the 1900 passed from rest api is 190 in system as it is /10
    mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 190 - after rest call", getDesiredWentRPM(NEW_WENT) == 190); //ci_defaultNewRPM =100

    mu_assert((char*)"Parsing request failed - bad method", !strcmp("POST", a->getMethod()));
    mu_assert((char*)"Parsing request failed - bad resource", !strcmp("WN", a->getResource()));
    mu_assert((char*)"Parsing request failed - bad value", !strcmp("1900", a->getValue()));
    delete a;
    a =new Request(requestGetWN);
    mu_assert((char*)"Parsing request failed - bad method", !strcmp("GET", a->getMethod()));
    mu_assert((char*)"Parsing request failed - bad resource", !strcmp("WN", a->getResource()));
    mu_assert((char*)"bad value", !strcmp("1900", a->getResponse()));
    delete a;

}

 char * test_request_set_new_vent_speed2() {
	vents_test_init();
    mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 90 - after init", getDesiredWentRPM(NEW_WENT) == 90); //ci_defaultNewRPM =100
    Request *a =new Request(requestSetWN2);
    mu_assert((char* )a->getError() , !a->isError() ); //
    //warning the 100 passed from rest api is 10 in system as it is /10
    mu_assert((char* )"getDesiredWentRPM(NEW_WENT) != 10 - after rest call", getDesiredWentRPM(NEW_WENT) == 10); //ci_defaultNewRPM =100
    delete a;
}



 char * test_request_set_used_vent_speed1() {
	vents_test_init();
    mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 70 - after init", getDesiredWentRPM(USED_WENT) == 70); //ci_defaultNewRPM =100
    Request *a =new Request(requestSetWU1);
    mu_assert((char* )a->getError() , !a->isError() ); //
    //warning the 100 passed from rest api is 190 in system as it is /10
    mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 190 - after rest call", getDesiredWentRPM(USED_WENT) == 190); //ci_defaultNewRPM =100
    delete a;
    a =new Request(requestGetWU);
    mu_assert((char*)"Parsing request failed - bad method", !strcmp("GET", a->getMethod()));
    mu_assert((char*)"Parsing request failed - bad resource", !strcmp("WU", a->getResource()));
    mu_assert((char*)"bad value", !strcmp("1900", a->getResponse()));
    delete a;

}
 char * test_request_set_used_vent_speed2() {
	vents_test_init();
    mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 70 - after init", getDesiredWentRPM(USED_WENT) == 70); //ci_defaultNewRPM =100
    Request *a =new Request(requestSetWU2);
    mu_assert((char* )a->getError() , !a->isError() ); //
    //warning the 100 passed from rest api is 10 in system as it is /10
    mu_assert((char* )"getDesiredWentRPM(USED_WENT) != 10 - after rest call", getDesiredWentRPM(USED_WENT) == 10); //ci_defaultNewRPM =100
    delete a;
}


 char * test_request_set_new_vent_airing_speed1() {
 	vents_test_init();
     mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT) != 100 - after init", getDesiredAiringVentRPM(NEW_WENT) == 100);
     Request *a =new Request(requestSetWWN1);
     mu_assert((char* )a->getError() , !a->isError() ); //
     mu_assert((char* )"getDesiredAiringVentRPM(NEW_WENT)  != 150 - after rest call", getDesiredAiringVentRPM(NEW_WENT) == 150); //ci_defaultNewRPM =100
     delete a;
     a =new Request(requestGetWWN);
     mu_assert((char*)"Parsing request failed - bad method", !strcmp("GET", a->getMethod()));
     mu_assert((char*)"Parsing request failed - bad resource", !strcmp("WWN", a->getResource()));
     mu_assert((char*)"bad value", !strcmp("1500", a->getResponse()));
     delete a;
 }


 char * test_request_set_used_vent_airing_speed1() {
 	vents_test_init();
     mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT) != 80 - after init", getDesiredAiringVentRPM(USED_WENT) == 80);
     Request *a =new Request(requestSetWWU1);
     mu_assert((char* )a->getError() , !a->isError() ); //
     mu_assert((char* )"getDesiredAiringVentRPM(USED_WENT)  != 110 - after rest call", getDesiredAiringVentRPM(USED_WENT) == 110); //ci_defaultNewRPM =100
     delete a;
     a =new Request(requestGetWWU);
     mu_assert((char*)"Parsing request failed - bad method", !strcmp("GET", a->getMethod()));
     mu_assert((char*)"Parsing request failed - bad resource", !strcmp("WWU", a->getResource()));
     mu_assert((char*)"bad value", !strcmp("1100", a->getResponse()));
     delete a;
 }


 char * test_request_set_get_minDayTemp() {
	 test_init();
	 Request *a =new Request(requestGetTMD);
      mu_assert((char*)"Parsing request failed - bad method", !strcmp("GET", a->getMethod()));
      mu_assert((char*)"Parsing request failed - bad resource", !strcmp("TMD", a->getResource()));
      mu_assert((char*)"bad value", !strcmp("29", a->getResponse()));
      delete a;
      a =new Request(requestSetTMD);
      delete a;
      a =new Request(requestGetTMD);
      mu_assert((char*)"bad value", !strcmp("19", a->getResponse()));
      delete a;
  }

 char * test_request_set_get_maxDayTemp() {
	 test_init();
	 Request *a =new Request(requestGetTXD);
	 mu_assert((char*)"bad value", !strcmp("31", a->getResponse()));
      delete a;
      a =new Request(requestSetTXD);
      delete a;
      a =new Request(requestGetTXD);
      mu_assert((char*)"bad value", !strcmp("21", a->getResponse()));
      delete a;
  }



 char * test_request_set_get_minNightTemp() {
	 test_init();
	 Request *a =new Request(requestGetTMN);
      mu_assert((char*)"bad value60", !strcmp("60", a->getResponse()));
      delete a;
      a =new Request(requestSetTMN);
      delete a;
      a =new Request(requestGetTMN);
      mu_assert((char*)"bad value65", !strcmp("65", a->getResponse()));
      delete a;
  }

 char * test_request_set_get_maxNightTemp() {
	 test_init();
	 Request *a =new Request(requestGetTXN);
	 mu_assert((char*)"bad value80", !strcmp("80", a->getResponse()));
      delete a;
      a =new Request(requestSetTXN);
      delete a;
      a =new Request(requestGetTXN);
      mu_assert((char*)"bad value85", !strcmp("85", a->getResponse()));
      delete a;
  }

 char * test_request_set_get_maxAfternoonTemp() {
 	 test_init();
 	 Request *a =new Request(requestGetTXA);
 	 mu_assert((char*)"bad value85", !strcmp("85", a->getResponse()));
       delete a;
       a =new Request(requestSetTXA);
       delete a;
       a =new Request(requestGetTXA);
       mu_assert((char*)"bad value86", !strcmp("86", a->getResponse()));
       delete a;
   }

 char * test_request_set_get_minAfternoonTemp() {
 	 test_init();
 	 Request *a =new Request(requestGetTMA);
 	 mu_assert((char*)"bad value85", !strcmp("75", a->getResponse()));
       delete a;
       a =new Request(requestSetTMA);
       delete a;
       a =new Request(requestGetTMA);
       mu_assert((char*)"bad value86", !strcmp("76", a->getResponse()));
       delete a;
   }

 char * test_request_set_Time() {
 	 test_init();
 	 Request *a =new Request(requestSetTIME);
 	 //mu_assert((char*)"bad time", !strcmp("75", a->getResponse()));
       delete a;
   }

 char * test_request_set_TimeUnit() {
 	 test_init();
 	 Request *a =new Request(requestSetTIME);
 	 mu_assert((char*)"parsing1", a->parseFromTo(0,2,"12") == 12);
 	 mu_assert((char*)"parsing2", a->parseFromTo(0,2,"1200") == 12);
 	 mu_assert((char*)"parsing3", a->parseFromTo(2,4,"1234") == 34);

 	 mu_assert((char*)"parsing4", a->parseHour("202001161355") == 13);
 	mu_assert((char*)"parsing4", a->parseHour("202001162555") == 12);//if wrong hour return 12

 	 mu_assert((char*)"parsing5", a->parseYear("202001161355") == 2020);
 	mu_assert((char*)"parsing5", a->parseYear("205001161355") == 2020);//if year <2020 or >2030 return 2020

 	 mu_assert((char*)"parsing6", a->parseMinute("202001161354") == 54);
 	mu_assert((char*)"parsing6", a->parseMinute("202001161364") == 0);//if wrong minute return 0

 	mu_assert((char*)"parsing7", a->parseMonth("202001161355") == 1);
 	mu_assert((char*)"parsing7", a->parseMonth("202013161355") == 1);//if wrong month return 1

 	mu_assert((char*)"parsing7", a->parseDay("202001161355") == 16);
 	mu_assert((char*)"parsing7", a->parseDay("202001321355") == 1);//if wrong day return 1

       delete a;
   }



char * rest_tests() {
	mu_run_test(test_requestError);
	mu_run_test(test_request1);
	mu_run_test(test_request2);
	mu_run_test(test_request3);
	mu_run_test(test_request_set_new_vent_speed1);
	mu_run_test(test_request_set_new_vent_speed2);
	mu_run_test(test_request_set_used_vent_speed1);
	mu_run_test(test_request_set_used_vent_speed2);
	mu_run_test(test_request_set_new_vent_airing_speed1);
	mu_run_test(test_request_set_used_vent_airing_speed1);
	mu_run_test(test_request_set_get_minDayTemp);
	mu_run_test(test_request_set_get_maxDayTemp);
	mu_run_test(test_request_set_get_minNightTemp);
	mu_run_test(test_request_set_get_maxNightTemp);
	mu_run_test(test_request_set_get_maxAfternoonTemp);
	mu_run_test(test_request_set_get_minAfternoonTemp);
	mu_run_test(test_request_set_TimeUnit);
	//mu_run_test(test_request_set_Time);
	return 0;
}


