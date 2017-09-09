#include <Servo.h>


Servo myservo1;
Servo myservo2;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //pinMode(3,OUTPUT);
  
  myservo1.attach(9);
  myservo2.attach(10);
  myservo1.write(90);
  myservo2.write(90);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sen1 = analogRead(A0);
  int sen2 = analogRead(A1);
 Serial.print(sen1); //
 Serial.print(F("  "));
 Serial.println(sen2);

 if (abs(sen1-sen2)<75){
 myservo1.write(100);
 myservo2.write(80);
 }
 else if (sen1>sen2){
 myservo1.write(100);
 myservo2.write(70);
 }
 else if (sen2>sen1){
  myservo1.write(110);
 myservo2.write(80);
 }
 
  
}
