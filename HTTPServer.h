#ifndef H_HTTPSERVER
#define H_HTTPSERVER

#include <Arduino.h>
#include <Ethernet.h>
#include "Switch.h"
#include "FieldController.h"

class HTTPServer
{
  EthernetServer server = EthernetServer(80);
  
  public:
  void SetGameState(Switch *mySwitch, FieldController *fieldController);
};

#endif // H_HTTPSERVER
