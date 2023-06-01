#include "Arduino.h"
#include "LVP1.h"

LPV1::LPV1();

String LPV1::convertProtocol(String inputString)
{
  _input = inputString;
  _input = "Test";
  return _input;
}
