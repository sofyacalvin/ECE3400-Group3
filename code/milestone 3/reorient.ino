#include <Servo.h>

int inLeft;
int inRight;
int outLeft; 
int outRight;
int stepCounter;
int frontwall;
int wallL;
int wallR;


Servo leftservo;
Servo rightservo;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  leftservo.attach(5);
  rightservo.attach(6);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(7, INPUT);
  //rightTurn = 0;
  //leftTurn = 0;
  stepCounter = 1;
}

// the loop routine runs over and over again forever:
void loop() {
  //digital line sensors: 0 = black; 1 = white
  // read the input on analog pin 0:
  
  //light sensors
  //(>950 : black line ; <900 : white space)
  readSensor();
  


  //test path following and reorientation with coordinates
  lineFollow();
  char current[] = {1, 1};
  char curr_o = 1;
  char next[] = {1, 2};
  curr_o = reorient(current, next, curr_o);
  lineFollow();
  current[0] = next[0];
  current[1] = next[1];
  next[0] = 2;
  next[1] = 2;
  curr_o = reorient(current, next, curr_o);
  lineFollow();
  current[0] = next[0];
  current[1] = next[1];
  next[0] = 2;
  next[1] = 3;
  curr_o = reorient(current, next, curr_o);
  lineFollow();
  current[0] = next[0];
  current[1] = next[1];
  next[0] = 2;
  next[1] = 4;
  curr_o = reorient(current, next, curr_o);
  lineFollow();
  current[0] = next[0];
  current[1] = next[1];
  next[0] = 2;
  next[1] = 3;
  curr_o = reorient(current, next, curr_o);
  lineFollow();
  stp();
  delay(5000);
  
 //-------------
 //end line follow section (reach an intersection)
 //-------------
  //digital wall sensors -> 0 = wall; 1 = no wall
  //analog wall sensor -> wall: > 250; no wall: < 250 

  
  
  /*
  if(wallL == 0 && wallR == 1 && frontwall > 200){
    right();
    delay(800);
  }else if(wallL == 1 && wallR == 0 && frontwall > 200){
    left();
    delay(800);
  }else if(wallL == 0 && wallR == 0 && frontwall < 200){
    forward();
  }else{
    stp();
  }
  */ 
}

  // --------------
  // LINE FOLLOWING
  // --------------
void lineFollow(){
  while (outLeft == 1 || outRight == 1){ //while both outer sensors see white
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
}

char reorient(char current[], char next[], char curr_o) {
  char next_o = 0;
  char diff[] = {0, 0};
  diff[0] = next[0]-current[0];
  diff[1] = next[1]-current[1];
  
  if (diff[0] == -1){ //north
    next_o = 0;
  }
  else if (diff[0] == 1){ // south
    next_o = 2; 
  }
  else if (diff[1] == -1){ // west
    next_o = 3; 
  }
  else if (diff[1] == 1){ // east
    next_o = 1;
  }

  if (next_o - curr_o == 0){
    //straight
    forward();
    delay(200);
  }
  else if (next_o - curr_o == 1 || next_o - curr_o == -3){
    //turn right
    right();
    delay(800);
  }
  else if (abs(next_o - curr_o) == 2){
    //flip
    flip();
    delay(1250);
  }
  else if (next_o - curr_o == -1 || next_o - curr_o == 3){
    //turn left
    left();
    delay(800);
  }
  
  curr_o = next_o;
  Serial.println(curr_o);
  
  stp();
  return curr_o;
}

void readSensor(){
  inLeft = analogRead(A0);
  inRight = analogRead(A1);
  frontwall = analogRead(A2);
  outLeft = digitalRead(2);
  outRight = digitalRead(7);
  wallL = digitalRead(3);
  wallR = digitalRead(4);
}

//turning functions
void forward() {
  leftservo.write(180);
  rightservo.write(0);
  readSensor();
  // delay(time);
}

void right() {
  leftservo.write(180);
  rightservo.write(95);
  readSensor();
  //delay(time);
}

void left() {
  leftservo.write(85);
  rightservo.write(0);
  readSensor();
  //delay(time);
}

void flip() {
  leftservo.write(180);
  rightservo.write(180);
  readSensor();
  //delay(time);
}

void stp() {
  leftservo.write(90);
  rightservo.write(90);
  readSensor();
}

/*
int reorient(current, next, int orientation, int x, int y){
  
  //current - next
  //if x changed:
    //2x - orientation
  //if y changed:
    //y - orientation
  //reorient

  val = abs(nextInd - currentInd);

  if(val == 3 || val == 6)
    left();
  else if(val == 5 || val == 4)
    right();
  else if(val == 0 || val == 1)
    forward();
  else
    flip();

  orientation = nextInd -(currentInd - orientation);

  if(orientation == -5)
    orientation += 1;
  else if (orientation == 5)
    orientation += -1;

  return orientation;
}
*/
