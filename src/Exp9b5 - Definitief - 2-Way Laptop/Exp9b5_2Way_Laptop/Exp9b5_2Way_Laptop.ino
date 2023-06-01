/*
	Final version of code for our end project enabeling 2 way-communication using frequency shift keying in Li-Fi technology.
  This code is disigned for the Laptop, sending 600 & 800 Hz, receiving 1000 & 1200 Hz.

  Photodiode: Pin D3,   Resistor 100 kΩ
  LED:        Pin D5,   Resistor 220 Ω
  
  Sample sequence: 0123456789:;,><=!.?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]/\|{}^abcdefghijklmnopqrstuvwxyz~-+×÷Ø&§€$*%#µ-_àéèùæ
	
	Written by: 
  Hannes Flament & Sibald Hulselmans
  6IW
*/


// -----------------------------------  Include Libraries  ------------------------------ //

#include <TimerOne.h>


// ------------------  Define constants and variables and strings  ---------------------- //


////////////////////// Mutual /////////////////////

const int frequencyInact = 100;               // Frequency value for inactive state
const int bitDuration = 40;                   // Duration of each bit (clock time)

bool transmitting = false;                    // Indication if transmission is in progress


//////////////////////// Transmitting /////////////////////////////////

const int ledPin = 5;                         // Digital pin number for the LED

const int frequencyOut0 = 600;               // Transmitting frequency for bit 0
const int frequencyOut1 = 800;               // Transmitting frequency for bit 1

int outputIndex = 0;                          // Index for the current character being transmitted

const unsigned long periodOut0 = 0.5e6 / frequencyOut0;               // Transmitting period for bit 0
const unsigned long periodOut1 = 0.5e6 / frequencyOut1;               // Transmitting period for bit 1
const unsigned long inactivePeriod = 0.5e6 / frequencyInact;          // Period value for inactive state

String inputString = "";                      // Stores the received input string
String outputString = "";                     // Stores the string to be transmitted

bool stringComplete = false;                  // Indication if a complete string is given


//////////////////////// Receiving /////////////////////////////////

const int photoPin = 3;                       // Digital pin number for the photodiode

const int frequencyIn0 = 1000;                 // Receiving frequency for bit 0
const int frequencyIn1 = 1200;                 // Receiving Frequency for bit 1
const int frequencyTolerance = 75;            // Tolerance receiving frequency

const int missingSourceTimeout = 250000;      // Timeout for pulseIn()

String byteString = "";                       // Stores the received byte string
String sentence = "";                         // Stores the complete received sentence

bool stringReceived = 0;                      // Indication if a complete string is received
bool startReceiving = 0;                      // Indication if receiving proces has started


// ------------------  void setup()   ---------------------- //

void setup() {

  pinMode(ledPin, OUTPUT);                    // Set LED pin as output
  pinMode(photoPin, INPUT);                   // Set photodiode pin as input

  Serial.begin(230400);                       // Start serial communication

  Timer1.initialize(inactivePeriod);          // Initialize timer with inactivePeriod
  Timer1.attachInterrupt(blinkLED);           // Attach interrupt for LED blinking
}


// ------------------  void loop()   ---------------------- //

void loop() {
  receiveData();
  sendData();
}


// ------------------  void receiveData()   ---------------------- //

void receiveData() {                          // Function for receiving data

  if (!transmitting){
  
    float timeBegin = millis();                 // Record the begin time of the loop iteration
                                              // This, later on, makes sure each bit is being received with the correct bitduration

    float pulseHigh = pulseIn(photoPin, HIGH, missingSourceTimeout);      // Measure the duration of the high pulse, incoming from the photodiode
    float pulseLow = pulseIn(photoPin, LOW, missingSourceTimeout);        // Measure the duration of the low pulse, incoming from the photodiode

    float pulseTotal = pulseHigh + pulseLow;    // Calculate the total duration of the pulse
    float frequencyIn = 1000000 / pulseTotal;   // Calculate the frequency of the incomming signal



    if (abs(frequencyIn - frequencyIn0) <= frequencyTolerance) {          // Compare the frequency to determine the bit value
    
      byteString += String(0);                                            // Add the bit value to the byte string

      if (byteString.length() == 1 && !startReceiving) {                  // If the bytestring contains info, and we did not already started receiving, 
                                                                          // we can print a signal to the serial monitor
        Serial.println("Receiving....");
        // Serial.println("");
        startReceiving = 1;

      }

      Serial.print("0");                                                  // Print a 0 to the serial monitor, to print back the byteString

    } else if (abs(frequencyIn - frequencyIn1) <= frequencyTolerance) {   // Compare the frequency to determine the bit value
    
      byteString += String(1);                                            // Add the bit value to the byte string

      if (byteString.length() == 1 && !startReceiving) {                  // If the bytestring contains info, and we did not already started receiving, 
                                                                        // we can print a signal to the serial monitor
        Serial.println("Receiving....");
        // Serial.println("");
        startReceiving = 1;

      }

      Serial.print("1");                                                            // Print a 1 to the serial monitor, to print back the byteString

    } else if (abs(frequencyIn - frequencyInact) <= 3 && byteString.length() > 0)   // Reset the byte string if it's an inactive frequency
    byteString = "";  


    if (byteString.length() == 8) {                                                 // If a complete byte string is received,
                                                                                  // we convert it to a character and add it to the sentence string
      char character = strtol(byteString.c_str(), NULL, 2);
      sentence += character;
      byteString = "";                                                              // Reset the byte string
      stringReceived = 1;

    }

    if (abs(frequencyIn - frequencyInact) <= 5 && stringReceived == 1) {   // If the frequency indicates an inactive state and a byte string has been processed,
                                                                        // we can print the sentence back out
      Serial.println("");
      Serial.println("");
      Serial.print("Received sentence: ");
      Serial.println(sentence);

      sentence = "";                                                       // Reset the sentence

      stringReceived = 0;
      startReceiving = 0;
    }

    float timeEnd = millis();                                              // Record the end time of the loop iteration

    float delayDuration = bitDuration - (timeEnd - timeBegin);             // Calculate the needed delay by subtracting the time for the program from the bitduration

    if (delayDuration > 0) delay(delayDuration);                           // Clock delay
  }
}


