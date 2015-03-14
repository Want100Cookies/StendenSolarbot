/*
 Example sketch for the Wiimote Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <Wii.h>
#include <usbhub.h>

#include <SoftwareSerial.h>
 
SoftwareSerial BTserial(2, 3); // RX, TX

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
WII Wii(&Btd, PAIR); // This will start an inquiry and then pair with your Wiimote - you only have to do this once
//WII Wii(&Btd); // After that you can simply create the instance like so and then press any button on the Wiimote

bool printAngle;

void setup() {
  BTserial.begin(9600);
//  Serial.begin(115200);
//#if !defined(__MIPSEL__)
//  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
//#endif
  if (Usb.Init() == -1) {
    BTserial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  BTserial.print(F("\r\nWiimote Bluetooth Library Started"));
}
void loop() {
  Usb.Task();
  if (Wii.wiimoteConnected) {
    if (Wii.getButtonClick(HOME)) { // You can use getButtonPress to see if the button is held down
      BTserial.print(F("\r\nHOME"));
      Wii.disconnect();
    }
    else {
      if (Wii.getButtonClick(LEFT)) {
        Wii.setLedOff();
        Wii.setLedOn(LED1);
        BTserial.print(F("\r\nLeft"));
      }
      if (Wii.getButtonClick(RIGHT)) {
        Wii.setLedOff();
        Wii.setLedOn(LED3);
        BTserial.print(F("\r\nRight"));
      }
      if (Wii.getButtonClick(DOWN)) {
        Wii.setLedOff();
        Wii.setLedOn(LED4);
        Serial.print(F("\r\nDown"));
      }
      if (Wii.getButtonClick(UP)) {
        Wii.setLedOff();
        Wii.setLedOn(LED2);
        BTserial.print(F("\r\nUp"));
      }

      if (Wii.getButtonClick(PLUS))
        BTserial.print(F("\r\nPlus"));
      if (Wii.getButtonClick(MINUS))
        BTserial.print(F("\r\nMinus"));

      if (Wii.getButtonClick(ONE))
        BTserial.print(F("\r\nOne"));
      if (Wii.getButtonClick(TWO))
        BTserial.print(F("\r\nTwo"));

      if (Wii.getButtonClick(A)) {
        printAngle = !printAngle;
        BTserial.print(F("\r\nA"));
      }
      if (Wii.getButtonClick(B)) {
        Wii.setRumbleToggle();
        BTserial.print(F("\r\nB"));
      }
    }
  }
}
