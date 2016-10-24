
#ifdef TEST_REST
char*request = "GET /ala/ola HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*request2 = "POST /ala/ola HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

char*requestGetWN = "GET /WN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestGetWU = "GET /WU HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestGetWWN = "GET /WWN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestGetWWU = "GET /WWU HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

char*requestSetWN = "POST /WN/1 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestSetWU = "POST /WU/22 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestSetWWN = "POST /WWN/333 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestSetWWU = "POST /WWU/4444 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

char*requestGetTMD = "GET /TMD HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestGetTXD = "GET /TXD HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestGetTMN = "GET /TMN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestGetTXN = "GET /TXN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

char*requestSetTMD = "POST /TMD/29 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestSetTXD = "POST /TXD/35 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestSetTMN = "POST /TMN/60 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
char*requestSetTXN = "POST /TXN/80 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
#endif

char* WN = "WN"; // went new
char* WU = "WU"; // went used
char* WWN = "WWN"; // went airing new
char* WWU = "WWU"; // went airing used

char* TMD = "TMD"; // temp min day
char* TXD = "TXD"; // temp max day

char* TMN = "TMN"; // temp min night
char* TXN = "TXN"; // temp max night
char* AUTH ="d3c6d3c="; //ww:ww

void parse(char* req, EthernetClient& client);
void doGET(char* resource, char* value, EthernetClient& client);
void doGETWent(char* resource, char* value, EthernetClient& client);
void doGETTemp(char* resource, char* value, EthernetClient& client);
void doPOST(char* resource, char* value, EthernetClient& client);
void doPOSTTemp(char* resource, char* value, EthernetClient& client);
void doPOSTWent(char* resource, char* value, EthernetClient& client);

