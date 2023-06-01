#define LEDPIN 5
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
  
  if (potData <= 100)                                        //Potwaarde:            0 --> 1023
        interval = 100000;                 // 5 Hz           //Intervalwaarde:       100 000 µs --> 250 µs
                                                             //Periode (2*interval): 200 000 µs --> 500 µs
  else if (potData > 100 && potData <= 200)                          //Frequentie:           5 Hz --> 2000 Hz   
        interval = 50000;                  // 10 Hz

  else if (potData > 200 && potData <= 300)                                           
        interval = 10000;                  // 50 Hz

  else if (potData > 300 && potData <= 400)                                           
        interval = 5000;                   // 100 Hz

  else if (potData > 400 && potData <= 500)                                           
        interval = 1000;                  // 500 Hz

  else if (potData > 500 && potData <= 600)                                           
        interval = 750;                    // 666,66 Hz

  else if (potData > 600 && potData <= 700)                                           
        interval = 666,67;                // 750 Hz

  else if (potData > 700 && potData <= 800)                                           
        interval = 600;                // 833,33 Hz

  else if (potData > 800 && potData <= 900)                                           
        interval = 500;                // 1000 Hz

  else if (potData > 900 && potData <= 1000)                                            
        interval = 333,33;              // 1500 Hz
  
  else if (potData > 1000)                                         
        interval = 250;                // 2000 Hz

// Serial.println(interval);

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
