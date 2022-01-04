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

const char *WN = "WN"; // went new
const char *WU = "WU"; // went used
const char *WWN = "WWN"; // went airing new
const char *WWU = "WWU"; // went airing used

const char *TMD = "TMD"; // temp min day
const char *TXD = "TXD"; // temp max day

const char *TMN = "TMN"; // temp min night
const char *TXN = "TXN"; // temp max night

const char *TMA = "TMA"; // temp min night
const char *TXA = "TXA"; // temp max night

const char *CLO = "CLO"; // clock YYYYMMDDHHmm

const char *AUTH = "d3c6d3c="; //ww:ww

int Request::returnDebugInResponse() {
	return debugFlag;
}

int Request::isError() {
	return errorFlag;
}

Request::Request(const char *buf) {
	errorFlag = 0;
	debugFlag = 0;
	method = new char[8];
	resource = new char[8];
	value = new char[13];
	value[0] = '\0';
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
	//debug("Request destructor done");
}

char* Request::getResponse() {
	return response;
}

char* Request::getError() {
	return error;
}

char* Request::getResource() {
	return resource;
}
char* Request::getValue() {
	return value;
}
char* Request::getMethod() {
	return method;
}

void Request::setError(const char *err) {
	errorFlag = 1;
	strcpy(error, err);
}

