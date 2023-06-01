#define FOTOPIN A1

void setup () {

  pinMode(FOTOPIN, INPUT);
  
  Serial.begin(9600);
}

void loop () {
  int fotoData = analogRead(FOTOPIN);
  Serial.println(fotoData);
  
}