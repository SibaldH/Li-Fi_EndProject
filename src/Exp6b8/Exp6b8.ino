#include <TimerOne.h>  // include Timer1 library

#define LED_PIN 5     // LED pin number
#define POTPIN A1 // Button pin number

const int bit0 = 50000;     // 750
const int bit1 = 25000;     // 600


void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);  // Set pin 5 as an output pin
  pinMode(POTPIN, INPUT);
  Timer1.initialize(bit0);
  Timer1.attachInterrupt(blinkLED);
}

void loop() {
  if (analogRead(POTPIN) >= 512){
    Timer1.setPeriod(bit0);
  } else {
    Timer1.setPeriod(bit1);
  }
}

void blinkLED() {
  static boolean ledState = LOW;    // current LED state
  ledState = !ledState;             // toggle the LED state
  digitalWrite(LED_PIN, ledState);  // update the LED state
}