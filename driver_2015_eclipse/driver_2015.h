void getTMD(EthernetClient& client);
void getTMN(EthernetClient& client);
void getTXD(EthernetClient& client);
void getTXN(EthernetClient& client);

// get went new rpm
void getWN(EthernetClient& client);
// get went used rpm
void getWU(EthernetClient& client);

// get went airing new rpm
void getWWN(EthernetClient& client);

// get went airing used rpm
void getWWU(EthernetClient& client);

// set went new rpm
void setWN(char* value, EthernetClient& client);
// set went used rpm
void setWU(char* value, EthernetClient& client);

// set went airing new rpm
void setWWN(char* value, EthernetClient& client);

// set went airing used rpm
void setWWU(char* value, EthernetClient& client);

void setTMD(char* value, EthernetClient& client);
void setTXD(char* value, EthernetClient& client);
void setTMN(char* value, EthernetClient& client);
void setTXN(char* value, EthernetClient& client);
void logError(char* s);
