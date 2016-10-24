void getTMD(const Print& client);
void getTMN(const Print& client);
void getTXD(const Print& client);
void getTXN(const Print& client);

// get went new rpm
void getWN(const Print& client);
// get went used rpm
void getWU(const Print& client);

// get went airing new rpm
void getWWN(const Print& client);

// get went airing used rpm
void getWWU(const Print& client);

// set went new rpm
void setWN(char* value, const Print& client);
// set went used rpm
void setWU(char* value, const Print& client);

// set went airing new rpm
void setWWN(char* value, const Print& client);

// set went airing used rpm
void setWWU(char* value, const Print& client);

void setTMD(char* value, const Print& client);
void setTXD(char* value, const Print& client);
void setTMN(char* value, const Print& client);
void setTXN(char* value, const Print& client);
void logError(char* s);

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


