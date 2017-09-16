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
  int sen1 = analogRead(A0);
  int sen2 = analogRead(A1);
 /*Serial.print(sen1); //
 Serial.print(F("  "));
 Serial.println(sen2);*/


 if (abs(sen1-sen2)<75){
 leftservo.write(180);
 rightservo.write(0);
 }
 else if (sen1>sen2){ //tilted to the right; right sensor senses white
 leftservo.write(90);
 rightservo.write(0);
 }
 else if (sen2>sen1){ //tilted to the left; left sensor senses white
  leftservo.write(180);
  rightservo.write(90);
 }
 
  
}