// ------------------  void sendData()   ---------------------- //

void sendData() {                                            // function for transmitting data

  if (stringComplete && !transmitting) {                     // If a input is given in the serial monitor, a setup block for transmitting is started

    outputString = inputString;                              // Store the received string for transmission
    inputString = "";                                        // Reset the input string

    stringComplete = false;                                  // Reset stringComplete
    transmitting = true;                                     // Set the transmitting indication to be true
    
    outputIndex = 0;                                         // Reset the output index
  }

  while (transmitting) {                                     // Open a loop to transmit each byte per byte
    
    if (outputIndex < outputString.length()) {

      char character = outputString.charAt(outputIndex);     // Extract the corresponding character out of the given sentence

      for (int j = 7; j >= 0; j--) {                         // Open a loop to transmit each bit per bit
        
        bool bit = bitRead(character, j);                    // Extract the corresponding bit out of the character

        float timeBegin = millis();                          // Record the begin time of the loop iteration
                                                             // This, later on, makes sure each bit is being transmitted with the correct bitduration

        if (bit) {                                           // If the extracted bit is a 1, set the frequency to the corresponding frequency
          Timer1.setPeriod(periodOut1);
          Serial.print("1");                                 // Print a 1 to the serial monitor, to print back the sentence in bits
        }

        else if (!bit) {                                     // If the extracted bit is a 0, set the frequency to the corresponding frequency
          Timer1.setPeriod(periodOut0);
          Serial.print("0");                                 // Print a 0 to the serial monitor, to print back the sentence in bits
        }

        float timeEnd = millis();                                       // Record the end time of the loop iteration

        float delayDuration = bitDuration - (timeEnd - timeBegin);      // Calculate the needed delay by subtracting the time for the program from the bitduration

        if (delayDuration > 0) delay(delayDuration);                    // Clock delay
      
      }
      outputIndex++;                                          // Add 1 to the index for extracting the corresponding character out of the given sentence
    }

    else {                                                    // If the sentence is fully transmitted, we can reset the led to its inactive state

      Timer1.setPeriod(inactivePeriod);                       // Restore the original timer period

      transmitting = false;                                   // Set the transmitting indication to false
      outputString = "";                                      // Reset the output string

      Serial.println("");                                     // Print out a transmitting verification in the serial monitor
      Serial.println("");
      Serial.println("Transmitting done");
      Serial.println("");
    }
  }
}


// ------------------  void serialEvent()   ---------------------- //

void serialEvent() {                                  // Interrupt function reading the serial monitor

  while (Serial.available()) {                        // Start the loop when sentence is given

    char inChar = (char)Serial.read();
    inputString += inChar;                            // Add character to inputString

    if (inChar == '\n') stringComplete = true;        // Set the stringComplete flag when end of sentence is reached

  }

  Serial.println("Sending.... ");                     // Print out a sending verification in the serial monitor
  Serial.print("Input sentence: ");
  Serial.println(inputString);
}

// ------------------  void blinkLED()   ---------------------- //

void blinkLED() {                           // Function to let the LED blink

  static bool ledState = false;
  ledState = !ledState;

  if (ledState)                             // Set LED pin port directly HIGH
    PORTD |= (1 << PD5);

  else                                      // Set LED pin port directly LOW
    PORTD &= ~(1 << PD5);
}
