#include <TimerOne.h>

const int ledPin = 5;  // Pin number for the LED

const int frequency0 = 666.66;   // Frequency for bit 0
const int frequency1 = 833.33;   // Frequency for bit 1
const int frequencyInact = 100;  // Inactive Frequency

const int bitDuration = 40;  // Duration of each bit (milliseconds)

const unsigned long period0 = 0.5e6 / frequency0;             // Period for bit 0
const unsigned long period1 = 0.5e6 / frequency1;             // Period for bit 1
const unsigned long inactivePeriod = 0.5e6 / frequencyInact;  // Inactive period for the timer

const String SOH = "00000001";
const String EOT = "00000100";
const String ETB = "00010111";
const String OK = "00000111";

String PMA[] = { "" };
String MSB = "";
String input = "";  // Stores the received input string

bool stringComplete = false;  // Flag indicating if a complete string is received
bool transmitActive = false;


void setup() {
  Serial.begin(230400);
  pinMode(ledPin, OUTPUT);  // Set LED pin as output

  Timer1.initialize(inactivePeriod);  // Initialize timer with inactivePeriod
  Timer1.attachInterrupt(blinkLED);   // Attach interrupt for LED blinking
}

void loop() {


  if (stringComplete) {
    transmitActive = true;
    inputToPackage(input);
    for (int i = 0; i < input.length(); i++) Serial.println(PMA[i]);
    input = "";
    stringComplete = false;
    transmitActive = false;
  }
}


void serialEvent() {
  while (Serial.available() && !transmitActive) {
    char inChar = (char)Serial.read();
    input += inChar;  // Append character to inputString

    if (inChar == '\n') {
      stringComplete = true;  // Set the stringComplete flag when end of line is reached
    }
  }
}

void inputToPackage(String _input) {
  for (int j = 0; j < 2; j++) {  // 2 bytes per package
    String byte = "";

    for (int i = 0; i < _input.length() - 1; i++) {  // get every character
      char character = _input.charAt(i);
      String package = "";

      for (int k = 7; k >= 0; k--) {
        bool bit = bitRead(character, k);
        byte += bit;
      }
      package += byte;
    }
    PMA[j] = package;
    // Serial.print(package);
  }
  // Serial.println("");
}

void blinkLED() {
  static bool ledState = false;
  ledState = !ledState;

  if (ledState)  // Set LED pin port directly HIGH
    PORTD |= (1 << PD5);

  else  // Set LED pin port directly LOW
    PORTD &= ~(1 << PD5);
}