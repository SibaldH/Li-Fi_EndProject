// Define the input pin for the block-input signal
const int inputPin = 2;

// Define the tolerance for frequency detection
const int frequencyTolerance = 75;

// Define the expected frequencies for 0 and 1 bits
const int frequency0 = 666.66;
const int frequency1 = 833.33;

// Define the size of the message buffer
const int messageBufferSize = 100;

// Define a buffer to hold the received message
char messageBuffer[messageBufferSize];

// Define a variable to track the length of the received message
int messageLength = 0;

// Define variables for pulse measurement
const int pulsePin = 2;
unsigned long pulseHigh;
unsigned long pulseLow;
float pulseTotal;
float frequency;

// Define the time between sending each bit (in milliseconds)
const int bitDelay = 50;

void setup() {
  // Set up serial communication
  Serial.begin(9600);

  // Set up the input pin as an input
  pinMode(inputPin, INPUT);
}

void loop() {
  // Read the frequency of the input signal
  pulseHigh = pulseIn(inputPin, HIGH);
  pulseLow = pulseIn(inputPin, LOW);
  pulseTotal = pulseHigh + pulseLow;
  frequency = 1000000 / pulseTotal;

  // If the frequency is within the tolerance of frequency0, add a 0 bit to the message buffer
  if (abs(frequency - frequency0) <= frequencyTolerance) {
    messageBuffer[messageLength++] = '0';
    Serial.print("0");
  }

  // If the frequency is within the tolerance of frequency1, add a 1 bit to the message buffer
  else if (abs(frequency - frequency1) <= frequencyTolerance) {
    messageBuffer[messageLength++] = '1';
    Serial.print("1");
  }

  // If the frequency is not within the tolerance of either frequency0 or frequency1, ignore it

  // If the message buffer contains a full byte, convert it to a character and print it out
  if (messageLength == 8) {
    char byte = 0;
    for (int i = 0; i < 8; i++) {
      if (messageBuffer[i] == '1') {
        byte |= (1 << i);
      }
    }
    Serial.print(byte);
    messageLength = 0;
  }

  // Delay to allow for a consistent time between sending each bit
  delay(bitDelay);
}