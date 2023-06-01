#include <ezOutput.h>

#define LEDPIN 6
#define POTPIN A1

ezOutput led(LEDPIN);

void setup() {
  pinMode(POTPIN, INPUT);
  pinMode(LEDPIN, OUTPUT);

  Serial.begin(9600);
}

void loop(){
  led.loop();

  // float potData = analogRead(POTPIN);

  // float delay_map = map(potData, 0, 1023, 0, 200.00);

  // Serial.print((delay_map));
  // Serial.print(" | ");

  unsigned long delay_map = 10.00;

  led.blink(delay_map);

  float freq = 1000.0/(delay_map);
  Serial.println(freq);
}