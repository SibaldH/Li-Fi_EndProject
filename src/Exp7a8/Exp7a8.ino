/*
  This code reads the frequency of an incoming pulse using a photoresistor
  connected to an Arduino board. It calculates the frequency based on the 
  pulse duration and compares it to predefined frequency values for bit 0 
  and bit 1. If the measured frequency falls within a certain tolerance 
  range of either frequency value, it determines the corresponding bit 
  value (0 or 1) and appends it to a byte string. When the byte string 
  reaches a length of 8 bits, it converts it to a character and adds it 
  to a sentence string. The sentence is then printed to the serial monitor. 
  The code also handles cases where the frequency indicates an inactive 
  period or an error. The timing of the code execution is controlled to 
  ensure a specific bit duration. Overall, this code is designed to decode 
  a series of pulses into a meaningful sentence.

  Written by: 
  Hannes Flament & Sibald Hulselmans
  6IW
*/


#include <TimerOne.h>

// ------------------  Define constants and variables and strings  ---------------------- //

const int ledPin = 5;    // Pin number for the LED
const int photoPin = 3;       // Define the input pin for the photodiode

int bitCounter = 0;          // Counter variable
int bitValue;       // Variable to hold received bit value

float pulseHigh;      // High time of incoming pulse
float pulseLow;       // Low time of incoming pulse

float pulseTotal;   // Total time of incoming pulse
float frequency;    // Calculated frequency of signal

float timeBegin;      // Start time of each loop iteration
float timeEnd;        // End time of each loop iteration
float delayDuration;  // Create a duration variable

const int frequencyTolerance = 50;      // Tolerance range for frequency comparison
const int frequency0 = 500;          // Frequency value for bit 0
const int frequency1 = 600;          // Frequency value for bit 1
const int frequencyInact = 100;         // Frequency value for inactive state

const unsigned long inactivePeriod = 0.5e6 / frequencyInact;  // Inactive period for the timer

const int bitDuration = 50;             // Duration of each bit (clock time)
const int missingSourceTimeout = 500;   // Timeout for pulseIn()

String byteString = "";       // Store the received byte string
String sentence = "";         // Store the complete sentence


// ------------------  void setup()   ---------------------- //

void setup() {
  pinMode(ledPin, OUTPUT);   // Set LED pin as output
  pinMode(photoPin, INPUT);  // Set the input pin for the photodiode
  Serial.begin(230400);      // Start serial communication

  Timer1.initialize(inactivePeriod);  // Initialize timer with inactivePeriod
  Timer1.attachInterrupt(blinkLED);   // Attach interrupt for LED blinking
}

// ------------------  void loop()   ---------------------- //

void loop() {

  timeBegin = millis();       // Record the start time of the loop iteration

  pulseHigh = pulseIn(photoPin, HIGH, missingSourceTimeout * 1000);   // Measure the duration of the high pulse
  pulseLow = pulseIn(photoPin, LOW, missingSourceTimeout * 1000);     // Measure the duration of the low pulse

  pulseTotal = pulseHigh + pulseLow;      // Calculate the total duration of the pulse
  frequency = 1000000 / pulseTotal;       // Calculate the frequency of the signal

  if (bitCounter % 9 != 8) {  // Skip reading each 9th bit
    if (abs(frequency - frequency0) <= frequencyTolerance) {      // Compare the frequency to determine the bit value
      bitValue = 0;
      byteString += String(bitValue);       // Append the bit value to the byte string
      Serial.print("0");
    } 
    else if (abs(frequency - frequency1) <= frequencyTolerance) {
      bitValue = 1;
      byteString += String(bitValue);       // Append the bit value to the byte string
      Serial.print("1");
    }
    else if (abs(frequency - frequencyInact) <= 3) {
      byteString = "";                     // Reset the byte string if frequency indicates inactive state
    }
  }

  if (byteString.length() == 8) {     // If the byte string is complete (8 bits), convert it to a character and add it to the sentence
    char character = strtol(byteString.c_str(), NULL, 2);
    sentence += character;
    byteString = "";                  // Reset the byte string
  }

  if (abs(frequency - frequencyInact) <= 3 && sentence.length() > 0) {   // If the frequency indicates inactive state and a byte string has been processed, print the sentence
    Serial.println("");
    Serial.print("Sentence: ");
    Serial.println(sentence);
    sentence = "";                    // Reset the sentence
  }

  bitCounter++;

  timeEnd = millis();                                     // Record the end time of the loop iteration

  delayDuration = bitDuration - (timeEnd - timeBegin);

  if (delayDuration > 0) {
    delay(delayDuration);
  }
}

void blinkLED() {
  static bool ledState = false;
  ledState = !ledState;

  if (ledState)  // Set LED pin port directly HIGH
    PORTD |= (1 << PD5);
  else  // Set LED pin port directly LOW
    PORTD &= ~(1 << PD5);
}

