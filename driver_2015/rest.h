
#define TEST_REST

#ifdef TEST_REST
//const char*request = "GET /ala/ola HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*request2 = "POST /WW/MM HTTP/1.1\n\rhost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";
const char*request1 = "GET /ala/ola HTTP/1.1\n\rhost: 176.115.13.101:8080\n\rAuthorization: Basic d3c6d3c=\n\raccept�";

const char*requestGetWN = "GET /WN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetWU = "GET /WU HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetWWN = "GET /WWN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetWWU = "GET /WWU HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

const char*requestSetWN = "POST /WN/1 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetWU = "POST /WU/22 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetWWN = "POST /WWN/333 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetWWU = "POST /WWU/4444 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

const char*requestGetTMD = "GET /TMD HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetTXD = "GET /TXD HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetTMN = "GET /TMN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestGetTXN = "GET /TXN HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";

const char*requestSetTMD = "POST /TMD/29 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetTXD = "POST /TXD/35 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetTMN = "POST /TMN/60 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";
const char*requestSetTXN = "POST /TXN/80 HTTP/1.1 \n\rHost: 176.115.13.101:8080\n\rUser-Agent: cos";



#endif

const char* WN = "WN"; // went new
const char* WU = "WU"; // went used
const char* WWN = "WWN"; // went airing new
const char* WWU = "WWU"; // went airing used

const char* TMD = "TMD"; // temp min day
const char* TXD = "TXD"; // temp max day

const char* TMN = "TMN"; // temp min night
const char* TXN = "TXN"; // temp max night
const char* AUTH = "d3c6d3c="; //ww:ww

void parse(const char* req);
void doGET(const char* resource, const char* value);
void doGETWent(const char* resource, const char* value);
void doGETTemp(const char* resource, const char* value);
void doPOST(const char* resource, const char* value);
void doPOSTTemp(const char* resource, const char* value);
void doPOSTWent(const char* resource, const char* value);

#ifndef TEST_REST
#else
#endif

class Request {
public:
    Request(const char* buf);
    char* getResource();
    char* getMethod();
    char* getValue();
    char* getError();
    void setError(const char* err);
    virtual ~Request();
    int isError();
private:
    void init(const char* buf);
    char* resource;
    char* method;
    char* value;
    char* error;
    char* auth;
    int errorFlag;

};
