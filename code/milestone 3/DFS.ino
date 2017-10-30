#include <StackList.h>
#include <Servo.h>

StackList<unsigned char> frontier;
unsigned char visited[20];  
unsigned char inFrontier[20];
unsigned char current;

unsigned x_pos = 0;
unsigned y_pos = 0;
int index = 0;
int orientation = 0;
int reorient = 0;//left is -1, straight is 0, right is -1

//wall sensors
int frontWall;
int leftWall;
int rightWall;

//line sensors
int inLeft;
int inRight;
int outLeft;
int outRight;
int value = 200;

Servo leftservo;
Servo rightservo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  leftservo.attach(5);
  rightservo.attach(6);
  frontier.push(x_pos << 2 | y_pos);

}
void loop() {
  // put your main code here, to run repeatedly:
  
  
  
  while(!frontier.isEmpty()) {
    current = frontier.pop();
    //move to current
    
    //line following code
    while (outLeft == 1|| outRight == 1){ //while both outer sensors see white
      readSensor();
    
    if (abs(inLeft-inRight)<70) //if inner are similar
       forward();
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
    //when at next square
    index = (y_pos * 5) + x_pos;
    if (!visited[index]) { //current is unvisited
      addFrontier();
      visited[index] = 1;
    }
  }
}






/*

// starting from top left (refer to map)
void addFrontier(){
  unsigned char next;
  if (orientation == 0) { //facing east
    if (leftWall == 0) { //north is path
      next = x_pos << 2 | y_pos - 1;
      frontier.push(next);
      reorient = -1;
    }
    if (rightWall == 0){//south is path
      next = x_pos << 2 | y_pos + 1;
      frontier.push(next);
      reorient = 1;
    }
    if (frontWall == value){//east is path--most prioritized
      //assuming receieved data ignores "state data"
      next = x_pos + 1 << 2 | y_pos;
      frontier.push(next);
      reorient = 0;
    }
  }
  
  
  else if (orientation == 1) { // facing south
    if(rightWall == 0) { // west path
      next = x_pos - 1 << 2 | y_pos;
      frontier.push(next);
      reorient = 1;
    }
    //south path
    if (frontWall > value){  
      next = x_pos << 2 | y_pos + 1;
      frontier.push(next);
      reorient = 0;
    }
    if (leftWall == 0) { //east
      next = x_pos + 1 << 2 | y_pos;
      frontier.push(next);
      reorient = -1;
    }
  }
  
  else if (orientation == 2) { // facing west
    if(rightWall == 0) { // north path
      next = x_pos << 2 | y_pos - 1;
      frontier.push(next);
      reorient = 1;
    }
    if (frontWall > value) { //west path
      next = x_pos - 1 << 2 | y_pos;
      frontier.push(next);
      reorient = 0;
    }
    if (leftWall == 0) { //south
      next = x_pos << 2 | y_pos + 1;
      frontier.push(next);
      reorient = -1;
    }
  }
  else if (orientation == 3) { // facing north
    if(leftWall == 0) { // west path
      next = x_pos - 1 << 2 | y_pos;
      frontier.push(next);
      reorient = -1;
    }
    if (frontWall > value) { //north path
      next = x_pos << 2 | y_pos - 1;
      frontier.push(next);
      reorient = 0;
    }
    if (rightWall == 0) { //east
      next = x_pos + 1 << 2 | y_pos;
      frontier.push(next);
      reorient = 1;
    }
  }
  if (reorient == 1){
    right();
        
  }
  else if (reorient == -1){
    left();
        
  }
}
*/


/*
// starting from bottom left (refer to map)
void addFrontier(){
  unsigned char next;
  if (orientation == 0) { //facing east
    if (1){//south is path
      next = x_pos << 2 | y_pos + 1;
      frontier.push(next);
    }
    if (1) { //north is path
      next = x_pos << 2 | y_pos - 1;
      frontier.push(next);
    }
    if (1){//east is path--most prioritized
      //assuming receieved data ignores "state data"
      next = x_pos + 1 << 2 | y_pos;
      frontier.push(next);
    }
  }
  
  else if (orientation == 1) { // facing south
    if (1) { //south path
      next = x_pos << 2 | y_pos + 1;
      frontier.push(next);
    }
    if(1) { // west path
      next = x_pos - 1 << 2 | y_pos;
      frontier.push(next);
    }
    if (1) { //east
      next = x_pos + 1 << 2 | y_pos;
      frontier.push(next);
    }
  }
  
  else if (orientation == 2) { // facing west
    if (1) { //south path
      next = x_pos << 2 | y_pos + 1;
      frontier.push(next);
    }
    if (1) { //west path
      next = x_pos - 1 << 2 | y_pos;
      frontier.push(next);
    }
    if(1) { // north path
      next = x_pos << 2 | y_pos - 1;
      frontier.push(next);
    }
  }
  else if (orientation == 3) { // facing north
    if(1) { // west path
      next = x_pos - 1 << 2 | y_pos;
      frontier.push(next);
    }
    if (1) { //north path
      next = x_pos << 2 | y_pos - 1;
      frontier.push(next);
    }
    if (1) { //east path
      next = x_pos + 1 << 2 | y_pos;
      frontier.push(next);
    }
  }
}
*/

void readSensor(){
  inLeft = analogRead(A0);
  inRight = analogRead(A1);
  outLeft = digitalRead(2);
  outRight = digitalRead(7);

  frontWall = analogRead(A2);
  leftWall = !digitalRead(3);
  rightWall = !digitalRead(4);
}

//turning functions
void forward() {
  leftservo.write(180);
  rightservo.write(0);
  readSensor();
}

void right() {
  leftservo.write(180);
  rightservo.write(95);
  delay(500);
  while(outLeft == 0 || outRight == 0) { //while out sees black, in sees white
   readSensor();
  }
  while(inLeft < 900 && inRight < 900) { //while out sees black, in sees white
   readSensor();
        }
}

void left() {
  leftservo.write(85);
  rightservo.write(0);
      delay(500);
        while(outLeft == 0 || outRight == 0) { //while out sees black, in sees white
          readSensor();
        }
        while(inLeft < 900 && inRight < 900) { //while out sees black, in sees white
          readSensor();
        }

  
}
