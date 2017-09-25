#include <Servo.h>

Servo leftservo;
Servo rightservo;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  
  leftservo.attach(9);
  rightservo.attach(10);
  
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int inLeft = analogRead(A0);
  int inRight = analogRead(A1);
 /*Serial.print(sen1); //
 Serial.print(F("  "));
 Serial.println(sen2);*/

 //if (digitalRead(3) = 1) {
  //Serial.println(analogRead(A5));
  //delay(500);
  Serial.print("line OR value ");
  Serial.print(inLeft);
  Serial.print("          ");
  Serial.print("line OL value ");
  Serial.println(inRight);

  if (analogRead(A5) > 230) {
  leftservo.write(90);
  rightservo.write(90);
 }
 else { 
    //readSensor();
     inLeft = analogRead(A0);
     inRight = analogRead(A1);
    if (abs(inLeft-inRight)<70){ //if inner are similar
      leftservo.write(180);
      rightservo.write(0);
      
    }
    else if (inLeft>inRight){ //if tilted left, correct
      leftservo.write(90);
      rightservo.write(0);
      
    }
    else if (inRight>inLeft){ //if tilted right
      leftservo.write(180);
       rightservo.write(90);
     
    } 

  inLeft = analogRead(A0);
  inRight = analogRead(A1);
  }

 }
