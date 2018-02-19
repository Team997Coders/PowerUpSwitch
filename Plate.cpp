#include "Plate.h"

Plate::Plate(int pin)
{
  // This is a hideous kludge, but I have run out of time trying to figure out
  // how to get around this issue using C++ templates (which I know can be done).
  switch (pin) {
    case 5:
      FastLED.addLeds<NEOPIXEL, 5>(leds, NUM_LEDS);
      break;
    case 6:
      FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
      break;
    default:
      Serial.println(F("Pin not recognized for LEDs"));
      break;
  }
}

// Set the alliance OF THE PLATE, NOT THE SWITCH
void Plate::SetAlliance(Alliance alliance) {
  this->alliance = alliance;
}

void Plate::Update() {
  switch (this->state)
  {
    // For states that require pulsing...
    case PlateState::PULSINGFULLALLIANCECOLOR:
    case PlateState::PULSINGFULLPURPLE:
      Pulse();
      break;
  }
  FastLED.show();
}

void Plate::Pulse() {
  unsigned long currentMillis = millis();
  // check to see if it's time to change the state of the LED for pulsing
  if (currentMillis - previousMillis >= fadeEvery)
  {
    brightness = brightness + fadeAmount;
    previousMillis = currentMillis;         // Remember the time
    if (brightness < 0)
      brightness = 0;
    if (brightness > 255)
      brightness = 255;
    if (brightness == 0 || brightness == 255)
    {
      fadeAmount = fadeAmount * -1;
    }
    CRGB color;
    if (this->state == PlateState::PULSINGFULLALLIANCECOLOR)
      color = alliance == Alliance::RED ? CRGB::Red : CRGB::Blue;
    else
      color = CRGB::Purple;
    fill_solid(leds, NUM_LEDS, color);
    fadeLightBy(leds, NUM_LEDS, brightness == 0 ? brightness : brightness - 1);
  }  
}

void Plate::SolidFullBlack() {
  this->state = PlateState::SOLIDFULLBLACK;
  // All black...maybe do some red/blue chaser and fade to black
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void Plate::SolidFullGreen() {
  this->state = PlateState::SOLIDFULLGREEN;
  fill_solid(leds, NUM_LEDS, CRGB::Green);
}

void Plate::PulsingFullPurple() {
  this->state = PlateState::PULSINGFULLPURPLE;
  fill_solid(leds, NUM_LEDS, CRGB::Purple);
  fadeAmount = abs(fadeAmount);
  brightness = fadeAmount;
  previousMillis = 0;
}

void Plate::SolidFullAllianceColor() {
  this->state = PlateState::SOLIDFULLALLIANCECOLOR;
  CRGB color = alliance == Alliance::RED ? CRGB::Red : CRGB::Blue;
  fill_solid(leds, NUM_LEDS, color);
}

void Plate::PulsingFullAllianceColor() {
  this->state = PlateState::PULSINGFULLALLIANCECOLOR;
  CRGB color = alliance == Alliance::RED ? CRGB::Red : CRGB::Blue;
  fill_solid(leds, NUM_LEDS, color);
  fadeAmount = abs(fadeAmount);
  brightness = fadeAmount;
  previousMillis = 0;
}

void Plate::SolidDimmedAllianceColor() {
  this->state = PlateState::SOLIDDIMMEDALLIANCECOLOR;
  CRGB color = alliance == Alliance::RED ? CRGB::Red : CRGB::Blue;
  fill_solid(leds, NUM_LEDS, color);
  fadeLightBy(leds, NUM_LEDS, 200);
}

Alliance Plate::GetAlliance() {
  return alliance;
}

