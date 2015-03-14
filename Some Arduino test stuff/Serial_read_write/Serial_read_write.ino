#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 7); // RX, TX

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(4800);
  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
}

void loop() // run over and over
{
//  if (mySerial.available())
//    Serial.write(mySerial.read());
  if (Serial.available()) {
    mySerial.write(Serial.read());
    Serial.write(Serial.read());
  }
}
