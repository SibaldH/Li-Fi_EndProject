int pulseHigh;     // Integer variable to capture High time of the incoming pulse
int pulseLow;      // Integer variable to capture Low time of the incoming pulse
float pulseTotal;  // Float variable to capture Total time of the incoming pulse
float frequency;   // Calculated Frequency
char bit = "";      // A bit received from the Photo Diode
char receivedByte[] = {};     // A string to present the 8 bits recieved
int clock = 20;    // time in micro seconds

const int pulsePin = 2;  // Input signal connected to Pin 8 of Arduino

void setup() {
  pinMode(pulsePin, INPUT);

  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  pulseHigh = pulseIn(pulsePin, HIGH);
  pulseLow = pulseIn(pulsePin, LOW);

  pulseTotal = pulseHigh + pulseLow;  // Time period of the pulse in microseconds
  frequency = 1000000 / pulseTotal;   // Frequency in Hertz (Hz)
  if (frequency > 600) { 
    signalToBit(frequency); 
  } else {
    Serial.println("Idle");
  }
}

void signalToBit(int freq) {
  if(freq > 750 ){
    bit = "1";
  } else {
    bit = "0";
  }
  receivedByte += bit;
  Serial.println(receivedByte);
  delayMicroseconds(clock);
}