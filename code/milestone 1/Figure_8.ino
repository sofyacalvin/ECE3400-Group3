#include <Servo.h>
int rightTurn = 0;
int leftTurn = 0;

int inLeft;
int inRight;
int outLeft; 
int outRight;

Servo leftservo;
Servo rightservo;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  leftservo.attach(9);
  rightservo.attach(10);
}

// the loop routine runs over and over again forever:
void loop() {
  stop();
  // read the input on analog pin 0:
  
  //light sensors
  readSensor();
  
  Serial.print(outLeft); //
  Serial.print(F("  "));
  Serial.println(outRight);


  while (outLeft < 900 && outRight < 900){ //while both outer sensors see white
    readSensor();
    Serial.println("in first while");
    Serial.print(outLeft); //
    Serial.print(F("  "));
    Serial.println(outRight);

    if (abs(inLeft-inRight)<75){ //if inner are similar
      forward();
     }
   else if (inLeft>inRight){ //tilted left
      right();
     }
   else if (inRight>inLeft){ //tilted right
      left();
     }   
     readSensor();
  }
  Serial.println("out first while");
  
  if (rightTurn < 3){ //enters this when both outer sensors see black
    while(outLeft > 930 || outRight > 930 || inLeft < 870 || inRight < 870) { //while out sees black, in sees white
      right();
    }
    rightTurn++;
  }
  else if (rightTurn == 3 && leftTurn == 0){ //middle s
    while(outLeft > 930 && outRight > 930) { //while both out see black
      forward();
    }
    leftTurn++;
  }
  else if (leftTurn < 4){
    while(outLeft > 930 || outRight > 930 || inLeft < 870 || inRight < 870) {
      left();
    }
    leftTurn++;
  }
  else if (leftTurn == 4){
    while(outLeft > 930 && outRight > 930) {
      forward();
    }
    rightTurn = 0;
    leftTurn = 0;
  }
  
}

void readSensor(){
  inLeft = analogRead(A0);
  inRight = analogRead(A1);
  outLeft = analogRead(A2);
  outRight = analogRead(A3);
}
void forward() {
  leftservo.write(100);
  rightservo.write(80);
  readSensor();
}

void right() {
  leftservo.write(100);
  rightservo.write(70);
  readSensor();
}

void left() {
  leftservo.write(110);
  rightservo.write(80);
  readSensor();
  
}

void stop() {
  leftservo.write(90);
  rightservo.write(90);
}
