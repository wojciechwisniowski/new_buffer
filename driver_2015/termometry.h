
#define TEMPCOUNT 6 //ilosc termometrów
#define TEMPCOUNT_REKU 4 //termometry requ
#define WENTCOUNT 2 //ilość wentylatorów
#define REKUPERATYOR_ID 4 //id na szynie I2C

#define PODLOGA_OUT 4
#define PODLOGA_IN 5

#define NEW_WENT 0
#define NEW_WENT_MAX 2700
#define USED_WENT 1
#define USED_WENT_MAX 2650
#define WENT_STEP1 100
#define WENT_STEP2 200
#define WENT_STEP3 500
#define NEW_IN 0
#define NEW_OUT 1
#define USED_IN 2
#define USED_OUT 3




//typedef struct {
//	uint8_t  vt_ID[OW_ROMCODE_SIZE]; //identyfikator termometru
//	int8_t   vi_Offs0,vi_Offs20;     //o ile (1/10'C) korygowaæ odczyt termometru przy 20'C,0'C
//	char     vv_Name[4];             //nazwa termometru
//	uint8_t  vi_EMASize10sec;        //wielkoœæ EMA dla TEMP_READ_SSEC=10*100 -> dla innych interwa³ów automatycznie korygowane
//} TempData;
//
//TempData ct_TempsData[TEMP_COUNT] /*PROGMEM = TEMP_DATA*/;
//
////fibo: 1 2 3 5 8 13 21 34 55 89 144 233 377
//#define TEMP_DATA /**/\
//TempData ct_TempsData[TEMP_COUNT] PROGMEM = {\
//  { {0x28,0x51,0x57,0x0E,0x02,0x00,0x00,0x99}\
//   ,0,0,"TOP",5}\
// ,{ {0x28,0xDB,0xC1,0x71,0x02,0x00,0x00,0xA5}\
//   ,0,-4,"MID",21}\
// ,{ {0x28,0x7C,0xBB,0xEA,0x03,0x00,0x00,0x72}\
//  ,0,3,"BOT",34}\
// ,{ {0x28,0x32,0xD0,0xEA,0x03,0x00,0x00,0x75}\
//  ,0,0,"PZA",55}\
// ,{ {0x28,0x6E,0x61,0x0E,0x02,0x00,0x00,0x48}\
//  ,0,1,"PPO",89}\
//}


