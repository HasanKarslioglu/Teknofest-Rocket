#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); 
 
byte i = 0;

 
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  delay(1000);
  Serial.println("başlıyoruz.");
}
 
void loop() {

  mySerial.write((byte)0x00);
  mySerial.write((byte)0x01);
  mySerial.write((byte)0x14);
  mySerial.write(i);

  i++;
  delay(1000);
}
 

 