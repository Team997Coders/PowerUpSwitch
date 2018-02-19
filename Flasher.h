#ifndef H_FLASHER
#define H_FLASHER

class Flasher
{
  // Class Member Variables
  // These are initialized at startup
  int ledPin;      // the number of the LED pin
  long onTime;     // milliseconds of on-time
  long offTime;    // milliseconds of off-time
  bool enabled;    // Should the flasher flash?

  // These maintain the current state
  int ledState;                 // ledState used to set the LED
  unsigned long previousMillis;   // will store last time LED was updated

  public:
  Flasher(int pin, long on, long off);
  void Enable();
  void Disable();
  void AlwaysOn();
  void Update();
};

#endif // H_FLASHER

