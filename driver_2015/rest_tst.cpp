#include "rest.h"
#include "test.h"

#include "driver2015mock.h"

#include "minunit.h"
#include <stdio.h>
#include <string.h>



const char*request2 = "POST /WW/MM HTTP/1.1\n\rhost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*request1 = "GET /ala/ola HTTP/1.1\n\rhost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

const char*requestGetWN = "GET /WN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetWU = "GET /WU HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetWWN = "GET /WWN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetWWU = "GET /WWU HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

const char*requestSetWN = "POST /WN/1 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetWU = "POST /WU/22 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetWWN = "POST /WWN/333 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetWWU = "POST /WWU/4444 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

const char*requestGetTMD = "GET /TMD HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetTXD = "GET /TXD HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetTMN = "GET /TMN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetTXN = "GET /TXN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

const char*requestSetTMD = "POST /TMD/29 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetTXD = "POST /TXD/35 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetTMN = "POST /TMN/60 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetTXN = "POST /TXN/80 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

 char * test_requestError() {
    Request a(request1);
    a.setError("TESTERROR");
    mu_assert((char*)"Set errorFlag on request failed", a.isError());
    mu_assert((char*)"Set error on request failed", !strcmp("TESTERROR", a.getError()));
}

 char * test_request1() {
    Request a(request1);
    debug("request parsed");

    debug(a.getError());
    debug(a.getResource());
    debug(a.getValue());
    debug(a.getMethod());


    mu_assert((char*)"Parsing request failed - bad method", !strcmp("GET", a.getMethod()));
    mu_assert((char*)"Parsing request failed - bad resource", !strcmp("ala", a.getResource()));
    mu_assert((char*)"Parsing request failed - bad value", !strcmp("ola", a.getValue()));

}

 char * test_request2() {
    Request a(request2);
    debug("request parsed");

    debug(a.getError());
    debug(a.getResource());
    debug(a.getValue());
    debug(a.getMethod());

    mu_assert((char*)"Parsing request failed - bad method", !strcmp("POST", a.getMethod()));
    mu_assert((char*)"Parsing request failed - bad resource", !strcmp("WW", a.getResource()));
    mu_assert((char*)"Parsing request failed - bad value", !strcmp("MM", a.getValue()));

}

void restDebug(){
    Request a(request2);
    printf("%s\n", "request parsed");

    printf("%s\n", a.getError());
    printf("%s\n", a.getResource());
    printf("%s\n", a.getValue());
    printf("%s\n", a.getMethod());

}

char * rest_tests() {
	mu_run_test(test_requestError);
	mu_run_test(test_request1);
	mu_run_test(test_request2);
	return 0;
}


