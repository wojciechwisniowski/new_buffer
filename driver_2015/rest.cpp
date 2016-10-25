#include "rest.h"

#define TEST_REST
//#define DEBUG

#ifndef TEST_REST
//#include <Print.h>
#include "driver_2015.h"
#else

#include "driver2015mock.h"

#endif

#include "minunit.h"
#include <stdio.h>
#include <string.h>

/**
 * Created by wisnia on 04.10.16.
 */

#ifdef DEBUG
#define debug(message) do { printf("%s\n", message); } while (0)
#else
#define debug(message)
#endif

int tests_run = 0;


static char * test_requestError() {
    Request a(request1);
    a.setError("TESTERROR");
    mu_assert((char*)"Set errorFlag on request failed", a.isError());
    mu_assert((char*)"Set error on request failed", !strcmp("TESTERROR", a.getError()));
}

static char * test_request1() {
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

static char * test_request2() {
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


static char * all_tests() {
    mu_run_test(test_requestError);
    mu_run_test(test_request1);
    mu_run_test(test_request2);
    return 0;
}

int main(int argc, char **argv) {
    Request a(request2);
    printf("%s\n", "request parsed");

    printf("%s\n", a.getError());
    printf("%s\n", a.getResource());
    printf("%s\n", a.getValue());
    printf("%s\n", a.getMethod());

    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;

}

//int main(int argc, char** argv) {
////	MyPrint ec;
////	ec.println("Start2");
////	parse(request, ec);
//
//
//}
//	parse(request);
//	log(request2);
//	parse(request2);
//
//	parse(requestGetWN);
//	parse(requestGetWU);
//	parse(requestGetWWN);
//	parse(requestGetWWU);
//
//	parse(requestSetWN);
//	parse(requestSetWU);
//	parse(requestSetWWN);
//	parse(requestSetWWU);
//
//	parse(requestGetTMD);
//	parse(requestGetTXD);
//	parse(requestGetTMN);
//	parse(requestGetTXN);
//
//	parse(requestSetTMD);
//	parse(requestSetTXD);
//	parse(requestSetTMN);
//	parse(requestSetTXN);
//
//}

int Request::isError() {
    return errorFlag;
}

Request::Request(const char* buf) {
    errorFlag = 0;
    method = new char[8];
    resource = new char[8];
    value = new char[8];
    auth = new char[9];
    error = new char[300];
    init(buf);
}

Request::~Request() {
    delete[] method;
    delete[] resource;
    delete[] value;
    delete[] auth;
    delete[] error;
}

char * Request::getError() {
    return error;
}

char * Request::getResource() {
    return resource;
}
char * Request::getValue() {
    return value;
}
char * Request::getMethod() {
    return method;
}

void Request::setError(const char* err) {
    errorFlag = 1;
    strcpy(error, err);
}

void Request::init(const char* buf) {
//void parse(const char* buf,const Print& ) {
    int i = 0;
    int j = 0;
    char c = buf[i++];

    // get method from request
    while (c != ' ' && i < 8) {
        method[j++] = c;
        c = buf[i++];
    }
    method[j] = '\0';
    debug("init - after method");
    //get resource from request
    c = buf[i++]; // this should be '/'
    if (c != '/')
        setError("ERR:bad request missing resource");
    debug("before res");
    if (!errorFlag) {
        j = 0;
        c = buf[i++]; // this should be 'first char of resource'
        while (c != ' ' && c != '/' && j < 8) {
            resource[j++] = c;
            c = buf[i++];
        }
        resource[j] = '\0';
        debug("after res");
        //get value from request
        if (c == '/') { // this is a start of value
            j = 0;
            c = buf[i++];
            while (c != ' ' && c != '/' && j < 8) {
                value[j++] = c;
                c = buf[i++];
            }
            value[j] = '\0';

        }
        if (c == ' ') { // no value just resource
            while (c != '\n')
                c = buf[i++]; // scroll to the next line
        }

        //host line
        c = buf[i++];//eat \r sign
        c = buf[i++];
        if (c != 'h' && c!= 'H')
            setError("Bad request missing host line:");


        if (!errorFlag) {
            while (c != '\r')
                c = buf[i++]; // scroll to the next line

            //Authorization line
            c = buf[i++];
            if (c != 'A')
                setError("Bad request missing Authorization");
            if (!errorFlag) {
                while (c != ' ')
                    c = buf[i++];
                //basic
                c = buf[i++];
                if (c != 'B')
                    setError("Bad request missing Authorization Basic required");
                if (!errorFlag) {
                    //skipp basic word
                    while (c != ' ')
                        c = buf[i++];
                    c = buf[i++];//eat  ' '
                    j = 0;
                    while (c != ' ' && c != '\n' && j < 9) {
                        auth[j++] = c;
                        c = buf[i++];
                    }
                    auth[j] = '\0';

                    if (strcmp(AUTH, auth) != 0) {
                        setError("Authorization failed");
                    }
//                    else {
//                        if (strcmp("GET", method) == 0) {
//                            if (resource[0] != '\0')
//                                debug("doGET");
//                            //doGET(resource, value);
//                        } else if (strcmp("POST", method) == 0) {
//                            if (resource[0] != '\0')
//                                debug("doPOST");
//                            //doPOST(resource, value);
//                        }
//                    }
                }
            }
        }
    }
}

void doGET(const char* resource, const char* value) {
    switch (resource[0]) {
    case 'W':
        doGETWent(resource, value);
        break;
    case 'T':
        doGETTemp(resource, value);
        break;
    }
}

void doGETWent(const char* resource, const char* value) {
    if (strcmp(WN, resource) == 0) {
        getWN();
    } else if (strcmp(WU, resource) == 0) {
        getWU();
    } else if (strcmp(WWN, resource) == 0) {
        getWWN();
    } else if (strcmp(WWU, resource) == 0) {
        getWWU();
    }
}

void doGETTemp(const char* resource, const char* value) {
    if (strcmp(TMD, resource) == 0) {
        getTMD();
    } else if (strcmp(TXD, resource) == 0) {
        getTXD();
    } else if (strcmp(TMN, resource) == 0) {
        getTMN();
    } else if (strcmp(TXN, resource) == 0) {
        getTXN();
    }
}

void doPOST(const char* resource, const char* value) {
    switch (resource[0]) {
    case 'W':
        doPOSTWent(resource, value);
        break;
    case 'T':
        doPOSTTemp(resource, value);
        break;
    }
}

void doPOSTTemp(const char* resource, const char* value) {
    if (strcmp(TMD, resource) == 0) {
        setTMD(value);
    } else if (strcmp(TXD, resource) == 0) {
        setTXD(value);
    } else if (strcmp(TMN, resource) == 0) {
        setTMN(value);
    } else if (strcmp(TXN, resource) == 0) {
        setTXN(value);
    }
}

void doPOSTWent(const char* resource, const char* value) {
    if (strcmp(WN, resource) == 0) {
        setWN(value);
    } else if (strcmp(WU, resource) == 0) {
        setWU(value);
    } else if (strcmp(WWN, resource) == 0) {
        setWWN(value);
    } else if (strcmp(WWU, resource) == 0) {
        setWWU(value);
    }
}

