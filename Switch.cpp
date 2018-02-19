#include <Arduino.h>
#include "Switch.h"

Switch::Switch(int lhsSwitchPin, int rhsSwitchPin, int lhsLEDPin, int rhsLEDPin) : lhsPlate(Plate(lhsLEDPin)), rhsPlate(Plate(rhsLEDPin)) {
  // Set local variables for later use
  this->lhsSwitchPin = lhsSwitchPin;
  this->rhsSwitchPin = rhsSwitchPin;

  // Set up pins
  pinMode(lhsSwitchPin, INPUT_PULLUP);
  pinMode(rhsSwitchPin, INPUT_PULLUP);

  gameState = GameState::OFF;
  updateState();
  hasChanged = false;
}

void Switch::setAlliance(Alliance value) {
  alliance = value;
}

bool Switch::stateChanged() {
  return hasChanged;
}

void Switch::acknowledgeStateChanged() {
  hasChanged = false;
}

bool Switch::isLHSOwned() {
  return lhsOwned;
}

bool Switch::isRHSOwned() {
  return rhsOwned;
}

void Switch::updateState() {
  // HIGH = switch is open
  // LOW = switch is closed (and owned)

  bool lhsWasOwned;
  bool rhsWasOwned;

  lhsWasOwned = lhsOwned;
  rhsWasOwned = rhsOwned;
  
  if (digitalRead(lhsSwitchPin) == LOW) {
    lhsOwned = true;
    rhsOwned = false;
  }

  if (digitalRead(rhsSwitchPin) == LOW) {
    rhsOwned = true;
    lhsOwned = false;  
  }

  // This should never happen and if it does, then no one owns the switch
  if (digitalRead(lhsSwitchPin) == LOW && digitalRead(rhsSwitchPin) == LOW) {
    rhsOwned = false;
    lhsOwned = false;
  }

  if (digitalRead(lhsSwitchPin) == HIGH) {
    lhsOwned = false;
  }

  if (digitalRead(rhsSwitchPin) == HIGH) {
    rhsOwned = false;
  }

  if (lhsWasOwned != lhsOwned || rhsWasOwned != rhsOwned) {
    hasChanged = true;
  }
}

void Switch::updatePlates() {
  if (hasChanged) {
    switch (gameState)
    {
      case GameState::OFF:
        lhsPlate.SolidFullBlack();
        rhsPlate.SolidFullBlack();
        break;
      case GameState::PLAY:
        if (!lhsOwned && !rhsOwned) {
          lhsPlate.SolidFullAllianceColor();
          rhsPlate.SolidFullAllianceColor();
        }
        if (lhsOwned) {
          if (getLHSAlliance() == alliance) {
            lhsPlate.PulsingFullAllianceColor();
            rhsPlate.SolidFullAllianceColor();
          } else {
            rhsPlate.SolidDimmedAllianceColor();
            lhsPlate.SolidFullAllianceColor();
          }
        }
        if (rhsOwned) {
          if (getRHSAlliance() == alliance) {
            lhsPlate.SolidFullAllianceColor();
            rhsPlate.PulsingFullAllianceColor();
          } else {
            rhsPlate.SolidFullAllianceColor();
            lhsPlate.SolidDimmedAllianceColor();
          }        
        }
        break;
      case GameState::SAFE:
        lhsPlate.SolidFullGreen();
        rhsPlate.SolidFullGreen();
        break;
      case GameState::STAFFSAFE:
        lhsPlate.PulsingFullPurple();
        rhsPlate.PulsingFullPurple();
        break;
      case GameState::RANDOMIZE:
        lhsPlate.SolidFullAllianceColor();
        rhsPlate.SolidFullAllianceColor();
        break;
    }
  }
  lhsPlate.Update();
  rhsPlate.Update();
}

void Switch::randomize() {
  int randomizer;

  gameState = GameState::RANDOMIZE;
  hasChanged = true;

  randomizer = random(0, 2);
  if (randomizer == 1)
  {
    lhsPlate.SetAlliance(Alliance::RED);
    rhsPlate.SetAlliance(Alliance::BLUE);
  }
  else
  {
    rhsPlate.SetAlliance(Alliance::RED);
    lhsPlate.SetAlliance(Alliance::BLUE);
  }
  Serial.print(F("LHS = "));
  Serial.print(AllianceAsString::Is(lhsPlate.GetAlliance()));
  Serial.print(F("; RHS = "));
  Serial.println(AllianceAsString::Is(rhsPlate.GetAlliance()));
}

bool Switch::allianceSwitchIsOwnedByAlliance() {
  if (alliance == getLHSAlliance() && lhsOwned)
    return true;
  if (alliance == getRHSAlliance() && rhsOwned)
    return true;
  return false;
}

Alliance Switch::getAlliance() {
  return alliance;
}

Alliance Switch::getLHSAlliance() {
  return lhsPlate.GetAlliance();
}

Alliance Switch::getRHSAlliance() {
  return rhsPlate.GetAlliance();
}

void Switch::play() {
  gameState = GameState::PLAY;
  hasChanged = true;
}

void Switch::off() {
  gameState = GameState::OFF;
  hasChanged = true;
}

void Switch::safe() {
  gameState = GameState::SAFE;
  hasChanged = true;
}

void Switch::staffsafe() {
  gameState = GameState::STAFFSAFE;
  hasChanged = true;
}

GameState Switch::getGameState() {
  return gameState;
}

