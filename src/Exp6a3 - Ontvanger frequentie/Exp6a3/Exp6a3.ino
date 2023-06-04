#define PHOTOPIN 3

float prevTime = 0;  // 1st timestamp to calculate Period
float currTime = 0;  // 2nd timestamp to calculate Period
float T = 0;         // Period
float f = 0;         // frequency


void updateT() {
  currTime = micros();
  T = (currTime - prevTime) / 0.500;
  prevTime = currTime;
}

void setup() {
  pinMode(PHOTOPIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PHOTOPIN), updateT, CHANGE);
  Serial.begin(9600);
}

void loop() {

  //println(Htime);
  // Serial.println(Ltime);
  // Serial.println(Ttime);

  // Serial.print("Period: ");
  // Serial.print(T);
  // Serial.print("μs | ");

  f = (1000000 / T);
  // Serial.print("Frequency: ");
  Serial.println(f);
  // Serial.println("Hz");

  // Serial.println("");

  //  delay(300);
}