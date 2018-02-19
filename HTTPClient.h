#ifndef H_HTTPCLIENT
#define H_HTTPCLIENT

#include <Arduino.h>
#include <Ethernet.h>

class HTTPClient
{
  EthernetClient client;
  
  public:
  //HTTPClient();
  byte Post(
    IPAddress ipAddress, 
    int thisPort, 
    char* page, 
    char* thisData);
};

#endif // H_HTTPCLIENT
