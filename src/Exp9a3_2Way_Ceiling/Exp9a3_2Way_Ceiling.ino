// ------------------  Libraries   ---------------------- //

#include <TimerOne.h>

// ------------------  Variables   ---------------------- //

// - - - - - - - - - -   Mutual   - - - - - - - - - - - - //

const int frequencyInact = 100;  // Frequency value for inactive state
const int bitDuration = 40;      // Duration of each bit (clock time)

float timeBegin;      // Start time of each loop iteration
float timeEnd;        // End time of each loop iteration
float delayDuration;  // Create a duration variable

bool isReading = false;

// - - - - - - - - -   Transmitter   - - - - - - - - - - - //

const int ledPin = 5;  // Pin number for the LED

const int frequencyOut0 = 500;  // Frequency for bit 0
const int frequencyOut1 = 600;  // Frequency for bit 1

const unsigned long period0 = 0.5e6 / frequencyOut0;          // Period for bit 0
const unsigned long period1 = 0.5e6 / frequencyOut1;          // Period for bit 1
const unsigned long inactivePeriod = 0.5e6 / frequencyInact;  // Inactive period for the timer

// - - - - - - - - - -   Receiever   - - - - - - - - - - - - //

const int PhotoPin = 3;  // Define the input pin for the photodiode

int bitValue;  // Variable to hold received bit value

float frequency;  // Calculated frequency of signal

const int frequencyTolerance = 50;  // Tolerance range for frequency comparison
const int frequencyIn0 = 700;       // Frequency value for bit 0
const int frequencyIn1 = 800;       // Frequency value for bit 1

const int missingSourceTimeout = 24000;  // Timeout for pulseIn()

String byteString = "";  // Store the received byte string
String OutputString = "";

// ------------------  void setup   ---------------------- //

void setup() {
  Serial.begin(230400);  // Start serial communication

  pinMode(PhotoPin, INPUT);  // Set the input pin for the photodiode
  pinMode(ledPin, OUTPUT);   // Set LED pin as output

  Timer1.initialize(inactivePeriod);  // Initialize timer with inactivePeriod
  Timer1.attachInterrupt(blinkLED);
}

// ------------------  void loop   ---------------------- //

void loop() {
  timeBegin = millis();
  CheckRead();

  LiFiRead();
  Serial.println(isReading);
}

// ------------------  void GetFrequency   ---------------------- //

void GetFrequency() {
  float pulseHigh = pulseIn(PhotoPin, HIGH, missingSourceTimeout);  // Measure the duration of the high pulse
  float pulseLow = pulseIn(PhotoPin, LOW, missingSourceTimeout);    // Measure the duration of the low pulse

  float pulseTotal = pulseHigh + pulseLow;  // Calculate the total duration of the pulse
  frequency = 1e6 / pulseTotal;             // Calculate the frequency of the signal
}

// ------------------  void CheckRead   ---------------------- //

void CheckRead() {

  GetFrequency();

  if (abs(frequency - frequencyIn0) <= frequencyTolerance || abs(frequency - frequencyIn1) <= frequencyTolerance) {
    isReading = true;
    Timer1.setPeriod(1000);
  }


  else if (abs(frequency - frequencyInact) <= 3) {
    isReading = false;
  }
}

// ------------------  void serialEvent   ---------------------- //

void serialEvent() {
  CheckRead();

  if (isReading == false) {
    String InputString = "";

    while (Serial.available()) {

      char inChar = (char)Serial.read();

      if (inChar != '\n')
        InputString += inChar;  // Append character to InputString

      else if (inChar == '\n') {
        transmitString(InputString);
        InputString = "";
      }
    }
    Serial.print("Input Sentence: ");
    Serial.println(InputString);
  }
}

// ------------------  void transmitString()   ---------------------- //

void transmitString(String _input) {

  for (int l = 0; l < _input.length(); l++) {
    char character = _input.charAt(l);

    for (int m = 7; m >= 0; m--) {
      bool bit = bitRead(character, m);
      bit ? Timer1.setPeriod(period1) : Timer1.setPeriod(period0);
      delay(bitDuration);
    }
  }
  //Serial.println("");
  Timer1.setPeriod(inactivePeriod);  // Restore the original timer period
}

// ------------------  void blinkLED()   ---------------------- //

void blinkLED() {
  static bool ledState = false;
  ledState = !ledState;

  if (ledState && !isReading)  // Set LED pin port directly HIGH
    PORTD |= (1 << PD5);

  else  // Set LED pin port directly LOW
    PORTD &= ~(1 << PD5);
}

// ------------------  void LifiRead   ------------------------- //

void LiFiRead() {

  while (isReading) {

    GetFrequency();

    if (abs(frequency - frequencyIn0) <= frequencyTolerance) {  // Compare the frequency to determine the bit value

      byteString += "0";              // Append the bit value to the byte string
      Serial.print("Byte String: ");  // Print the byte string
      Serial.println(byteString);
    }

    else if (abs(frequency - frequencyIn1) <= frequencyTolerance) {

      byteString += "1";              // Append the bit value to the byte string
      Serial.print("Byte String: ");  // Print the byte string
      Serial.println(byteString);
    }

    if (byteString.length() == 8) {  // If the byte string is complete (8 bits), convert it to a character and add it to the sentence

      char character = strtol(byteString.c_str(), NULL, 2);
      OutputString += character;
      byteString = "";
    }

    if (abs(frequency - frequencyInact) <= 3) {  // If the frequency indicates inactive state and a byte string has been processed, print the sentence

      Serial.println("");
      Serial.print("Output Sentence: ");
      Serial.println(OutputString);

      byteString = "";
      OutputString = "";  // Reset the sentence
      isReading = false;
    }

    timeEnd = millis();  // Record the end time of the loop iteration

    delayDuration = bitDuration - (timeEnd - timeBegin);

    if (delayDuration > 0) {
      delay(delayDuration);
    }
  }
}
