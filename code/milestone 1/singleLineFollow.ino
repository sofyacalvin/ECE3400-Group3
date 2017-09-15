#include <Servo.h>


Servo leftservo;
Servo rightservo;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(9,OUTPUT);
  pinMode(11,OUTPUT);
  
  leftservo.attach(9);
  rightservo.attach(11);

  
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sen1 = analogRead(A0);
  int sen2 = analogRead(A1);
 Serial.print(sen1); //
 Serial.print(F("  "));
 Serial.println(sen2);

 /*for (int i = 0 ; i< 100 ; i++){
   myservo1.write(100);
  myservo2.write(80);
 }*/
 //myservo1.write(100);
 //myservo2.write(80);

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
