#ifndef H_FIELDCONTROLLER
#define H_FIELDCONTROLLER

#include <Arduino.h>
#include <Ethernet.h>
#include "Alliance.h"
#include "HTTPClient.h"

class FieldController {
  IPAddress fieldControllerServer;
  HTTPClient httpClient;
  
  public:
  FieldController(char *ipAddress);
  void updateSwitch(Alliance forAlliance, bool lhsOwnsIt, bool rhsOwnsIt);
  void initializeSwitchSettings(Alliance forAlliance, Alliance lhs, Alliance rhs, IPAddress ip);
};

#endif // H_FIELDCONTROLLER
