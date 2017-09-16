#include <Servo.h>
int rightTurn;
int leftTurn;

int inLeft;
int inRight;
int outLeft; 
int outRight;
int stepCounter;

Servo leftservo;
Servo rightservo;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  leftservo.attach(9);
  rightservo.attach(10);

  rightTurn = 0;
  leftTurn = 0;
  stepCounter = 1;
  
}

// the loop routine runs over and over again forever:
void loop() {
  
  // read the input on analog pin 0:
  
  //light sensors
  //(>950 : black line ; <900 : white space)
  readSensor();
  
  // --------------
  // LINE FOLLOWING start
  // --------------
  while (outLeft < 900 || outRight < 900){ //while both outer sensors see white
    readSensor();
    
    if (abs(inLeft-inRight)<70){ //if inner are similar
      forward();
      
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

  stop();
 //-------------
 //end line follow section
 //-------------

 
//--------------
//steps when intersection is encountered section, start
//--------------
  switch(stepCounter){
    case 1://for first 3 detected intersections, turn right
    case 2:
    case 3:
        right();
        delay(500);
        while(outLeft > 900 || outRight > 900) { //while out sees black, in sees white
          right();
        }
        while(inLeft < 900 && inRight < 900) { //while out sees black, in sees white
          right();
        }
        stepCounter++;
        break;
    case 4: //go straight after 3 right turns
        forward();
        delay(500);
        stepCounter++;
        break;
    case 5://turn left at next 3 intersections
    case 6:
    case 7:
        left();
        delay(500);
        while(outLeft > 900 || outRight > 900) { //while out sees black, in sees white
          left();
        }
        while(inLeft < 900 && inRight < 900) { //while out sees black, in sees white
          left();
        }
        stepCounter++;
        break;
     case 8://go straight after 3 left turns
        forward();
        delay(500);
        stepCounter=1;
        break;
     default:
        break;
  }
}

void readSensor(){
  inLeft = analogRead(A0);
  inRight = analogRead(A1);
  outLeft = analogRead(A2);
  outRight = analogRead(A3);
}
void forward() {
  leftservo.write(180);
  rightservo.write(0);
  readSensor();
}

void right() {
  leftservo.write(180);
  rightservo.write(95);
  readSensor();
}

void left() {
  leftservo.write(85);
  rightservo.write(0);
  readSensor();
  
}

void stop() {
  leftservo.write(90);
  rightservo.write(90);
}
