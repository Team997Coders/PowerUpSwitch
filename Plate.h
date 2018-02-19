#ifndef H_PLATE
#define H_PLATE

#include <Arduino.h>
#include "Alliance.h"
#include "FastLED.h"
#define NUM_LEDS 75

enum class PlateState { SOLIDFULLALLIANCECOLOR = 0, SOLIDDIMMEDALLIANCECOLOR = 1, PULSINGFULLALLIANCECOLOR = 2, SOLIDFULLGREEN = 3, PULSINGFULLPURPLE = 4, SOLIDFULLBLACK = 5 };

class Plate
{
  Alliance alliance;
  PlateState state;
  CRGB leds[NUM_LEDS];
  unsigned long previousMillis;   // will store last time LED strip was updated for fading
  long brightness;           // 
  int fadeAmount = 8;      // amount to fade by for each fade increment
  unsigned long fadeEvery = 10;   // number of millisecond ticks to bump the fade amount
  void Pulse();

  public:
  Plate(int pin);
  void SetAlliance(Alliance alliance);
  Alliance GetAlliance();
  void SolidFullAllianceColor();
  void SolidDimmedAllianceColor();
  void PulsingFullAllianceColor();
  void SolidFullGreen();
  void PulsingFullPurple();
  void SolidFullBlack();
  void Update();
};

#endif
