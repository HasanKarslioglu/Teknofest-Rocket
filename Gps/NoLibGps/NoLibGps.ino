#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(4,3);

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  if(gpsSerial.available()){
    Serial.write(gpsSerial.read());
  }
  // Alttakini yorum satırına al!
  if(Serial.available()){
    gpsSerial.write(Serial.read());
  }
}
