#include <PS3BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);

int YmaxValue = 100;
int YminValue = 100;
int XmaxValue = 100;
int XminValue = 100;

void setup() {
  Serial.begin(9600);
  if (Usb.Init() == -1) {
    Serial.print("OSC did not start");
    while (1);
  }
  Serial.println("Ready for takeof");
}

void loop() {
  Usb.Task();
  
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getAnalogHat(RightHatY) > YmaxValue)
      YmaxValue = PS3.getAnalogHat(RightHatY);
    if (PS3.getAnalogHat(RightHatY) < YminValue)
      YminValue = PS3.getAnalogHat(RightHatY);
    if (PS3.getAnalogHat(RightHatX) > XmaxValue)
      XmaxValue = PS3.getAnalogHat(RightHatX);
    if (PS3.getAnalogHat(RightHatX) < XminValue)
      XminValue = PS3.getAnalogHat(RightHatX);
      
    Serial.print("X max: ");
    Serial.print(XmaxValue);
    Serial.print(" | X min: ");
    Serial.print(XminValue);
    Serial.print(" | Y max: ");
    Serial.print(YmaxValue);
    Serial.print(" | Y min: ");
    Serial.println(YminValue);
    delay(100);
  }
}
