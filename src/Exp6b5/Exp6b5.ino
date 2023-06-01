#define LEDPIN 6
#define POTPIN A1

bool ledState = LOW;
float interval = 0;
float potData = 0;
float freq = 0;

unsigned long previousMicors = 0;
unsigned long currentMicros = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LEDPIN, OUTPUT);
  pinMode(POTPIN, INPUT);
}

void loop() {

  potData = analogRead(POTPIN);
  
  interval = map(potData, 0, 1023, 100000.00, 250.00);      //Potwaarde:            0 --> 1023
                                                            //Intervalwaarde:       100 000 µs --> 250 µs
                                                            //Periode (2*interval): 200 000 µs --> 500 µs
                                                            //Frequentie:           5 Hz --> 2000 Hz

  currentMicros = micros();

  if (currentMicros - previousMicors >= interval) {
    previousMicors = currentMicros;

    ledState = !ledState;

    digitalWrite(LEDPIN, ledState);

    //  Serial.print((interval)*2);
    //  Serial.print(" ms | ");
    //  freq = 1000000.00/((interval)*2);
    //  Serial.print(freq);
    //  Serial.println(" Hz");
  }
}
