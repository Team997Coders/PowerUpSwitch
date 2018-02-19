#include <Arduino.h>
#include "Alliance.h"

AllianceSelector::AllianceSelector(int pin)
{
  alliancePin = pin;
  pinMode(alliancePin, INPUT_PULLUP);
  if (digitalRead(alliancePin) == HIGH)
  {
    alliance = Alliance::RED;
  }
  else
  {
    alliance = Alliance::BLUE;
  }
}

Alliance AllianceSelector::Is()
{
  return alliance;
}

static const char * const AllianceAsString::Is(Alliance alliance) {
  return alliance == Alliance::BLUE ? "BLUE" : "RED";
}

