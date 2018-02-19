#include <Arduino.h>
#include <EEPROM.h>
#include "Network.h"

MACFactory::MACFactory()
{
   mac[0] = 0xBE;
   mac[1] = 0xEF;
   mac[2] = 0x00;
   mac[3] = 0x00;
   mac[4] = 0x00;
   mac[5] = 0x00;
}

void MACFactory::setupMac() {
  // Random MAC address stored in EEPROM
  // This relies on "noise" from the unconnected analog port 0 to seed
  if (EEPROM.read(1) == '#') {
    for (int i = 2; i < 6; i++) {
      mac[i] = EEPROM.read(i);
    }
  } else {
    for (int i = 2; i < 6; i++) {
      mac[i] = random(0, 255);
      EEPROM.write(i, mac[i]);
    }
    EEPROM.write(1, '#');
  }
  snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  printMac();
}

void MACFactory::getMac(byte *buffer, int count) {
  int max;
  // Don't overrun our buffer
  if (count > sizeof(mac))
    max = sizeof(mac);
  else
    max = count;
  // Copy away...zero based array!
  for(int i = 0; i < max; i++)
    buffer[i] = mac[i];
}

void MACFactory::printMac() {
  // Spit out the MAC address
  Serial.print(F("MAC ("));
  Serial.print(macstr);
  Serial.println(F(")"));  
}

DHCPIndicator::DHCPIndicator(int pin) {
  DHCPPin = pin;
  pinMode(pin, INPUT_PULLUP);
  useDHCPFlag = digitalRead(pin); // set the state once
}

bool DHCPIndicator::useDHCP() {
  return useDHCPFlag;  // default is HIGH, which means use DHCP by default  
}

