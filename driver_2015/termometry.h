#ifndef _termometry_H_
#define _termometry_H_

#include <DallasTemperature.h>
#include <OneWire.h>
#include <EEPROM.h>

#define TEMPCOUNT 6 //ilosc termometrów


#define GI_PIN_ONE_WIRE_BUS 40 //termometry 40 dla buf

#define PODLOGA_OUT 4
#define PODLOGA_IN 5


extern uint8_t gi_EE_Temp_Min_Day;
extern uint8_t gi_EE_Temp_Max_Day;
extern uint8_t gi_EE_Temp_Min_Night;
extern uint8_t gi_EE_Temp_Max_Night;
extern uint8_t gi_EE_Temp_Mixing_Start;

extern uint8_t gi_Temp_Min_Day;
extern uint8_t gi_Temp_Max_Day;
extern uint8_t gi_Temp_Min_Night;
extern uint8_t gi_Temp_Max_Night;
extern uint8_t gi_Temp_Mixing_Start;

extern float gf_currentTemps[TEMPCOUNT];


extern DeviceAddress gDA_sensors[TEMPCOUNT];


extern OneWire oneWire;
extern DallasTemperature sensors;

void initConfigTemp();
//from Dallas Temp example Simple
void setupDS();

void decMinDzienna();
void incMinDzienna();
void decMaxDzienna();
void incMaxDzienna();
void decMinNocna();
void incMinNocna();
void decMaxNocna();
void incMaxNocna();
void decPompaMiesz();
void incPompaMiesz();


#endif /* _termometry_H_ */
