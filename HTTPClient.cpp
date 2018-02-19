#include <Arduino.h>
#include "HTTPClient.h"
#include "BufferFiller.h"

byte HTTPClient::Post(
  IPAddress ipAddress, 
  int thisPort, 
  char* page, 
  char* thisData)
{
  int inChar;
  char outBuf[64];    // This buffer is small to preserve stack space...must conserve
  BufferFiller bfill(outBuf, sizeof(outBuf));

  Serial.print(F("connecting to "));

  bfill.emit_p(PSTR("$D.$D.$D.$D"), ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]); 

  Serial.print(outBuf);
  Serial.print(F("..."));
  
  if(client.connect(ipAddress, thisPort) == 1)
  {
    Serial.println(F("connected"));

    // send the header

    // POST header string
    bfill.reset();
    bfill.emit_p(
      PSTR(
        "POST $S HTTP/1.1"
      ),
      page
    );
    client.println(outBuf);

    // HOST header string
    bfill.reset();
    bfill.emit_p(
      PSTR(
        "Host: $D.$D.$D.$D"
      ),
      ipAddress[0],
      ipAddress[1],
      ipAddress[2],
      ipAddress[3]
    );
    client.println(outBuf);

    // Connection and content-type header string
    client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));

    // Content-length string
    bfill.reset();
    bfill.emit_p(
      PSTR(
        "Content-Length: $D\r\n"
      ),
      strlen(thisData)      
    );      
    client.println(outBuf);

    // send the body (variables)
    client.print(thisData);
  } 
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  int connectLoop = 0;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      Serial.write(inChar);
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if(connectLoop > 10000)
    {
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
  }

  Serial.println();
  Serial.println(F("disconnecting."));
  client.stop();
  return 1;
};

/*
byte HTTPClient::Post(
  IPAddress ipAddress, 
  int thisPort, 
  char* page, 
  char* thisData)
{
  int inChar;
  char outBuf[64];
  char server[15];

  Serial.print(F("connecting to "));

  sprintf(server, "%d.%d.%d.%d", ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]);

  Serial.print(server);
  Serial.print(F("..."));
  
  if(client.connect(ipAddress, thisPort) == 1)
  {
    Serial.println(F("connected"));

    // send the header
    sprintf(outBuf, "POST %s HTTP/1.1", page);
    client.println(outBuf);
    sprintf(outBuf, "Host: %s", server);
    client.println(outBuf);
    client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
    sprintf(outBuf, "Content-Length: %u\r\n", strlen(thisData));
    client.println(outBuf);

    // send the body (variables)
    client.print(thisData);
  } 
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  int connectLoop = 0;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      Serial.write(inChar);
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if(connectLoop > 10000)
    {
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
  }

  Serial.println();
  Serial.println(F("disconnecting."));
  client.stop();
  return 1;
};
*/
