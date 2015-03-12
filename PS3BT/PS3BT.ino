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

// Set the motor pins
const int MotorAFORWARD = 3;
const int MotorABACKWARD = 5;
const int MotorBFORWARD = 6;
const int MotorBBACKWARD = 11;

char dirL;
char dirR;

void setup() {
  Serial.begin(115200);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}
void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    Serial.print(F("\r\nPS3 controller connected. Ready to rummmbllleeee!"));
    if (PS3.getButtonClick(L1))
      toggleDir('L');
    if (PS3.getButtonClick(R1))
      toggleDir('R');
    if (PS3.getAnalogButton(L2)) {
      Serial.print(F("\r\nL2: "));
      Serial.print(PS3.getAnalogButton(L2));
      move('L', PS3.getAnalogButton(L2));
    }
    if (PS3.getAnalogButton(R2)) {
      Serial.print(F("\r\nR2: "));
      Serial.print(PS3.getAnalogButton(R2));
      move('R', PS3.getAnalogButton(R2));
    }
  }
}

void move(char Motor, int speed)
{
  int forward, backward, dir;
  switch(Motor) {
    case 'L':
    {
      forward = MotorAFORWARD;
      backward = MotorABACKWARD;
      dir = dirL;
      break;
    }
    case 'R':
    {
      forward = MotorBFORWARD;
      backward = MotorBBACKWARD;
      dir = dirR;
      break;
    }
  }
  if(dir == 'B') {
    digitalWrite(forward, LOW);
    analogWrite(backward, speed);
  } else {
    digitalWrite(backward, LOW);
    analogWrite(forward, speed);
  }
}

void toggleDir(char Motor)
{
  switch(Motor) {
    case 'L':
    {
      if(dirL == 'F') {
        dirL = 'B';
      } else {
        dirL = 'F';
      }
      break;
    }
    case 'R':
    {
      if(dirR == 'F') {
        dirR = 'B';
      } else {
        dirR = 'F';
      }
    }
  }
}
