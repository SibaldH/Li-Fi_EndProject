#define PHOTOPIN 2

unsigned int now = 0;
unsigned int start = 0;
unsigned int periode = 0;
unsigned int freq = 0;

void setup() {
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(PHOTOPIN), Rec_Interrupt, CHANGE);
}

void loop() {
  Serial.print(periode);
  Serial.print(" | ");
  Serial.println(freq);
}


void Rec_Interrupt() {
  now = micros();
  periode = now - start;
  start = now;
  freq = 1000000/periode;
}