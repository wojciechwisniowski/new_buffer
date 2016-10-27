/*
 * driver_net.h
 *
 *  Created on: Oct 26, 2016
 *      Author: root
 */

#ifndef DRIVER_NET_H_
#define DRIVER_NET_H_
#include <Arduino.h>
#include <Ethernet.h>
#include <Time.h>
#include <SD.h>

#include "termometry.h"
#include "vents.h"
#include "power_tariffs.h"
#include "driver_buffer.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
extern byte mac[];
extern IPAddress ip;

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
extern EthernetServer server;
extern File myFile;

void setupHttp();
void loopServer();

#endif /* DRIVER_NET_H_ */
