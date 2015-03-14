#include <PS3BT.h>
#include <usbhub.h>
#include <SoftwareSerial.h>
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

SoftwareSerial BTserial(2,4); // (RX, TX) For sending results to the server

boolean stateSendReady = false;
boolean stateSendNotReady = false;

// Set the motor pins
const int MotorAFORWARD = 3;
const int MotorABACKWARD = 5;
const int MotorBFORWARD = 6;
const int MotorBBACKWARD = 9;

// Set Sensor pin
const int sensorPin = A0;

const int numReadings = 50;

int readings[numReadings];
int index = 0;
int total = 0;
int average = 0;

void setup() {
  pinMode(MotorAFORWARD, OUTPUT);
  pinMode(MotorABACKWARD, OUTPUT);
  pinMode(MotorBFORWARD, OUTPUT);
  pinMode(MotorBBACKWARD, OUTPUT);
  digitalWrite(MotorAFORWARD, LOW);
  digitalWrite(MotorABACKWARD, LOW);
  digitalWrite(MotorBFORWARD, LOW);
  digitalWrite(MotorBBACKWARD, LOW);
  
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
  
  Serial.begin(9600);
  BTserial.begin(9600);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  BTserial.println("{\"NAME\": \"ROBOT02\", \"COMMAND\": \"STATE\", \"VALUE\": \"NOTREADY\"}");
  stateSendNotReady = true;
}

void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    
    // Send ready message in json to the server (and usb serial)
    if(!stateSendReady) {
      BTserial.println("{\"NAME\": \"ROBOT02\", \"COMMAND\": \"STATE\", \"VALUE\": \"READY\"}");
      Serial.println("Remote connected");
      stateSendReady = true;
      stateSendNotReady = false;
    }
    
    // !!! TODO: MAPPING SHOULD OCCUR (but don't know the ranges yet)
    int speed, direction;
    if (PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117)
      speed = map(PS3.getAnalogHat(RightHatY), FROMLOW, TOLOW, -255, 255);
    if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117) {
      direction = map(PS3.getAnalogHat(LeftHatY), FROMLOW, TOLOW, -255, 255);
    move(speed, direction);
    
    // Read the sensor and send to server
    if(readSensor() == 1)
      BTserial.println("{\"NAME\": \"ROBOT02\", \"COMMAND\": \"SENSOR\", \"VALUE\": \"1\"}");
      
  } else {
    if(!stateSendNotReady) {
      BTserial.println("{\"NAME\": \"ROBOT02\", \"COMMAND\": \"STATE\", \"VALUE\": \"NOTREADY\"}");
      Serial.println("Not ready :(");
      stateSendReady = true;
      stateSendNotReady = false;
    }
  }
}

void move(int speed, int direction)
{
  if (speed < 0) { // go backwards
    if (direction < 0) { // go left
      
    } else { // go right
      
    }
  } else { // go forwards
    if (direction < 0) { // go left
      
    } else { // go right
      
    }
  }
  
  
//  int forward, backward;
//  if (Motor == 'L') {
//    forward = MotorAFORWARD;
//    backward = MotorABACKWARD;
//  } else {
//    forward = MotorBFORWARD;
//    backward = MotorBBACKWARD;
//  }
//  if(speed > 0) {
//    digitalWrite(forward, LOW);
//    analogWrite(backward, speed);
//  } else {
//    speed = -speed;
//    digitalWrite(backward, LOW);
//    analogWrite(forward, speed);
//  }
}

int readSensor() {
  // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = digitalRead(sensorPin); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         
  return average;
}
