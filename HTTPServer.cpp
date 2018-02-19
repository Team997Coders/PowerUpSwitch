#include <Arduino.h>
#include "HTTPServer.h"
#include "BufferFiller.h"

void HTTPServer::SetGameState(Switch *mySwitch, FieldController *fieldController) {
  // listen for incoming clients
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println(F("Game state server request..."));
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      char command[16];
      int i = 0;
      while(client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          // Here is where the POST data is.  
          while(client.available())
          {
            if (i < sizeof(command)) {
              command[i++] = client.read();
            } else {
              client.read();
            }
          }

          if (strncasecmp("state=", command, 6) == 0)
          {
            if (strncasecmp("off", &command[6], 3) == 0) {
              mySwitch->off();
            } else if (strncasecmp("play", &command[6], 4) == 0) {
              mySwitch->play();
            } else if (strncasecmp("safe", &command[6], 4) == 0) {
              mySwitch->safe();
            } else if (strncasecmp("staffsafe", &command[6], 9) == 0) {
              mySwitch->staffsafe();
            } else if (strncasecmp("randomize", &command[6], 9) == 0) {
              mySwitch->randomize();
              Serial.println(F("Posting switch settings to server..."));
              // Send the update to the field controller
              fieldController->initializeSwitchSettings(mySwitch->getAlliance(), mySwitch->getLHSAlliance(), mySwitch->getRHSAlliance(), Ethernet.localIP());
            }
          }
          Serial.println();
          Serial.println(F("Sending response"));
          // send a standard http response header
          client.println(F("HTTP/1.0 200 OK"));
          client.println(F("Connection: close"));
          client.println(F("Content-Length: 0"));
          client.println();
          client.stop();
        }
        else if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    Serial.println(F("Disconnected"));
  }
}
