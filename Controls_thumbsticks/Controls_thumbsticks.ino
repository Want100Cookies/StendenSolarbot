#include <PS3BT.h>
#include <usbhub.h>
#include <SoftwareSerial.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#include "musical_notes.h"
int speakerPin = A4;

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
  
  pinMode(speakerPin, OUTPUT);

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
    
    if (PS3.getButtonClick(LEFT))
      r2D2();
      
    if (PS3.getButtonClick(UP))
      laugh();
      
    if (PS3.getButtonClick(DOWN))
      squeak();
      
    if (PS3.getButtonClick(RIGHT))
      waka();

      
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

// # # # # # # # # # # # # # # # # # # # # # # # # # # #
//               CODE FOR THE TONES
// # # # # # # # # # # # # # # # # # # # # # # # # # # #

void beep (int speakerPin, float noteFrequency, long noteDuration)
{    
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000/(microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x=0;x<loopTime;x++)   
          {   
              digitalWrite(speakerPin,HIGH); 
              delayMicroseconds(microsecondsPerWave); 
              digitalWrite(speakerPin,LOW); 
              delayMicroseconds(microsecondsPerWave); 
          } 
}       

void r2D2(){
          beep(speakerPin, note_A7,100); //A 
          beep(speakerPin, note_G7,100); //G 
          beep(speakerPin, note_E7,100); //E 
          beep(speakerPin, note_C7,100); //C
          beep(speakerPin, note_D7,100); //D 
          beep(speakerPin, note_B7,100); //B 
          beep(speakerPin, note_F7,100); //F 
          beep(speakerPin, note_C8,100); //C 
          beep(speakerPin, note_A7,100); //A 
          beep(speakerPin, note_G7,100); //G 
          beep(speakerPin, note_E7,100); //E 
          beep(speakerPin, note_C7,100); //C
          beep(speakerPin, note_D7,100); //D 
          beep(speakerPin, note_B7,100); //B 
          beep(speakerPin, note_F7,100); //F 
          beep(speakerPin, note_C8,100); //C 
}

void laugh() {
  for (int i=1000; i<2000; i=i*1.10) {
    beep(speakerPin,i,10);
  }
  delay(50);
  for (int i=1000; i>500; i=i*.90) {
    beep(speakerPin,i,10);
  }
  delay(50);
  for (int i=1000; i<2000; i=i*1.10) {
    beep(speakerPin,i,10);
  }
  delay(50);
  for (int i=1000; i>500; i=i*.90) {
    beep(speakerPin,i,10);
  }
  delay(50);
    for (int i=1000; i<2000; i=i*1.10) {
    beep(speakerPin,i,10);
  }
  delay(50);
}

void squeak() {
  for (int i=100; i<5000; i=i*1.45) {
    beep(speakerPin,i,60);
  }
  delay(10);
  for (int i=100; i<6000; i=i*1.5) {
    beep(speakerPin,i,20);
  }
}

void waka() {
  for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
    for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
    for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
    for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
}

