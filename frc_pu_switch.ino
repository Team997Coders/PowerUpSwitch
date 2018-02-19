/*
  Web client for FRC Power Up Switch Field Element

 This sketch connects to a field controller to transmit switch events
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * LHS magnetic reed switch to pin 2
 * RHS magnetic reed switch to pin 3
 * LHS led to pin 5
 * RHS led to pin 6
 * On net to pin 8 (flashes until ip set)
 * Use DHCP pin 9 (active high - not jumpered)
 * Alliance color pin 7 (high, un-jumpered is RED)

 Authored by Chuck Benedict
 Library contributions by David A. Mellis,
 Tom Igoe, and Adrian McEwen

 */

#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include "Flasher.h"
#include "Alliance.h"
#include "HTTPClient.h"
#include "HTTPServer.h"
#include "Network.h"
#include "Switch.h"
#include "FieldController.h"

// Set the default IP here if DHCP fails
byte defaultIP[4] = {192, 168, 1, 3};

// Are we connected to the network with an IP ready to communicate?
bool connected;

// Define pins
#define LHSSWITCHPIN 2
#define RHSSWITCHPIN 3
#define LHSLEDPIN 5
#define RHSLEDPIN 6
#define ALLIANCEPIN 7
#define HAVEIPLEDPIN 8
#define DHCPPIN 9

// Define simple flashing LED to display whether we have an IP address on the network
Flasher haveIPLED(HAVEIPLEDPIN, 350, 350);

// Define a representation of the alliance selected, as jumpered on the board
AllianceSelector allianceSelector(ALLIANCEPIN);

// Define a representation of the physical switch
Switch theSwitch(LHSSWITCHPIN, RHSSWITCHPIN, LHSLEDPIN, RHSLEDPIN);

// Deal with getting a random MAC since ethernet shields do not have one in EEPROM
MACFactory macFactory;

// Define an indicator to tell us whether to use DHCP
DHCPIndicator dhcpIndicator(DHCPPIN);

// Define a representation of the field controller so we can send updates when the switch changes
FieldController fieldController("192.168.1.54");

// Define an HTTP server to get the game state
HTTPServer httpServer;

void setup() {
  // Get true-ish randomness
  CreateTrulyRandomSeed();
  
  // Open serial communications and wait for port to open...
  // Stuff from here on out will write to serial port to let us know what is going on.
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only.
    // TODO: What happens when USB port is not connected?  Does this hang?
  }

  // Gen a unique mac address
  macFactory.setupMac();

  // Start the Ethernet connection
  connected = false;
  byte mac[6];
 
  macFactory.getMac(mac, 6);
  if (dhcpIndicator.useDHCP()) {
    Serial.print(F("Using DHCP..."));
    if (Ethernet.begin(mac) == 0) {
      Serial.println(F("Failed to configure Ethernet using DHCP"));
    }
    else
    {
      connected = true;
      indicateHaveIP();
      Serial.print(F("success: "));
      Serial.println(Ethernet.localIP());
    }
  }
  else {
    connected = true;
    Ethernet.begin(mac, defaultIP);
    Serial.print(F("Using default IP: "));    
    Serial.println(Ethernet.localIP());
  }
    

  // Give the Ethernet shield a second to initialize
  delay(1000);

  // Tell the switch class which alliance we are
  theSwitch.setAlliance(allianceSelector.Is());
  Serial.print(F("Alliance is "));
  Serial.println(AllianceAsString::Is(allianceSelector.Is()));
  theSwitch.randomize();
  if (connected)
  {
    Serial.println(F("Posting switch settings to server..."));
    // Send the update to the field controller
    fieldController.initializeSwitchSettings(theSwitch.getAlliance(), theSwitch.getLHSAlliance(), theSwitch.getRHSAlliance(), Ethernet.localIP());
  }
  theSwitch.off();
}

void loop() {
  // TODO: This should be disabled if we are using static IP
  if (dhcpIndicator.useDHCP()) {
    Ethernet.maintain();
  }
  
  // Update the local board LED telling us if we have an IP address (flashing = no, solid = yes)
  haveIPLED.Update();

  // Process web server requests
  if (connected) {
    httpServer.SetGameState(&theSwitch, &fieldController);
  }
  
  // Get the switch state
  theSwitch.updateState();
  theSwitch.updatePlates();

  // If the state of the switch has changed, 
  if (theSwitch.stateChanged() && theSwitch.getGameState() == GameState::PLAY)
  {
    if (connected) {
      Serial.println(F("Posting change to server..."));
      // Send the update to the field controller
      fieldController.updateSwitch(allianceSelector.Is(), theSwitch.isLHSOwned(), theSwitch.isRHSOwned());
    }
  }
  theSwitch.acknowledgeStateChanged();
}

void indicateHaveIP() {
  haveIPLED.AlwaysOn();
}

void clearEEPROM() {
  pinMode(13, OUTPUT);
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  digitalWrite(13, HIGH);
  while(true){};
}
