#include <Servo.h>

Servo leftservo;
Servo rightservo;

int inLeft;
int inRight;
int outLeft; 
int outRight;

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
  readSensor();

  while (outLeft < 900 || outRight < 900){ //while both outer sensors see white
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

  readSensor();
  }

  if (analogRead(A5) > 200) { //If at an intersection and a wall is detected in front
    left();
    delay(500);
    while(outLeft > 900 || outRight > 900) { //while out sees black, in sees white
      left();
    }
    while(inLeft < 900 && inRight < 900) { //while out sees black, in sees white
      left();
    }
  }
  else{ // if there is no wall in front at the intersection, drive across
    forward();
    delay(500);
  }
}
void forward() {
  leftservo.write(180);
  rightservo.write(0);
  readSensor();
}

void left() {
  leftservo.write(85);
  rightservo.write(0);
  readSensor();
}
void readSensor(){
  inLeft = analogRead(A0);
  inRight = analogRead(A1);
  outLeft = analogRead(A2);
  outRight = analogRead(A3);
}
 
