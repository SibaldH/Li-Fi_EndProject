// #include “MapFloat.h”

#define LEDPIN 6
#define POTPIN A1

void setup(){
  pinMode(LEDPIN, OUTPUT);
  pinMode(POTPIN, INPUT);
  
  Serial.begin(9600);
}

void loop(){
  int potData = analogRead(POTPIN);
  
  // Serial.print("pot data: ");
  // Serial.print(potData);  
  
  float delay_map = map(potData, 0, 1023, 0, 100.00);
  // Serial.print(" | mapped delay: ");
  Serial.print((2*delay_map));
  Serial.print(" | ");
  
  blink(delay_map);

  float freq = 1000.0/(2*delay_map);
  Serial.println(freq);
}

void blink(int blink_delay){
  digitalWrite(LEDPIN, HIGH);
  delay(blink_delay);
  digitalWrite(LEDPIN, LOW);
  delay(blink_delay);
}