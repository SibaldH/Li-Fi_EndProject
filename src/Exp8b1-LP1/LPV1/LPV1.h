#ifndef LPV1_h
#define LPVI_h

#include "Arduino.h"

class LPV1
{
  public:
    LPV1();
    String convertProtocol(String input);
  private:
    String _input;
};

#endif