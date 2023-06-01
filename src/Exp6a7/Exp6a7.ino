volatile unsigned long long pulseCount = 0;
volatile unsigned long lastPulseTime = 0;
volatile float frequency = 0.0;

void handleInterrupt()
{
  pulseCount++;
}

void setup()
{
  pinMode(2, INPUT);  // Assuming the photodiode signal is connected to digital pin 2
  attachInterrupt(digitalPinToInterrupt(2), handleInterrupt, RISING);  // Attach interrupt to pin 2
  Serial.begin(9600);
}

void loop()
{
  if (micros() - lastPulseTime >= 50000)  // Calculate frequency every 25 milliseconds
  {
    detachInterrupt(digitalPinToInterrupt(2));  // Disable interrupt during frequency calculation
    frequency = pulseCount;  // Multiply by 40 to convert pulses per millisecond to frequency in Hz
    pulseCount = 0;
    lastPulseTime = micros();
    attachInterrupt(digitalPinToInterrupt(2), handleInterrupt, RISING);  // Re-enable interrupt
    Serial.print("Freq.: ");
    Serial.print(frequency*20);
    Serial.println(" Hz");
  }
}

//write me a code in arduino IDE. There is an block-input signal with a changing frequency as input, this will contain a sentence converted into bits. When te signal is 100 Hz, no info is sent. When the signal is 666,66Hz with a tolerance of 75Hz, the bit is 0. When the signal is 833,33 Hz with 75 Hz tolerance, the signal is 1. You need to read the frequency in the most correct way, convert the bits into a string and print the sentence back out in the serial monitor. The signal is connected to pin 2. Avoid the FreqMeasure library.