// #include “MapFloat.h”

#define LEDPIN 6
#define POTPIN A1

void setup(){
  pinMode(LEDPIN, OUTPUT);
  pinMode(POTPIN, INPUT);
  
  Serial.begin(9600);
}

void loop(){
  float potData = analogRead(POTPIN);
  
  // Serial.print("pot data: ");
  //Serial.println(potData);  
  
  float delay_map = map(potData, 0, 1023, 0, 200.00);
  // Serial.print(" | mapped delay: ");

  // float delay_map = 5.00;

 Serial.print((delay_map));
 Serial.print(" | ");
  
  blink(delay_map);

  float freq = 1000.00/(delay_map);
   Serial.println(freq);
}

void blink(float blink_delay){
  digitalWrite(LEDPIN, HIGH);
  delay(blink_delay);
  digitalWrite(LEDPIN, LOW);
  delay(blink_delay);
}