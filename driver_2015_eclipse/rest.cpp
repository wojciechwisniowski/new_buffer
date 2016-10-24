#include <Ethernet.h>
#include "rest.h"
#include "driver_2015.h"

/**
 * Created by wisnia on 04.10.16.
 */

void main(int argc, char* argv) {
	log(request);
}
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
void parse(char* buf, EthernetClient& client) {

	char* method = new char[8];
	char* resource = new char[8];
	char* value = new char[8];
	char* auth = new char[9];
	int i = 0;
	int j = 0;
	char c = buf[i++];

	// get method from request
	while (c != ' ' && i < 8) {
		method[j++] = c;
		c = buf[i++];
	}
	method[j] = '\0';

	//get resource from request
	c = buf[i++]; // this should be '/'
	if (c != '/')
		logError("bad request missing resource");
	j = 0;
	c = buf[i++]; // this should be 'first char of resource'
	while (c != ' ' && c != '/' && j < 8) {
		resource[j++] = c;
		c = buf[i++];
	}
	resource[j] = '\0';

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
	c = buf[i++];
	if (c != 'h') {
		logError("Bad request missing host line");
	}
	while (c != '\n')
		c = buf[i++]; // scroll to the next line

	//Authorization line
	c = buf[i++];
	if (c != 'A') {
		logError("Bad request missing Authorization");
	}
	while (c != ' ')
		c = buf[i++];
	//basic
	c = buf[i++];
	if (c != 'B') {
		logError("Bad request missing Authorization Basic required");
	}
	//skipp basic word
	while (c != ' ')
		c = buf[i++];
	j = 0;
	while (c != ' ' && c != '/' && j < 9) {
		auth[j++] = c;
		c = buf[i++];
	}
	auth[j] = '\0';

	if (strcmp(AUTH, auth) != 0) {
		logError("Authorization failed");
	} else {
		if (strcmp("GET", method) == 0) {
			if (resource[0] != '\0')
				doGET(resource, value, client);
		} else if (strcmp("POST", method) == 0) {
			if (resource[0] != '\0')
				doPOST(resource, value, client);
		}
	}
}

void doGET(char* resource, char* value, EthernetClient& client) {
	switch (resource[0]) {
	case 'W':
		doGETWent(resource, value, client);
		break;
	case 'T':
		doGETTemp(resource, value, client);
		break;
	}
}

void doGETWent(char* resource, char* value, EthernetClient& client) {
	if (strcmp(WN, resource) == 0) {
		getWN(client);
	} else if (strcmp(WU, resource) == 0) {
		getWU(client);
	} else if (strcmp(WWN, resource) == 0) {
		getWWN(client);
	} else if (strcmp(WWU, resource) == 0) {
		getWWU(client);
	}
}

void doGETTemp(char* resource, char* value, EthernetClient& client) {
	if (strcmp(TMD, resource) == 0) {
		getTMD(client);
	} else if (strcmp(TXD, resource) == 0) {
		getTXD(client);
	} else if (strcmp(TMN, resource) == 0) {
		getTMN(client);
	} else if (strcmp(TXN, resource) == 0) {
		getTXN(client);
	}
}

void doPOST(char* resource, char* value, EthernetClient& client) {
	switch (resource[0]) {
	case 'W':
		doPOSTWent(resource, value, client);
		break;
	case 'T':
		doPOSTTemp(resource, value, client);
		break;
	}
}

void doPOSTTemp(char* resource, char* value, EthernetClient& client) {
	if (strcmp(TMD, resource) == 0) {
		setTMD(value, client);
	} else if (strcmp(TXD, resource) == 0) {
		setTXD(value, client);
	} else if (strcmp(TMN, resource) == 0) {
		setTMN(value, client);
	} else if (strcmp(TXN, resource) == 0) {
		setTXN(value, client);
	}
}

void doPOSTWent(char* resource, char* value, EthernetClient& client) {
	if (strcmp(WN, resource) == 0) {
		setWN(value, client);
	} else if (strcmp(WU, resource) == 0) {
		setWU(value, client);
	} else if (strcmp(WWN, resource) == 0) {
		setWWN(value, client);
	} else if (strcmp(WWU, resource) == 0) {
		setWWU(value, client);
	}
}

