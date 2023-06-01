#define PHOTOPIN 2

int Htime;                //integer for storing high time
int Ltime;                //integer for storing low time
float Ttime;              // integer for storing total time of a cycle
float freq;               //storing frequency

void setup(){
    pinMode(8,INPUT);

    Serial.begin(9600);
}

void loop(){
    Htime=pulseIn(PHOTOPIN, HIGH);      //read high time
    Ltime=pulseIn(PHOTOPIN, LOW);        //read low time

    Ttime = Htime+Ltime;

    freq=1000000/Ttime;    //getting freq with Ttime is in Micro seconds

    Serial.println(freq);

    delay(500);

}