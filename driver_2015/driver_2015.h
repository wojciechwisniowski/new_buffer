char* getTMD();
char* getTMN();
char* getTXD();
char* getTXN();

// get went new rpm
char* getWN();
// get went used rpm
char* getWU();

// get went airing new rpm
char* getWWN();

// get went airing used rpm
char* getWWU();

// set went new rpm
char* setWN(char* value );
// set went used rpm
char* setWU(char* value );

// set went airing new rpm
char* setWWN(char* value );

// set went airing used rpm
char* setWWU(char* value );

char* setTMD(char* value );
char* setTXD(char* value );
char* setTMN(char* value );
char* setTXN(char* value );
char* logError(char* s);

void decWentNew();

void incWentNew();
void decWentUSED();
void incWentUSED();
void setWents();
void changeWentStep();
void startWietrzenie();
void stopWietrzenie();
void checkAndChangeTariff();
void checkAndChangeBuffor();
String getWentString();
void clearScreenWithoutTime();
void incWent(int went, int max);
void decWent(int went, int max);
void loopServer();
void nextScreen();
void prevScreen();

void obslugaPompyMieszajacej(float temp);
void wylaczPompaBuf();
void wlaczPompaBuf();
void ustawPinyGrzalek(uint8_t stan);
int setupGLCD(int line);
void setupDS();
void setupBuf();
void setupHttp();
void setupPompy();
int setupRTC(int line);
int setupSD(int line);

void printConfigGodz();
void printConfigGodz2();
void printConfigTemp();
void printConfigWent();
void printConfigFiles();
void printDigitalClock();
void print0(int line, char const * str);
void printBottomStatus(char* s);
void printGrzalkiStatus(int grzejDo, int czekajDo);
void printWent();
void printTemps(int counter);
void patDog();
void wylaczGrzalki();
void wlaczGrzalki();
void logCurrentStatusToFile();


