#include <Servo.h>
int rightTurn;
int leftTurn;

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

  rightTurn = 0;
  leftTurn = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  
  // read the input on analog pin 0:
  
  //light sensors
  //(>950 : black line ; <900 : white space)
  readSensor();
  
  Serial.print(outLeft); //
  Serial.print(F("  "));
  Serial.println(outRight);

  Serial.print(inLeft); //
  Serial.print(F("  "));
  Serial.println(inRight);

 
  while (outLeft < 900 && outRight < 900){ //while both outer sensors see white
    readSensor();
    //Serial.println("in first while");
    //Serial.print(outLeft); //
    //Serial.print(F("  "));
    //Serial.println(outRight);
    
    if (abs(inLeft-inRight)<70){ //if inner are similar
      forward();
      
    }
    else if (inLeft>inRight){ //tilted left
       leftservo.write(0);
       rightservo.write(0);
    }
    else if (inRight>inLeft){ //tilted right
      leftservo.write(180);
       rightservo.write(180);
    } 

    readSensor();
  }

  stop();
 
  Serial.println("out first while");
  
  if (rightTurn < 3 && leftTurn == 0){ //enters this when both outer sensors see black
    //while(outLeft > 950 || outRight > 950 || inLeft < 900 || inRight < 900) { //while out sees black, in sees white
     // right();
    //}
    right();
    delay(500);
    while(outLeft > 900 || outRight > 900) { //while out sees black, in sees white
      right();
    }
    while(inLeft < 900 && inRight < 900) { //while out sees black, in sees white
      right();
    }
    rightTurn++;
  }
  else if (rightTurn == 3 && leftTurn == 0){ //middle s
    while(outLeft > 950 && outRight > 950) { //while both out see black
      forward();
    }
    leftTurn++;
  }
  else if (rightTurn == 3 && leftTurn < 4){
    //while(outLeft > 950 || outRight > 950 || inLeft < 900 || inRight < 900) {
    // left();
    //}
    left();
    delay(500);
    while(outLeft > 900 || outRight > 900) { //while out sees black, in sees white
      left();
    }
    while(inLeft < 900 && inRight < 900) { //while out sees black, in sees white
      left();
    }
    leftTurn++;
  }
  else if (rightTurn == 3 && leftTurn == 4){
    while(outLeft > 900 && outRight > 900) {
      delay(500);
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
  leftservo.write(60);
  rightservo.write(120);
  readSensor();
}

void right() {
  leftservo.write(90);
  rightservo.write(120);
  readSensor();
}

void left() {
  leftservo.write(60);
  rightservo.write(90);
  readSensor();
  
}

void stop() {
  leftservo.write(90);
  rightservo.write(90);
}
