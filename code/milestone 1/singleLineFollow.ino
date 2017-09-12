#include <Servo.h>


Servo myservo1;
Servo myservo2;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //pinMode(3,OUTPUT);
  
  myservo1.attach(9);
  myservo2.attach(10);
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
 //myservo1.write(90);
 //myservo2.write(90);

 if (abs(sen1-sen2)<70){
 myservo1.write(0);
 myservo2.write(180);
 }
 else if (sen1>sen2){
 myservo1.write(0);
 myservo2.write(0);
 }
 else if (sen2>sen1){
  myservo1.write(180);
  myservo2.write(180);
 }
 
  
}