int Request::parseMethod(int i, const char *buf) {
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

char Request::parseResource(char c, const char *buf, int &i) {
	int j = 0;
	c = buf[i++]; // this should be 'first char of resource'
	while (c != ' ' && c != '/' && c != '?' && j < 8) {
		resource[j++] = c;
		c = buf[i++];
	}
	resource[j] = '\0';

	i = parseDebug(c, buf, i);

	return c;
}

int Request::parseDebug(char c, const char *buf, int i) {
	if (c == '?') {
		int j = 0;
		c = buf[i++];
		//hack if name of query starts from d = debug
		if (c == 'd') {
			debugFlag = 1;
		}
		while (c != '\n' && c != ' ') {
			c = buf[i++]; // scroll to the next line
		}
	}
	return i;
}

int Request::parseValue(char c, const char *buf, int i) {
	//get value from request
	if (c == '/') {
		// this is a start of value
		int j = 0;
		c = buf[i++];
		while (c != ' ' && c != '/' && c != '?' && j < 14) {
			value[j++] = c;
			c = buf[i++];
		}
		value[j] = '\0';
	}
	//check query
	i = parseDebug(c, buf, i);
	if (c == ' ') {
		// no value just resource
		while (c != '\n')
			c = buf[i++]; // scroll to the next line
	}
	return i;
}

//return the position of Authorization in buf
// -1 if not found
int Request::findAuthorization(int start, int guard, const char *buf) {
	int i = start;
	char c, c1;
	do {
		c = buf[i++];
		c1 = buf[i];
	} while ((c != 'A' || c1 != 'u') && i < guard);

	if (c == 'A' && c1 == 'u' && buf[i + 1] == 't' && buf[i + 2] == 'h') {
		return i - 1;
	}
	return -1;
}

int Request::checkAuthorization(int start, int guard, const char *buf) {
	int ret = 0;
	int i = start;
	int j;
	char c = buf[i]; //this should be the first char of "Authoriztion:"

	while (c != ':' && i < guard)
		c = buf[i++]; // scroll till ":"

	while (c != 'B' && i < guard)
		c = buf[i++]; // scroll till start of "Basic"
	//basic

	if (buf[i] == 'a' || buf[i + 1] == 's') {
		while (c != ' ')
			c = buf[i++];	//skipp basic word

		c = buf[i++]; //eat  ' '
		j = 0;
		while (c != ' ' && c != '\n' && j < 9 && c != '\r') {
			auth[j++] = c;
			c = buf[i++];
		}
		auth[j] = '\0';

		if (strcmp(AUTH, auth) != 0) {
			setError(auth);
			ret = 1;
		}
	} else {
		setError("Bad request missing Authorization Basic required");
		ret = 1;
	}
}

//max buf size = 100
void Request::init(const char *buf) {
	int total_guard = 295; //max buff size 300
	int i = 0;
	int j = 0;
	char c, c1;
	i = parseMethod(i, buf);

	//get resource from request
	c = buf[i++]; // this should be '/'
	if (c != '/')
		setError("ERR:bad request missing resource");
	if (!errorFlag) {
		c = parseResource(c, buf, i);
		//get value from request
		i = parseValue(c, buf, i);
		//Find Authorization line
		int authStart = findAuthorization(i, total_guard, buf);
		if (authStart == -1) {
			setError("Bad request missing Authorization");
		} else {
			checkAuthorization(authStart, total_guard, buf);
		}
		if (!errorFlag) {
			if (strcmp("GET", method) == 0) {
				if (resource[0] != '\0')
					debug("doGET");
				doGET(resource);
			} else if (strcmp("POST", method) == 0
					|| strcmp("PUT", method) == 0) {
				if (resource[0] != '\0')
					debug("doPOST");
				doPOST(resource, value);
			}
		}
	}
}

void Request::doGET(const char *resource) {
switch (resource[0]) {
case 'W':
doGETWent(resource);
break;
case 'T':
doGETTemp(resource);
break;
case 'C':
doGETClock(resource);
break;

}
}

void Request::doGETWent(const char *resource) {
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

void Request::doGETClock(const char *resource) {
//	time_t vt_now = now();
//	snprintf(response, "\"%02d:%02d:%02d\"", hour(vt_now), minute(vt_now), second(vt_now));
}

void Request::doGETTemp(const char *resource) {
if (strcmp(TMD, resource) == 0) {
sprintf(response, "%d", getTempMinDay());
} else if (strcmp(TXD, resource) == 0) {
sprintf(response, "%d", getTempMaxDay());
} else if (strcmp(TMN, resource) == 0) {
sprintf(response, "%d", getTempMinNight());
} else if (strcmp(TXN, resource) == 0) {
sprintf(response, "%d", getTempMaxNight());
} else if (strcmp(TXA, resource) == 0) {
sprintf(response, "%d", getTempMaxAfternoon());
} else if (strcmp(TMA, resource) == 0) {
sprintf(response, "%d", getTempMinAfternoon());
}
}

void Request::doPOST(const char *resource, const char *value) {

switch (resource[0]) {
case 'W':
doPOSTWent(resource, value);
break;
case 'T':
doPOSTTemp(resource, value);
break;
case 'C':
doPOSTClock(resource, value);
break;
}
}

void Request::doPOSTTemp(const char *resource, const char *value) {
int temp = atoi(value);
if (strcmp(TMD, resource) == 0) {
setMinDayTemp(temp);
} else if (strcmp(TXD, resource) == 0) {
setMaxDayTemp(temp);
} else if (strcmp(TMN, resource) == 0) {
setMinNightTemp(temp);
} else if (strcmp(TXN, resource) == 0) {
setMaxNightTemp(temp);
} else if (strcmp(TMA, resource) == 0) {
setMinAfternoonTemp(temp);
} else if (strcmp(TXA, resource) == 0) {
setMaxAfternoonTemp(temp);
}
}

//parse from to from string - string max 12chars result max 4 chars
int Request::parseFromTo(int from, int to, const char *value) {
char ret[7];
int i = from;
int j = 0;
char c = value[i];
while (++i <= to && i < 14 && j < 6) {
ret[j++] = c;
c = value[i];
}
ret[j] = '\0';
int iRet = atoi(ret);

return iRet;
}

//YYYYMMDDHHmm
int Request::parseHour(const char *value) {
int i = parseFromTo(8, 10, value);
if (i < 0 || i > 23)
i = 12;
return i;
}
//YYYYMMDDHHmm
int Request::parseMinute(const char *value) {
int i = parseFromTo(10, 12, value);
if (i < 0 || i > 60)
i = 0;
return i;
}
//YYYYMMDDHHmm
int Request::parseDay(const char *value) {
int i = parseFromTo(6, 8, value);
if (i < 1 || i > 31)
i = 1;
return i;
}
//YYYYMMDDHHmm
int Request::parseMonth(const char *value) {
int i = parseFromTo(4, 6, value);
if (i < 1 || i > 12)
i = 1;
return i;
}
//YYYYMMDDHHmm
int Request::parseYear(const char *value) {
int i = parseFromTo(0, 4, value);
if (i < 2020 || i > 2030)
i = 2020;
return i;
}

//YYYYMMDDHHmm
void Request::doPOSTClock(const char *resource, const char *value) {
//	printf("-----doPOSTClock-----------------------\n");
//	printf("-----doPOSTClock -phResult----------------------:%d\n",parseHour(value));
//	printf("-----doPOSTClock -pmResult----------------------:%d\n",parseMinute(value));
//	printf("-----doPOSTClock -pDResult----------------------:%d\n",parseDay(value));
//	printf("-----doPOSTClock -pMResult----------------------:%d\n",parseMonth(value));
//	printf("-----doPOSTClock -pYResult----------------------:%d\n",parseYear(value));
if (strcmp(CLO, resource) == 0) {
setTime(parseHour(value), parseMinute(value), /*second()*/0, parseDay(value),
		parseMonth(value), parseYear(value));
setTimeRTC();
}
}

void Request::doPOSTWent(const char *resource, const char *value) {
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
setWents();
}

