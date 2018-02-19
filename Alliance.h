#ifndef H_ALLIANCE
#define H_ALLIANCE

enum class Alliance { RED = 1, BLUE = 0 };

class AllianceAsString
{
  public:
  static const char * const Is(Alliance alliance);
};

class AllianceSelector
{
  int alliancePin;      // the number of the pin to read alliance selected on the board
  Alliance alliance;

public:
  AllianceSelector(int pin);
  Alliance Is();
};

#endif // H_ALLIANCE

