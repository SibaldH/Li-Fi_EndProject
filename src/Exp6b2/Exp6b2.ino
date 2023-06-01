#define LEDPIN 6

void setup() {
  pinMode(LEDPIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  for (unsigned long delay = 5000; delay >= 100; delay--) {
    blink(delay);
    Serial.println((2*delay));
  }
}

void blink(unsigned long blink_delay) {
  digitalWrite(LEDPIN, HIGH);
  delayMicroseconds(blink_delay);
  digitalWrite(LEDPIN, LOW);
  delayMicroseconds(blink_delay);
}