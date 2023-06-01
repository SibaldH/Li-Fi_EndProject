#include <TimerOne.h>

////////////////////// Mutual /////////////////////

const int frequencyInact = 100;  // Frequency value for inactive state
const int bitDuration = 40;      // Duration of each bit (clock time)
bool isReading = false;

//////////////////////// Zender /////////////////////////////////

const int ledPin = 5;  // Pin number for the LED

const int frequencyOut0 = 600;  // Frequency for bit 0
const int frequencyOut1 = 800;  // Frequency for bit 1

const unsigned long period0 = 0.5e6 / frequencyOut0;          // Period for bit 0
const unsigned long period1 = 0.5e6 / frequencyOut1;          // Period for bit 1
const unsigned long inactivePeriod = 0.5e6 / frequencyInact;  // Inactive period for the timer

float timeBeginOut;      // Start time of each loop iteration
float timeEndOut;        // End time of each loop iteration
float delayDurationOut;  // Create a duration variable

String inputString = "";  // Stores the received input string
bool stringComplete = false;

////////////////////////// Ontvanger //////////////////////////////////////

const int PhotoPin = 3;  // Define the input pin for the photodiode

int i = 0;     // Counter variable
int bitValue;  // Variable to hold received bit value

float pulseHigh;  // High time of incoming pulse
float pulseLow;   // Low time of incoming pulse
float timeout = 1000000;

float pulseTotal;  // Total time of incoming pulse
float frequency;   // Calculated frequency of signal

float timeBegin;      // Start time of each loop iteration
float timeEnd;        // End time of each loop iteration
float delayDuration;  // Create a duration variable

const int frequencyTolerance = 75;  // Tolerance range for frequency comparison
const int frequencyIn0 = 1000;       // Frequency value for bit 0
const int frequencyIn1 = 1200;       // Frequency value for bit 1

const int missingSourceTimeout = 500;  // Timeout for pulseIn()

String byteString = "";  // Store the received byte string
String sentence = "";

void setup() {
  Serial.begin(230400);  // Start serial communication

  pinMode(PhotoPin, INPUT);  // Set the input pin for the photodiode
  pinMode(ledPin, OUTPUT);   // Set LED pin as output

  Timer1.initialize(inactivePeriod);  // Initialize timer with inactivePeriod
  Timer1.attachInterrupt(blinkLED);
}

void loop() {
  checkRead();
  if (isReading) {
    LiFiRead();
    isReading = false;
  } else {
    timeBegin = millis();

    if (stringComplete) {
      transmitString(inputString);  // Transmit the received string
      inputString = "";             // Reset input string
      stringComplete = false;       // Reset stringComplete flag
    }
  }
}

// ------------------  void checkRead   ---------------------- //

void checkRead() {
  getFreq();

  if (abs(frequency - frequencyIn0) <= frequencyTolerance) isReading = true;

  else if (abs(frequency - frequencyIn1) <= frequencyTolerance) isReading = true;

  else if (abs(frequency - frequencyInact) <= 3) isReading = false;
}

// ------------------  void serialEvent   ---------------------- //

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;  // Append character to inputString

    if (inChar == '\n') {
      stringComplete = true;  // Set the stringComplete flag when end of line is reached
    }
  }
}


// ------------------  void transmitString()   ---------------------- //

void transmitString(String input) {
  for (int i = 0; i < input.length(); i++) {
    char character = input.charAt(i);

    for (int j = 7; j >= 0; j--) {
      bool bit = bitRead(character, j);
      transmitBit(bit);  // Transmit each character as bits
    }
  }

  Timer1.setPeriod(inactivePeriod);  // Restore the original timer period
}


// ------------------  Variables   ---------------------- //

void transmitBit(bool bit) {
  if (bit) {
    Timer1.setPeriod(period1);  // Set timer period for bit 1
  } else {
    Timer1.setPeriod(period0);  // Set timer period for bit 0
  }
  delay(bitDuration);
}


// ------------------  void blinkLED()   ---------------------- //

void blinkLED() {
  static bool ledState = false;
  ledState = !ledState;

  if (ledState)  // Set LED pin port directly HIGH
    PORTD |= (1 << PD5);

  else  // Set LED pin port directly LOW
    PORTD &= ~(1 << PD5);
}

// ------------------  void getFreq   ---------------------- //

void getFreq() {
  pulseHigh = pulseIn(PhotoPin, HIGH, missingSourceTimeout * 1000);  // Measure the duration of the high pulse
  pulseLow = pulseIn(PhotoPin, LOW, missingSourceTimeout * 1000);    // Measure the duration of the low pulse

  // Serial.println(pulseHigh);
  // Serial.println(pulseLow);

  pulseTotal = pulseHigh + pulseLow;  // Calculate the total duration of the pulse
  frequency = 1000000 / pulseTotal;   // Calculate the frequency of the signal
                                      // Serial.println(frequency);
}

// ------------------  void read   ------------------------- //

void LiFiRead() {
  timeBegin = millis();  // Record the start time of the loop iteration

  getFreq();

  if (abs(frequency - frequencyIn0) <= frequencyTolerance) {  // Compare the frequency to determine the bit value

    bitValue = 0;
    byteString += String(bitValue);  // Append the bit value to the byte string
    Serial.print("Byte String: ");   // Print the byte string
    Serial.println(byteString);
    //byteString = "";
  }

  else if (abs(frequency - frequencyIn1) <= frequencyTolerance) {

    bitValue = 1;
    byteString += String(bitValue);  // Append the bit value to the byte string
    Serial.print("Byte String: ");   // Print the byte string
    Serial.println(byteString);
    //byteString = "";
  }

  else if (abs(frequency - frequencyInact) <= 3) {
    byteString = "";  // Reset the byte string if frequency indicates inactive state
  }

  //else {
  // Handle error case where frequency is not within the expected range
  //Serial.println("Error: Invalid frequency detected");
  //}

  if (byteString.length() == 8) {  // If the byte string is complete (8 bits), convert it to a character and add it to the sentence

    char character = strtol(byteString.c_str(), NULL, 2);
    sentence += character;
    //Serial.print("Byte String: ");        // Print the byte string
    //Serial.println(byteString);
    byteString = "";  // Reset the byte string
    i = 1;
  }

  if (abs(frequency - frequencyInact) <= 3 && i == 1) {  // If the frequency indicates inactive state and a byte string has been processed, print the sentence

    Serial.print("Sentence: ");
    Serial.println(sentence);
    sentence = "";  // Reset the sentence
    i = 0;
  }

  timeEnd = millis();  // Record the end time of the loop iteration

  delayDuration = bitDuration - (timeEnd - timeBegin);

  if (delayDuration > 0) {
    delay(delayDuration);
  }
}