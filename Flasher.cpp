#include <Arduino.h>
#include "Flasher.h"

// Constructor - creates a Flasher 
// and initializes the member variables and state
Flasher::Flasher(int pin, long on, long off)
{
  ledPin = pin;
  pinMode(ledPin, OUTPUT);     
    
  onTime = on;
  offTime = off;
  
  ledState = LOW; 
  previousMillis = 0;
  enabled = true;
}

void Flasher::Enable()
{
  enabled = true;
  ledState = LOW;
  previousMillis = 0;
  digitalWrite(ledPin, ledState);
}

void Flasher::Disable()
{
  enabled = false;
  ledState = LOW;
  previousMillis = 0;
  digitalWrite(ledPin, ledState);
}

void Flasher::AlwaysOn()
{
  enabled = false;
  ledState = HIGH;
  previousMillis = 0;
  digitalWrite(ledPin, ledState);
}

void Flasher::Update()
{
  if (enabled == true)
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= onTime))
    {
      ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= offTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);   // Update the actual LED
    }
  }
}

