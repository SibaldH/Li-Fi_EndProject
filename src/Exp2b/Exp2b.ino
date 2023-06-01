#define LEDPIN 6

void setup () {
  pinMode(LEDPIN, OUTPUT);
}

void loop(){
  blink(2000);
}

void blink(int blink_delay){
  digitalWrite(LEDPIN, HIGH);
  delayMicroseconds(blink_delay);
  digitalWrite(LEDPIN, LOW);
  delayMicroseconds(blink_delay);
}