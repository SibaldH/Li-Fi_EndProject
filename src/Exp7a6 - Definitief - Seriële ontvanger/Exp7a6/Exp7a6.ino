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


// ------------------  Define constants and variables and strings  ---------------------- //

const int PhotoPin = 3;       // Define the input pin for the photodiode

int i = 0;          // Counter variable
int bitValue;       // Variable to hold received bit value

float pulseHigh;      // High time of incoming pulse
float pulseLow;       // Low time of incoming pulse
float timeout = 1000000;

float pulseTotal;   // Total time of incoming pulse
float frequency;    // Calculated frequency of signal

float timeBegin;      // Start time of each loop iteration
float timeEnd;        // End time of each loop iteration
float delayDuration;  // Create a duration variable

const int frequencyTolerance = 50;      // Tolerance range for frequency comparison
const int frequency0 = 500;          // Frequency value for bit 0
const int frequency1 = 600;          // Frequency value for bit 1
const int frequencyInact = 100;         // Frequency value for inactive state

const int bitDuration = 40;             // Duration of each bit (clock time)
const int missingSourceTimeout = 500;   // Timeout for pulseIn()


String byteString = "";       // Store the received byte string
String sentence = "";         // Store the complete sentence


// ------------------  void setup()   ---------------------- //

void setup() {

  pinMode(PhotoPin, INPUT);   // Set the input pin for the photodiode
  Serial.begin(230400);       // Initialize serial communication
}


// ------------------  void loop()   ---------------------- //

void loop() {

  timeBegin = millis();       // Record the start time of the loop iteration

  /* if (timeBegin - timeEnd > missingSourceTimeout) {     // Reset important variables and strings if timeout
      byteString = "";
      sentence = "";
      i = 0;
    }
    */

  pulseHigh = pulseIn(PhotoPin, HIGH, missingSourceTimeout * 1000);   // Measure the duration of the high pulse
  pulseLow = pulseIn(PhotoPin, LOW, missingSourceTimeout * 1000);     // Measure the duration of the low pulse

 // Serial.println(pulseHigh);
 // Serial.println(pulseLow);

  pulseTotal = pulseHigh + pulseLow;      // Calculate the total duration of the pulse
  frequency = 1000000 / pulseTotal;       // Calculate the frequency of the signal
 // Serial.println(frequency);
   

  if (abs(frequency - frequency0) <= frequencyTolerance) {      // Compare the frequency to determine the bit value
    
    bitValue = 0;
    byteString += String(bitValue);       // Append the bit value to the byte string
    Serial.print("Byte String: ");        // Print the byte string
    Serial.println(byteString);
    //byteString = "";
  } 

  else if (abs(frequency - frequency1) <= frequencyTolerance) {
    
    bitValue = 1;
    byteString += String(bitValue);       // Append the bit value to the byte string
    Serial.print("Byte String: ");        // Print the byte string
    Serial.println(byteString);
    //byteString = "";
  }

  else if (abs(frequency - frequencyInact) <= 3) {
  byteString = "";                     // Reset the byte string if frequency indicates inactive state
  }

  //else {
    // Handle error case where frequency is not within the expected range
    //Serial.println("Error: Invalid frequency detected");
  //}

  if (byteString.length() == 8) {     // If the byte string is complete (8 bits), convert it to a character and add it to the sentence

    char character = strtol(byteString.c_str(), NULL, 2);
    sentence += character;
    //Serial.print("Byte String: ");        // Print the byte string
    //Serial.println(byteString);
    byteString = "";                  // Reset the byte string
    i = 1;
  }

  if (abs(frequency - frequencyInact) <= 3 && i == 1) {   // If the frequency indicates inactive state and a byte string has been processed, print the sentence

    Serial.print("Sentence: ");
    Serial.println(sentence);
    sentence = "";                    // Reset the sentence
    i = 0;
  }

  timeEnd = millis();                                     // Record the end time of the loop iteration

  delayDuration = bitDuration - (timeEnd - timeBegin);

  if (delayDuration > 0) {
    delay(delayDuration);
  }
}