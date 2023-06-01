/*
	This code allows an Arduino board to receive a series of characters
  from a connected device via a serial connection. It then converts
  each character into a sequence of individual bits and transmits them
  to another device using an LED. The LED blinks to represent the 
  transmitted bits. Essentially, it establishes a simple communication
  link between the Arduino and an external device.
  
  Sample sequence: 0123456789:;,><=!.?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]/\|{}^abcdefghijklmnopqrstuvwxyz~-+×÷Ø&§€$*%#µ-_àéèùæ
	
	Written by: 
  Hannes Flament & Sibald Hulselmans
  6IW
*/


// -----------------------------------  Include Libraries  ------------------------------ //

#include <TimerOne.h>


// ------------------  Define constants and variables and strings  ---------------------- //

const int ledPin = 5;                        // Pin number for the LED

const int frequency0 = 500;                  // Frequency for bit 0
const int frequency1 = 600;                  // Frequency for bit 1
const int frequencyInact = 100;                 // Inactive Frequency

const int bitDuration = 40;                         // Duration of each bit (milliseconds)

const unsigned long period0 = 0.5e6 / frequency0;             // Period for bit 0
const unsigned long period1 = 0.5e6 / frequency1;             // Period for bit 1
const unsigned long inactivePeriod = 0.5e6 / frequencyInact;  // Inactive period for the timer

float timeBegin;      // Start time of each loop iteration
float timeEnd;        // End time of each loop iteration
float delayDuration;  // Create a duration variable

String inputString = "";                     // Stores the received input string
bool stringComplete = false;                 // Flag indicating if a complete string is received


// ------------------  void setup()   ---------------------- //

void setup() {

  pinMode(ledPin, OUTPUT);                    // Set LED pin as output
  Serial.begin(230400);                       // Start serial communication

  Timer1.initialize(inactivePeriod);          // Initialize timer with inactivePeriod
  Timer1.attachInterrupt(blinkLED);           // Attach interrupt for LED blinking

}


// ------------------  void loop()   ---------------------- //

void loop() {
  timeBegin = millis();
//  Serial.println(timeBegin);                       
  
  if (stringComplete) {
    transmitString(inputString);               // Transmit the received string
    inputString = "";                          // Reset input string
    stringComplete = false;                    // Reset stringComplete flag
    Serial.println("done");
  }
}


// ------------------  void serialEvent   ---------------------- //

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;                      // Append character to inputString

    if (inChar == '\n') {
      stringComplete = true;                   // Set the stringComplete flag when end of line is reached
    }
  }
}


// ------------------  void transmitString()   ---------------------- //

void transmitString(String input) {
  for (int i = 0; i < input.length(); i++) {
    char character = input.charAt(i);
    
    for (int j = 7; j >= 0; j--) {
      bool bit = bitRead(character, j);
      transmitBit(bit);                         // Transmit each character as bits
    }
  }
  
  Timer1.setPeriod(inactivePeriod);             // Restore the original timer period
}


// ------------------  Variables   ---------------------- //

void transmitBit(bool bit) {
  if (bit) {
    Timer1.setPeriod(period1);                  // Set timer period for bit 1
  } else {
    Timer1.setPeriod(period0);                  // Set timer period for bit 0
  }
  delay(bitDuration);
}


// ------------------  void blinkLED()   ---------------------- //

void blinkLED() {
  static bool ledState = false;
  ledState = !ledState;

  if (ledState)                                // Set LED pin port directly HIGH
    PORTD |= (1 << PD5);
  
  else                                         // Set LED pin port directly LOW
    PORTD &= ~(1 << PD5);
  
}
