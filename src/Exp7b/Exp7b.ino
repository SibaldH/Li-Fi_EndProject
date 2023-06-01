const int ledPin = 5;
const int freq1 = 1000; // frequency for binary digit 0
const int freq2 = 2000; // frequency for binary digit 1
const int bitDuration = 10; // duration of each bit in milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    String inputString = Serial.readStringUntil('\n');
    Serial.print("Input String: ");
    Serial.println(inputString);

    // Convert string to binary
    String binaryString = "";
    for (int i = 0; i < inputString.length(); i++) {
      char c = inputString[i];
      for (int j = 7; j >= 0; j--) {
        binaryString += ((c >> j) & 1) ? '1' : '0';
      }
      binaryString += " ";
    }
    Serial.print("Binary String: ");
    Serial.println(binaryString);

    // Transmit binary string using FSK
    for (int i = 0; i < binaryString.length(); i++) {
      char bit = binaryString[i];
      int freq = (bit == '0') ? freq1 : freq2;
      digitalWrite(ledPin, HIGH);
      tone(ledPin, freq, bitDuration);
      delay(bitDuration);
      digitalWrite(ledPin, LOW);
      delay(bitDuration);
    }
  }
}
