#include <FreqMeasure.h>

const int signalPin = 2;
const double freqTolerance = 75.0;
const double freq0 = 666.66;
const double freq1 = 833.33;
const double noInfo = 100;

String decodedMessage = "";

void setup() {
  Serial.begin(115200);
  FreqMeasure.begin(signalPin, 1000000);
}

void loop() {
  if (FreqMeasure.available()) {
    double frequency = FreqMeasure.read() * 1000000 / FreqMeasure.countToFrequency(1);

    if (abs(frequency - noInfo) > freqTolerance) {
      if (abs(frequency - freq0) <= freqTolerance) {
        decodedMessage += "0";
      } else if (abs(frequency - freq1) <= freqTolerance) {
        decodedMessage += "1";
      }
    }

    if (decodedMessage.length() == 8) {
      char character = (char)strtol(decodedMessage.c_str(), 0, 2);
      Serial.print(character);
      decodedMessage = "";
    }
  }
}