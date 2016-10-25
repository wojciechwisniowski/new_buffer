#include <OneWire.h>
#include <DallasTemperature.h>

#include <openGLCD.h>

//time
#include <Time.h>  
#include <Wire.h>  
#include "driver_2015_term_get.h"
//#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
//#include <DS3232RTC.h>  // better RTC
//time

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 40

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


void setup(void){
  setupSerial();
  setupDS();
  setupGLCD();
  setupRTC();
}

void setupSerial(){
  Serial.begin(9600);
}

//from Time RTC example
void setupRTC()  {
//  setSyncProvider(RTC.get);   // the function to get the time from the RTC
//  if(timeStatus()!= timeSet){
//     print0(0,"Unable to sync with the RTC");
//     delay(1000);
//  } else {
//     print0(0,"RTC has set the system time");
//     delay(1000);
//  }
}

//from Dallas Temp example Simple
void setupDS(void)
{
  // Start up the library
  sensors.begin();
}

//from openGLCD example Hello world
void setupGLCD()
{
  // Initialize the GLCD 
  GLCD.Init();

 // Select the font for the default text area
  GLCD.SelectFont(System5x7);

//  GLCD.print(F("hello, world!")); // keep string in flash on AVR boards with IDE 1.x
//  GLCD.Puts(F("hello, world!")); // Puts() supports F() with any version of IDE

  // print() below uses RAM on AVR boards but works
  // on any version of IDE with any processor
  // note: Same is true for Puts()
}

void loop(void)
{ 
//for(int i=0;i<10;i++)  print0(i,char(i));
//  // call sensors.requestTemperatures() to issue a global temperature 
//  // request to all devices on the bus
//  print0(1,"Requesting temperatures...");
 sensors.requestTemperatures(); // Send the command to get temperatures
//  print0(2,"DONE");
//  
//  print0(3,"Temperature for the device 1 (index 0) is: ");
//  print0(4,sensors.getTempCByIndex(0));  
//  digitalClockDisplay();  
GLCD.ClearScreen();
  testTerm();
    delay(3000);
}



void print0(int line,char* str){
 
// set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  GLCD.CursorTo(0, line);

  // print the number of seconds since reset:
  GLCD.print(str);
  
}  

void print0(int line,float f){ 
// set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  GLCD.CursorTo(0, line);

  // print the number of seconds since reset:
  GLCD.print(f);  
}  



//from openGLCD digital clock example
void  digitalClockDisplay()
{  
static time_t  prevtime;
char buf[11];

	if( prevtime != now() ) // if 1 second has gone by, update display
	{
		prevtime = now();   // save the last update time

		// format the time in a buffer
		snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hour(), minute(), second());

		// draw the formatted string centered on the display
  		GLCD.DrawString(buf,  gTextfmt_right, gTextfmt_top);
  
  		// format the time in a buffer

                snprintf(buf, sizeof(buf), "%02d.%02d.%04d", day(), month(), year());
		// draw the formatted string centered on the display
  		GLCD.DrawString(buf,  gTextfmt_left, gTextfmt_top);
	}
}


tmElements_t tm;

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

bool getTime(const char *str)
{
int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
    return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}


//from openGLCD digital clock example

//temp test
void testTerm(){ 
  int numberOfDevices;
  numberOfDevices = sensors.getDeviceCount();
  DeviceAddress tempDeviceAddress;
  print0(0,char(numberOfDevices));
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
	{
		print0(0,"Found device ");
		print0(1,char(i));
                print0(2+i,": ");
		printAddress(tempDeviceAddress);

	}else{
		print0(6,"Found ghost device at ");
                print0(7,char(i));
	}
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{

  for (uint8_t i = 0; i < 8; i++)  {
    if (deviceAddress[i] < 16) GLCD.print("0");
    GLCD.print(deviceAddress[i], HEX);
  }
}


//temp test
