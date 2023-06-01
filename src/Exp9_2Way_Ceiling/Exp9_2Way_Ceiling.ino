// -----------------------------------  Include Libraries  ------------------------------ //

#include <TimerOne.h>


// ------------------  Define constants and variables and strings  ---------------------- //

const int ledPin = 5;    // Pin number for the LED
const int photoPin = 3;  // Pin number for the photodiode

const int frequencyIn0 = 500;     // Frequency for bit 0
const int frequencyIn1 = 600;      // Frequency for bit 1
const int frequencyInact = 100;     // Inactive Frequency
const int frequencyTolerance = 50;  // Inactive Frequency

const int bitDuration = 50;  // Duration of each bit (milliseconds)

const unsigned long periodIn0 = 0.5e6 / frequencyIn0;             // Period for bit 0
const unsigned long periodIn1 = 0.5e6 / frequencyIn1;             // Period for bit 1
const unsigned long inactivePeriodIn = 0.5e6 / frequencyInact;  // Inactive period for the timer

String inputString = "";      // Stores the received input string
String outputString = "";   // Stores the string to be transmitted

bool transmitting = false;  // Flag indicating if transmission is in progress
bool stringComplete = false;  // Flag indicating if a complete string is received

String byteString = "";  // Store the received byte string
String sentence = "";    // Store the complete sentence

int i = 0;               // Counter variable
int j = 0;
int k = 0;

int outputIndex = 0;        // Index of the current character being transmitted


// ------------------  void setup()   ---------------------- //

void setup() {
  pinMode(ledPin, OUTPUT);   // Set LED pin as output
  pinMode(photoPin, INPUT);  // Set the input pin for the photodiode
  Serial.begin(230400);      // Start serial communication

  Timer1.initialize(inactivePeriodIn);  // Initialize timer with inactivePeriod
  Timer1.attachInterrupt(blinkLED);   // Attach interrupt for LED blinking
}


// ------------------  void loop()   ---------------------- //

void loop() {
  receiveData();
  sendData();
}


// ------------------  void receiveData()   ---------------------- //

void receiveData() {

  float timeBegin = millis();

  float pulseHigh = pulseIn(photoPin, HIGH, inactivePeriodIn * 50);  // Measure the duration of the high pulse
  float pulseLow = pulseIn(photoPin, LOW, inactivePeriodIn * 50);    // Measure the duration of the low pulse

  float pulseTotal = pulseHigh + pulseLow;  // Calculate the total duration of the pulse
  float frequencyIn = 1000000 / pulseTotal;   // Calculate the frequency of the signal

  if (abs(frequencyIn - frequencyIn0) <= frequencyTolerance) {  // Compare the frequency to determine the bit value
    byteString += String(0);                // Append the bit value to the byte string

    if (byteString.length() == 1 && k == 0) {
      Serial.println("Receiving....");
      // Serial.println("");
      k = 1;
    }

    Serial.print("0");

  } else if (abs(frequencyIn - frequencyIn1) <= frequencyTolerance) {
    byteString += String(1);  // Append the bit value to the byte string

    if (byteString.length() == 1 && k == 0) {
      Serial.println("Receiving....");
      // Serial.println("");
      k = 1;
    }
    Serial.print("1");

  } else if (abs(frequencyIn - frequencyInact) <= 3 && byteString.length() > 0) {
    byteString = "";  // Reset the byte string if it's an inactive frequency
  }

  if (byteString.length() == 8) {  // If a complete byte string is received, convert it to a character and append to the sentence
    char character = strtol(byteString.c_str(), NULL, 2);
    sentence += character;
    byteString = "";  // Reset the byte string
    i = 1;
  }

  if (abs(frequencyIn - frequencyInact) <= 5 && i == 1) {  // If the frequency indicates an inactive state and a byte string has been processed, print the sentence
    Serial.println("");
    Serial.println("");
    Serial.print("Received sentence: ");
    Serial.println(sentence);
    sentence = "";  // Reset the sentence
    i = 0;
    k = 0;
  }

  float timeEnd = millis();  // Record the end time of the loop iteration

  float delayDuration = bitDuration - (timeEnd - timeBegin);

  if (delayDuration > 0) {
    delay(delayDuration);
  }
}

// ------------------  void sendData()   ---------------------- //

void sendData() {
  if (stringComplete && !transmitting) {
    outputString = inputString;  // Store the received string for transmission
    inputString = "";            // Reset input string
    stringComplete = false;      // Reset stringComplete flag
    transmitting = true;         // Set the transmitting flag to start transmission
    outputIndex = 0;             // Reset the output index
    // Timer1.setPeriod(period0);   // Set timer period for bit 0
  }

  while (transmitting) {
    if (outputIndex < outputString.length()) {

      char character = outputString.charAt(outputIndex);
      //  Serial.print("k");

      for (j = 7; j >= 0; j--) {
        bool bit = bitRead(character, j);
         float timeBegin = millis();

        if (bit) {
          Timer1.setPeriod(periodIn1);
          Serial.print("1");
        }

        else if (!bit) {
          Timer1.setPeriod(periodIn0);
          Serial.print("0");
        }

        float timeEnd = millis();  // Record the end time of the loop iteration

        float delayDuration = bitDuration - (timeEnd - timeBegin);

        if (delayDuration > 0) {
          delay(delayDuration);
        }
      }

      outputIndex++;
    }

    else {
      Timer1.setPeriod(inactivePeriodIn);  // Restore the original timer period
      transmitting = false;              // Set the transmitting flag to false
      outputString = "";
      Serial.println("");
      Serial.println("");
      Serial.println("Transmitting done");
    }
  }
}



// ------------------  void serialEvent()   ---------------------- //

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;  // Append character to inputString

    if (inChar == '\n') {
      stringComplete = true;  // Set the stringComplete flag when end of line is reached
    }
  }
  Serial.println("Sending.... ");
  Serial.print("Input sentence: ");
  Serial.println(inputString);
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
