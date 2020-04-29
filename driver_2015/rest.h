


void parse(const char* req);




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
    char* getResponse();
    void setError(const char* err);
    virtual ~Request();
    int isError();
    int parseFromTo(int from,int to, const char* value);
    int parseHour(const char* value);
    int parseMinute(const char* value);
    //YYYYMMDDHHmm
    int parseDay(const char* value);
    //YYYYMMDDHHmm
    int parseMonth(const char* value);
    //YYYYMMDDHHmm
    int parseYear(const char* value);
    int returnDebugInResponse();

private:
    void init(const char* buf);
    int parseMethod(int i, const char* buf);
	char parseResource(char c, const char* buf, int& i);
	int parseValue(char c,  const char* buf, int i);
	int parseDebug(char c, const char* buf, int i);

	void doGET(const char* resource);
	void doGETWent(const char* resource);
	void doGETTemp(const char* resource);
	void doGETClock(const char* resource);

	void doPOST(const char* resource, const char* value);
	void doPOSTTemp(const char* resource, const char* value);
	void doPOSTClock(const char* resource, const char* value);
	void doPOSTWent(const char* resource, const char* value);

    char* resource;
    char* method;
    char* value;
    char* error;
    char* auth;
    int errorFlag;
    char* response;
    int debugFlag;

};
