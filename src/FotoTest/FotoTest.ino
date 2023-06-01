#define LEDPIN 6
#define FOTOPIN A0

void setup () {
  pinMode(LEDPIN, OUTPUT);
  pinMode(FOTOPIN, INPUT);
  
  Serial.begin(9600);
}

void loop () {
  int fotoData = analogeRead(FOTOPIN);
  Serial.println(FOTOPIN);
  
  blink(200);
}

void blink(delay){
  digitalWrite(LEDPIN, HIGH);
  delay(delay);
  digitalWrite(LEDPIN, LOW);
  delay(delay);
}