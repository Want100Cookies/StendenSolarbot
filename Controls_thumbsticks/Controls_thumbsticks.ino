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

SoftwareSerial BTserial(7,6); // (RX, TX) For sending results to the server

boolean stateSendReady = false;
boolean stateSendNotReady = false;

// Set the motor pins
const int MotorAFORWARD = 2;
const int MotorABACKWARD = 3; // PWM pin
const int MotorBFORWARD = 4;
const int MotorBBACKWARD = 5; // PWM pin

// Sensor variables
const int sensorPin = A5;

unsigned long lastTime = 0;
unsigned long time = 0;
int delaySeconds = 15000; // 20 second


void setup() {
  pinMode(MotorAFORWARD, OUTPUT);
  pinMode(MotorABACKWARD, OUTPUT);
  pinMode(MotorBFORWARD, OUTPUT);
  pinMode(MotorBBACKWARD, OUTPUT);
  digitalWrite(MotorAFORWARD, LOW);
  digitalWrite(MotorABACKWARD, LOW);
  digitalWrite(MotorBFORWARD, LOW);
  digitalWrite(MotorBBACKWARD, LOW);

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
    if (PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
      speed = map(PS3.getAnalogHat(RightHatY), 0, 255, 255, -255);
    } else {
      speed = 0;
    }
    if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117) {
      direction = map(PS3.getAnalogHat(LeftHatX), 0, 255, -255, 255);
    } else {
      direction = 0;
    }
    move(speed, direction);
    
    if (PS3.getButtonClick(CROSS)) {
      irRead();
    }
    
//    // Read the sensor and send to server
//    if(readSensor() == 1)
//      BTserial.println("{\"NAME\": \"ROBOT02\", \"COMMAND\": \"SENSOR\", \"VALUE\": \"1\"}");
      
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
  int speedLeft = speed;
  int speedRight = speed;
  
  if (speed < 0) { // go backwards
    speedLeft = -speed;
    speedRight = -speed;
    speed = -speed;

    direction = map(direction, 0, 255, 0, speed);

    if (direction < 0) { // go left
      direction = -direction;
      speedLeft = speed - direction;
      
    } else { // go right
      speedRight = speed - direction;
      
    }
    
    analogWrite(MotorABACKWARD, speedLeft);
    analogWrite(MotorBBACKWARD, speedRight);
    digitalWrite(MotorAFORWARD, LOW);
    digitalWrite(MotorBFORWARD, LOW);
  } else { // go forwards
    direction = map(direction, 0, 255, 0, speed);
    
    if (direction < 0) { // go left
      direction = -direction;
      speedLeft = speed - direction;
      
    } else { // go right
      speedRight = speed - direction;
      
    }
    
    speedLeft = map(speedLeft, 0, 255, 255, 0);
    speedRight = map(speedRight, 0, 255, 255, 0);
    
    analogWrite(MotorABACKWARD, speedLeft);
    analogWrite(MotorBBACKWARD, speedRight);
    digitalWrite(MotorAFORWARD, HIGH);
    digitalWrite(MotorBFORWARD, HIGH);
    
  }
}

void irRead()
{
  time = millis();
  if(time > (lastTime + delaySeconds)) {
    int reading = digitalRead(sensorPin);
    if (reading == 0) { // Point scored
      BTserial.println("{\"NAME\": \"ROBOT02\", \"COMMAND\": \"POINT\", \"VALUE\": \"1\"}");
      lastTime = time;
    } else {
      BTserial.println("{\"NAME\": \"ROBOT02\", \"COMMAND\": \"POINT\", \"VALUE\": \"0\"}");
    }
  } else {
    BTserial.println("{\"NAME\": \"ROBOT02\", \"COMMAND\": \"POINT\", \"VALUE\": \"0\"}");
  }
}
