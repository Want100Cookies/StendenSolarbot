const int LeftMotorForward = 2;
const int LeftMotorBackward = 3;
const int RightMotorForward = 4;
const int RightMotorBackward = 5;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(11, 10);
  
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(RightMotorForward, OUTPUT);
  Serial.println("Connection succesful!");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    int data = Serial.read();
    switch(data)
    {
      case 'w' : moveForward(); break;
      case 's' : moveBackward(); break;
      case 'a' : moveLeft(); break;
      case 'd' : moveRight(); break;
      case 'e' : moveStop(); break;
      default  : break;  
    }
  }
  delay(50);
}

//Deze functie laat de robot naar voor rijden
void moveForward()
{
  Serial.println("moveForward");
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(RightMotorForward, HIGH);
}

//Deze functie laat de robot naar achter rijden
void moveBackward()
{
  Serial.println("moveBackward");
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, HIGH);
}

//Deze functie laat de robot naar links gaan
void moveLeft()
{
  Serial.println("moveLeft");
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(RightMotorForward, HIGH);
}

//Deze functie laat de robot naar rechts gaan
void moveRight()
{
  Serial.println("moveRight");
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, HIGH);
}

//Deze functie laat de robot stoppen
void moveStop()
{
  Serial.println("moveStop");
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}
