const int sensorPin = A0;

unsigned long lastTime = 0;
unsigned long time = 0;
int delaySeconds = 5000; // 1 second

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  delay(100);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) {
   if(Serial.read() == 'r') {
    irRead();
   }
  } 
  delay(10);
}

void irRead()
{
  time = millis();
  if(time > (lastTime + delaySeconds)) {
    Serial.println(digitalRead(sensorPin));
  } else {
    Serial.println("Sorry, delay not yet been met !! :P");
  }
  
}
