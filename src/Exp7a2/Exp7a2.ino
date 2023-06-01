#include <TimerOne.h>

const byte photodiodePin = 3;
volatile long startTime = 0;
volatile long timeDifference;
volatile boolean receivingData = false;
String receivedBits = "";
String receivedMessage = "";

void setup() {
  Serial.begin(115200);
  pinMode(photodiodePin, INPUT);

  Timer1.initialize(50); // Frequency detection window in microseconds
  Timer1.attachInterrupt(timerIsr);

  attachInterrupt(digitalPinToInterrupt(photodiodePin), photodiodeIsr, CHANGE);
}

void loop() {
  if (receivedBits.length() >= 8) {
    char character = (char)strtol(receivedBits.substring(0, 8).c_str(), 0, 2);
    receivedMessage += character;
    receivedBits = receivedBits.substring(8);
  }

  // if (receivedMessage.length() > 0) {
  //   Serial.print(receivedMessage);
  //   receivedMessage = "";
  //   Serial.println("");
  // }
  
}

void photodiodeIsr() {
  if (receivingData) {
    return;
  }

  long currentTime = micros();
  timeDifference = currentTime - startTime;
  startTime = currentTime;
  
  if (timeDifference > 1000) {
    receivingData = true;
    Timer1.start();
  }
}

void timerIsr() {
  Timer1.stop();
  receivingData = false;

  if (timeDifference >= 1000 && timeDifference <= 1600) { // FSK frequency range
    if (timeDifference < 1300) { // 666.66 Hz (bit 0)
      receivedBits += '0';
    } else { // 833.33 Hz (bit 1)
      receivedBits += '1';
    }
  }
}