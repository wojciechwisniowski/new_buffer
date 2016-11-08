#ifndef _termometry_prod_H_
#define _termometry_prod_H_

#include <DallasTemperature.h>
#include <OneWire.h>
#include <EEPROM.h>

float getTempC(int nr);

bool inline checkTemp(uint8_t temp); //check if the tem is between const uint8_t ci_minTemp = 5; const uint8_t ci_maxTemp = 95;

void requestTemperatures();
//from Dallas Temp example Simple
void setupDS();


#endif /* _termometry_prod_H_ */
