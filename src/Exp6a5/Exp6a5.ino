#include <FreqCount.h>
 
void setup(void) {
  FreqCount.begin(1000);

  Serial.begin(9600);
}
 
void loop() {
  
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
   
    Serial.println(count);
  }

}