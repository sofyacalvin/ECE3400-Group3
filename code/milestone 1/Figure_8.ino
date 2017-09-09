#include <Servo.h>

  int rightTurn = 0;
  int leftTurn = 0;

Servo leftservo;
Servo rightservo;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //pinMode(3,OUTPUT);
  
  leftservo.attach(9);
  rightservo.attach(10);

}

// the loop routine runs over and over again forever:
void loop() {
  stop();
  // read the input on analog pin 0:
  
  //light sensors
  int inLeft = analogRead(A0);
  int inRight = analogRead(A1);
  int outLeft = analogRead(A2);
  int outRight = analogRead(A3);
  
 Serial.print(inLeft); //
 Serial.print(F("  "));
 Serial.println(inRight);

while (outLeft < 870 && outRight < 870){
  if (abs(inLeft-inRight)<75){
  forward();
 }
 else if (inLeft>inRight){
 right();
 }
 else if (inRight>inLeft){
  left();
 }   
}
if (rightTurn < 3){
  while(outLeft > 930 || outRight > 930 || inLeft < 870 || inRight < 870)
  right();
  rightTurn++;
}
else if (rightTurn == 3 && leftTurn == 0){
  while(outLeft > 930 && outRight > 930)
  forward();
  leftTurn++;
}
else if (leftTurn < 4){
  while(outLeft > 930 || outRight > 930 || inLeft < 870 || inRight < 870)
  left();
  leftTurn++;
}
else if (leftTurn == 4){
  while(outLeft > 930 && outRight > 930)
  forward();
  rightTurn = 0;
  leftTurn = 0;
}
}

void forward() {
  leftservo.write(100);
  rightservo.write(80);
}

void right() {
  leftservo.write(100);
  rightservo.write(70);
}

void left() {
  leftservo.write(110);
  rightservo.write(80);
  
}

void stop() {
  leftservo.write(90);
  rightservo.write(90);
}
