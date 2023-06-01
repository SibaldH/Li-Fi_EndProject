// Define the input pin

int inputPin = 2;

int i = 0;

int bitValue;

int pulseHigh; // Integer variable to capture High time of the incoming pulse
int pulseLow; // Integer variable to capture Low time of the incoming pulse

float pulseTotal; // Float variable to capture Total time of the incoming pulse
float frequency; // Calculated Frequency

const int frequencyTolerance = 75;
const int frequency0 = 666.66;
const int frequency1 = 833.33;
const int frequencyInact = 100;

String byteString = ""; // Define a variable to store the byte string
String sentence = ""; // Define a variable to store the sentence

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the frequency from the serial monitor
  if (Serial.available() > 0) {
    frequency = Serial.parseFloat();
  }

  // Check if the frequency is within the tolerance range for bit 0 or bit 1
  if (abs(frequency - frequency0) <= frequencyTolerance) {
    bitValue = 0;
    byteString += String(bitValue);
  } else if (abs(frequency - frequency1) <= frequencyTolerance) {
    bitValue = 1;
    byteString += String(bitValue);
  } else if (abs(frequency - frequencyInact) <= 3) {
    // Do nothing if the frequency is 100 Hz (no info sent)
    byteString = "";
  } else {
    // Log an error if the frequency is not within the expected range
    //Serial.println("Error: Invalid frequency detected");
    bitValue = NULL;
    return;
  }

  // Add the bit value to the byte string


  // If the byte string is 8 bits long, convert it to a character and add it to the sentence

  Serial.print("Byte String: ");    // Print the byte string to the serial monitor
  Serial.println(byteString);

  if (byteString.length() == 8) {
    char character = strtol(byteString.c_str(), NULL, 2);
    sentence += character;
    byteString = "";
    i = 1;
  }

  if (abs(frequency - frequencyInact) <= 3 && i == 1){
    Serial.print("Sentence: ");   // Print the sentence to the serial monitor
    Serial.println(sentence);
    i = 0;
  }
}