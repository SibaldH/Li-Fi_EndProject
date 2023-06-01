/* Te doen in deze code

- missingSourceTimeout minimum checken
- bitDuration minimum checken
- PORTD of digitalWrite nu beter?
- Werkt code volledig?
- Alles verklaring geven
- Tekst inleidingen schrijven

*/


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

const int frequencyOut0 = 700;  // Frequency for bit 0
const int frequencyOut1 = 800;  // Frequency for bit 1

const unsigned long period0 = 0.5e6 / frequencyOut0;          // Period for bit 0
const unsigned long period1 = 0.5e6 / frequencyOut1;          // Period for bit 1
const unsigned long inactivePeriod = 0.5e6 / frequencyInact;  // Inactive period for the timer

String InputString = "";  // Stores the received input string

bool stringComplete = false;

// - - - - - - - - - -   Receiever   - - - - - - - - - - - - //

const int PhotoPin = 3;  // Define the input pin for the photodiode

int i = 0;  // Counter variable
int j = 0;
int k = 0;
int bitValue;  // Variable to hold received bit value

float pulseHigh;  // High time of incoming pulse
float pulseLow;   // Low time of incoming pulse

float pulseTotal;  // Total time of incoming pulse
float frequency;   // Calculated frequency of signal

const int frequencyTolerance = 50;  // Tolerance range for frequency comparison
const int frequencyIn0 = 500;       // Frequency value for bit 0
const int frequencyIn1 = 600;       // Frequency value for bit 1

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

  if (isReading || j) {
    LiFiRead();
  }

  if (!isReading && stringComplete) {
    transmitString(InputString);  // Transmit the received string
    InputString = "";             // Reset input string
    stringComplete = false;       // Reset stringComplete flag
  }
}

// ------------------  void serialEvent   ---------------------- //

void serialEvent() {

  if (isReading == false) {

    while (Serial.available()) {

      char inChar = (char)Serial.read();

      if (inChar != '\n')
        InputString += inChar;  // Append character to InputString

      else if (inChar == '\n')
        stringComplete = true;  // Set the stringComplete flag when end of line is reached
    }
     Serial.print("Input Sentence: '");
     Serial.print(InputString);
     Serial.println("'");
  }
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
    if (k == 1) j = 1;
  }

  // else {
  //   // Serial.println("No expected frequency detected in CheckRead()");
  //   isReading = false;
  // }
}


// ------------------  void GetFreq   ---------------------- //

void GetFrequency() {

  pulseHigh = pulseIn(PhotoPin, HIGH, missingSourceTimeout);  // Measure the duration of the high pulse
  pulseLow = pulseIn(PhotoPin, LOW, missingSourceTimeout);    // Measure the duration of the low pulse

  // Serial.println(pulseHigh);
  // Serial.println(pulseLow);

  pulseTotal = pulseHigh + pulseLow;  // Calculate the total duration of the pulse
  frequency = 1e6 / pulseTotal;       // Calculate the frequency of the signal
  // Serial.println(frequency);
}


// ------------------  void LifiRead   ------------------------- //

void LiFiRead() {

  // timeBeginOut = millis();  // Record the start time of the loop iteration
  k = 1;

  GetFrequency();

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

  else {
    // Handle error case where frequency is not within the expected range
    // Serial.println("No expected frequency detected in LifiRead()");
  }

  if (byteString.length() == 8) {  // If the byte string is complete (8 bits), convert it to a character and add it to the sentence

    char character = strtol(byteString.c_str(), NULL, 2);
    OutputString += character;
    //Serial.print("Byte String: ");        // Print the byte string
    //Serial.println(byteString);
    byteString = "";  // Reset the byte string
    i = 1;
  }

  if (abs(frequency - frequencyInact) <= 3 && i == 1) {  // If the frequency indicates inactive state and a byte string has been processed, print the sentence

    Serial.println("");
    Serial.print("Output Sentence: ");
    Serial.println(OutputString);


    OutputString = "";  // Reset the sentence
    isReading = false;
    i = 0;
    j = 0;
    k = 0;
  }

  timeEnd = millis();  // Record the end time of the loop iteration

  delayDuration = bitDuration - (timeEnd - timeBegin);

  if (delayDuration > 0) {
    delay(delayDuration);
  }
}


// ------------------  void transmitString()   ---------------------- //

void transmitString(String input) {

  for (int l = 0; l < input.length(); l++) {
    char character = input.charAt(l);

    for (int m = 7; m >= 0; m--) {
      bool bit = bitRead(character, m);
      transmitBit(bit);  // Transmit each character as bits
    }
  }
  //Serial.println("");
  Timer1.setPeriod(inactivePeriod);  // Restore the original timer period
}


// ------------------  Variables   ---------------------- //

void transmitBit(bool bit) {

  if (bit){
    Timer1.setPeriod(period1);  // Set timer period for bit 1
   // Serial.print("1");
  }
  else{
    Timer1.setPeriod(period0);  // Set timer period for bit 0
   // Serial.print("0");
  }

  delay(bitDuration);
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