#ifndef _termometry_H_
#define _termometry_H_

#define TEMPCOUNT 6 //ilosc termometrów

#define PODLOGA_OUT 4
#define PODLOGA_IN 5

/*
 * UT: test_init
 */
void initConfigTemp();

/*
 * UT: test_init
 */
void setMaxDayTemp(int temp);
/*
 * UT: test_init
 */
void setMinDayTemp(int temp);
/*
 * UT: test_init
 */
void setMinNightTemp(int temp);
/*
 * UT: test_init
 */
void setMaxNightTemp(int temp);

/*
 * UT: test_init
 */
int getTempMixingStart();

/*
 * UT: test_set50, test_init2, test_init3
 */
void setMixingPumpStartTemp(int temp);

/*
 * UT: test_inc
 */
void incMinDzienna();
void incMaxDzienna();
void incMinNocna();
void incMaxNocna();
void incPompaMiesz();

/*
 * UT: test_dec
 */
void decMinDzienna();
void decMaxDzienna();
void decMinNocna();
void decMaxNocna();
void decPompaMiesz();

/*
 * UT: test_set50, test_init2, test_init3
 */
int getTempMinDay();
int getTempMaxDay();
int getTempMinNight();
int getTempMaxNight();

/*
 * UT: test_checkTemp
 */
bool checkTemp(int temp); //check if the tem is between const uint8_t ci_minTemp = 5; const uint8_t ci_maxTemp = 95;

/*
 * UT: test_currentTemps
 */
float getCurrentTemps(int i);
void setCurrentTemps(int i, float value);

//UT - impossible actual read from sensors on arduino
float getTempC(int nr);

void setTempWaitTo(int t);
int getTempWaitTo();
void setTempHeatTo(int t);
int getTempHeatTo();

#endif /* _termometry_H_ */
