#define FOTOPIN A1
#define LEDPIN 6

void setup () {
  pinMode(LEDPIN, OUTPUT);
  pinMode(FOTOPIN, INPUT);
  
  Serial.begin(9600);
}

void loop () {
  int fotoData = analogRead(FOTOPIN);
  Serial.println(fotoData);
  
  // digitalWrite(LEDPIN, HIGH);

  blink(200);
}

void blink(int blink_delay){
  digitalWrite(LEDPIN, HIGH);
  delay(blink_delay);
  digitalWrite(LEDPIN, LOW);
  delay(blink_delay);
}