#include <PS3BT.h>
#include <usbhub.h>

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

const int sensorPin = A5;

unsigned long lastTime = 0;
unsigned long time = 0;
int delaySeconds = 20000; // 20 second

void setup() 
{
  Serial.begin(9600);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  delay(100);
}

void loop()
{
  Usb.Task();
  
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getButtonClick(CROSS)) {
      irRead();
    }
  }
}

void irRead()
{
  time = millis();
  Serial.print(time);
  Serial.print(" | ");
  Serial.print(lastTime + delaySeconds);
  Serial.print(" -> ");
  if(time > (lastTime + delaySeconds)) {
    int reading = digitalRead(
    lastTime = time;
  } else {
    Serial.println("Sorry, delay not yet been met !! :P");
  }
}
