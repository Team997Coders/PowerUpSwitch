#include <Arduino.h>

class MACFactory
{
  // This mac will get set from EEPROM
  byte mac[6];
  char macstr[18];

  public:
  MACFactory();
  void setupMac();
  void getMac(byte *buffer, int count);
  void printMac();
};

class DHCPIndicator
{
  int DHCPPin;
  bool useDHCPFlag;
  
  public:
  DHCPIndicator(int pin);
  bool useDHCP();
};

