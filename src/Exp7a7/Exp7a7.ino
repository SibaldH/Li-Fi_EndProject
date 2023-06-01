#include <TimerOne.h>

const int ledPin = 5;
const float freq0 = 666.66;
const float freq1 = 833.33;
const float freqNewByte = 400.0; // New frequency for a new byte
const unsigned long period0 = 0.5e6 / freq0;
const unsigned long period1 = 0.5e6 / freq1;
const unsigned long periodNewByte = 0.5e6 / freqNewByte;
const float inactivePeriod = 0.5e4;
const unsigned long bitDuration = 50; // 50 milliseconds per bit

String inputString = "";
bool stringComplete = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  Timer1.initialize(inactivePeriod);
  Timer1.attachInterrupt(blinkLED);
}

void loop() {
  if (stringComplete) {
    transmitString(inputString);
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void transmitString(String input) {
  transmitBit(true); // Start bit
  for (int i = 0; i < input.length(); i++) {
    char character = input.charAt(i);
    for (int j = 7; j >= 0; j--) {
      bool bit = bitRead(character, j);
      transmitBit(bit);
    }
    transmitBit(true); // New byte control bit
  }
  transmitBit(false); // Stop bit
  transmitBit(true); // New byte control bit
  Timer1.setPeriod(inactivePeriod);  // Restore original period 
}

void transmitBit(bool bit) {
  if (bit) {
    Timer1.setPeriod(period1);
  } else {
    Timer1.setPeriod(period0);
  }
  delay(bitDuration);
}

void blinkLED() {
  static bool ledState = false;
  ledState = !ledState;

  if (ledState) {
    // Set LED pin HIGH
    PORTD |= (1 << PD5);
  } else {
    // Set LED pin LOW
    PORTD &= ~(1 << PD5);
  }
}
