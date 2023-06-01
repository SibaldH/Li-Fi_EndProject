#define IR_PIN 2  // Connect Photodiode to Pin 2.

unsigned int  now, start, capture[3], i, add = 0;
boolean flag_complete;



void setup()
{
  flag_complete = true;
  start = 0;
  now = 0;
  i = 0;
  
  Serial.begin(9600);  //Baud_Rate.
  delay(500);  // wait
  Serial.println("Press the button");
  attachInterrupt(0, Rec_Interrupt, CHANGE);  // Call the function, whenever
                                              // change in pulse is detected.
  
}

void loop()
{
  while (1) {
    if (flag_complete == 0) {
      for (i = 1; i < 3; i++) {
        if (i % 2 == 0)
        {
          Serial.print("LOW ");
          Serial.print(capture[i]);
          Serial.println(" microseconds");
        }

        else
        {
          Serial.print("HIGH ");
          Serial.print(capture[i]);
          Serial.println(" microseconds");
        }

        flag_complete = true;
      }
      for (int x = 1; x < 3; x++)
      {
        add += capture[x];  // Adding High Time and Low Time.
      }

      Serial.print("Period ");
      Serial.println(add);
      Serial.print("Frequency ");
      Serial.print((float)1000 / add, 5);
      Serial.println(" kHz");
      Serial.println(" ");
    }

    add = 0;  // clear 
  }
}


void Rec_Interrupt() {
  now = micros();                  // Capture current time.
  capture[i++] = now - start;      // Subtract Current time and Previous time.
  start = now;                     // Previous time is equal to current time.
  if (i >= 3)                      // If count is equal to or greater than 3
  {
    detachInterrupt(0);            // disable Interrupt 0
    flag_complete = false;         // clear flag.
  }
}