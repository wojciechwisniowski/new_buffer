#include "rest.h"

#include "driver2015mock.h"
#include "vents.h"

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

const char* WN = "WN"; // went new
const char* WU = "WU"; // went used
const char* WWN = "WWN"; // went airing new
const char* WWU = "WWU"; // went airing used

const char* TMD = "TMD"; // temp min day
const char* TXD = "TXD"; // temp max day

const char* TMN = "TMN"; // temp min night
const char* TXN = "TXN"; // temp max night
const char* AUTH = "d3c6d3c="; //ww:ww

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
	response = new char[100];
	response[0] = '\0';
	for (int i = 0; i < 100; i++)
		error[i] = 0;
	init(buf);
}

Request::~Request() {
	delete[] method;
	delete[] resource;
	delete[] value;
	delete[] auth;
	delete[] error;
	delete[] response;

	debug("Request destructor done");
}

char * Request::getResponse() {
	return response;
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

int Request::parseMethod(int i, const char* buf) {
	char c = buf[i++];
	int j = 0;
	// get method from request
	while (c != ' ' && i < 8) {
		method[j++] = c;
		c = buf[i++];
	}
	method[j] = '\0';
	return i;
}

char Request::parseResource(char c, const char* buf, int& i) {
	int j = 0;
	c = buf[i++]; // this should be 'first char of resource'
	while (c != ' ' && c != '/' && j < 8) {
		resource[j++] = c;
		c = buf[i++];
	}
	resource[j] = '\0';

	return c;
}

int Request::parseValue(char c, const char* buf, int i) {
	//get value from request
	if (c == '/') {
		// this is a start of value
		int j = 0;
		c = buf[i++];
		while (c != ' ' && c != '/' && j < 8) {
			value[j++] = c;
			c = buf[i++];
		}
		value[j] = '\0';
	}
	if (c == ' ') {
		// no value just resource
		while (c != '\n')
			c = buf[i++]; // scroll to the next line
	}
	return i;
}

void Request::init(const char* buf) {
//void parse(const char* buf,const Print& ) {
	int i = 0;
	int j = 0;
	char c;
	i = parseMethod(i, buf);
	debug("init - after method");

	//get resource from request
	c = buf[i++]; // this should be '/'
	if (c != '/')
		setError("ERR:bad request missing resource");
	debug("before res");
	if (!errorFlag) {
		c = parseResource(c, buf, i);
		debug("after res");

		//get value from request
		i = parseValue(c, buf, i);

		//find host line
		int guard = 0;
		while (c != 'h' && c != 'H' && guard < 5) {
			c = buf[i++];
			guard++;
		}
		if (c != 'h' && c != 'H')
			setError("Bad request missing host line:");

		if (!errorFlag) {
			while (c != '\n')
				c = buf[i++]; // scroll to the next line

			//Authorization line
			guard = 0;
			while (c != 'A' && guard < 5) {
				c = buf[i++];
				guard++;
			}
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
					c = buf[i++]; //eat  ' '
					j = 0;
					while (c != ' ' && c != '\n' && j < 9 && c!='\r') {
						auth[j++] = c;
						c = buf[i++];
					}
					auth[j] = '\0';

					if (strcmp(AUTH, auth) != 0) {
						setError(auth);
					} else {
						if (strcmp("GET", method) == 0) {
							if (resource[0] != '\0')
								debug("doGET");
							doGET(resource);
						} else if (strcmp("POST", method) == 0) {
							if (resource[0] != '\0')
								debug("doPOST");
							doPOST(resource, value);
						}
					}
				}
			}
		}
	}
}

void Request::doGET(const char* resource) {
	switch (resource[0]) {
	case 'W':
		doGETWent(resource);
		break;
	case 'T':
		doGETTemp(resource);
		break;
	}
}

void Request::doGETWent(const char* resource) {
	if (strcmp(WN, resource) == 0) {
		int rpm = getDesiredWentRPM(NEW_WENT) * 10;
		sprintf(response, "%d", rpm);
	} else if (strcmp(WU, resource) == 0) {
		int rpm = getDesiredWentRPM(USED_WENT) * 10;
		sprintf(response, "%d", rpm);
	} else if (strcmp(WWN, resource) == 0) {
		int rpm = getDesiredAiringVentRPM(NEW_WENT) * 10;
		sprintf(response, "%d", rpm);
	} else if (strcmp(WWU, resource) == 0) {
		int rpm = getDesiredAiringVentRPM(USED_WENT) * 10;
		sprintf(response, "%d", rpm);
	}
}

void Request::doGETTemp(const char* resource) {
	if (strcmp(TMD, resource) == 0) {
		sprintf(response, "%d", getTempMinDay());
	} else if (strcmp(TXD, resource) == 0) {
		sprintf(response, "%d", getTempMaxDay());
	} else if (strcmp(TMN, resource) == 0) {
		sprintf(response, "%d", getTempMinNight());
	} else if (strcmp(TXN, resource) == 0) {
		sprintf(response, "%d", getTempMaxNight());
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
	int temp = atoi(value);
	if (strcmp(TMD, resource) == 0) {
		setMinDayTemp(temp);
	} else if (strcmp(TXD, resource) == 0) {
		setMaxDayTemp(temp);
	} else if (strcmp(TMN, resource) == 0) {
		setMinNightTemp(temp);
	} else if (strcmp(TXN, resource) == 0) {
		setMaxNightTemp(temp);
	}
}

void doPOSTWent(const char* resource, const char* value) {
	debug("doPOSTWent");
	int i_value = atoi(value) / 10;
	if (strcmp(WN, resource) == 0) {
		setDesiredVentRPM(NEW_WENT, i_value);
	} else if (strcmp(WU, resource) == 0) {
		setDesiredVentRPM(USED_WENT, i_value);
	} else if (strcmp(WWN, resource) == 0) {
		setDesiredAiringVentRPM(NEW_WENT, i_value);
	} else if (strcmp(WWU, resource) == 0) {
		setDesiredAiringVentRPM(USED_WENT, i_value);
	}
}

