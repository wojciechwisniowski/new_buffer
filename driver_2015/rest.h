


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
