#include "FieldController.h"
#include "Utils.h"

FieldController::FieldController(char *ipAddress) {
  fieldControllerServer.fromString(ipAddress);
}

void FieldController::updateSwitch(Alliance forAlliance, bool lhsOwnsIt, bool rhsOwnsIt) {
  char outBuf[64];
  
  sprintf(outBuf, "alliance=%s&lhsownsit=%s&rhsownsit=%s", AllianceAsString::Is(forAlliance), BoolToString(lhsOwnsIt), BoolToString(rhsOwnsIt));

  httpClient.Post(
    fieldControllerServer,
    5000,
    "/api/switch",
    outBuf);
}

void FieldController::initializeSwitchSettings(Alliance forAlliance, Alliance lhs, Alliance rhs, IPAddress ip) {
  char outBuf[64];
  
  sprintf(outBuf, "Alliance=%s&LHS=%s&RHS=%s&IP=%d.%d.%d.%d", AllianceAsString::Is(forAlliance), AllianceAsString::Is(lhs), AllianceAsString::Is(rhs), ip[0], ip[1], ip[2], ip[3]);

  httpClient.Post(
    fieldControllerServer,
    5000,
    "/api/SwitchSettings",
    outBuf);
}

