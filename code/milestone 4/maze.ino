#include <StackArray.h>
#include <Servo.h>

#define ROWS 4
#define COLS 5

#define LeftWall 3
#define RightWall 4
#define OutLineLeft 2
#define OutLineRight 7

typedef struct{
  int x; int y;
} Square;

//light sensors (analog)
  //(>950 : black line ; <900 : white space)
int inLeft;
int inRight;

//digital line sensors
  //0 = black; 1 = white
int outLeft;
int outRight;

//  //analog wall sensor ->
  //wall: > 250; no wall: < 250
int frontwall;

//digital wall sensors ->
  //0 = wall; 1 = no wall
int wallL;
int wallR;

int goback;

char orient;



Servo leftservo;
Servo rightservo;
//Maze maze;

// HWALL = {{1,1,1,1,1},
//          {0,0,0,0,0},
//          {0,0,0,0,0},
//          {0,0,0,0,0},
//          {1,1,1,1,1}}
//
// VWALL = {{1,0,0,0,0,1},
//          {1,0,0,0,0,1},
//          {1,0,0,0,0,1},
//          {1,0,0,0,0,1}}

Square visited[20];
Square start;

StackArray <Square> frontier;
StackArray <Square> path;
Square current;
int visitedSize;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  leftservo.attach(5);
  rightservo.attach(6);
  pinMode(OutLineLeft, INPUT);
  pinMode(LeftWall, INPUT);
  pinMode(RightWall, INPUT);
  pinMode(OutLineRight, INPUT);
  start.x = 0;
  start.y = -1;
  frontier.push(start);
  orient = 1;
}


// the loop routine runs over and over again forever:
void loop() {
  dfs();
}

/********************************HELPER FUNCTIONS******************************/
void dfs(){
  Square current;
  Square next;
  //current = start;
  char c_coor [] = {0,0};
  char n_coor [] = {0,0};
  //followline();

  while(!frontier.isEmpty()){
  //if we decide to start at (0,0) add code to check if we are at an intersection
    if(goback == 0){
      current = frontier.pop();
      Serial.print("CURRENT:   ");
      Serial.print(current.x);
      Serial.println(current.y);
      c_coor[0] = current.x;
      c_coor[1] = current.y;

      if (!isMember(current, visited, visitedSize)){
        visited[visitedSize] = current;
        visitedSize ++;
      }
      int length = frontier.count();
      if(wallL == 1){ //no wall on left
        Serial.println("GOES IN HERE!");
        if(orient == 3){
          next.x = current.x + 1;
          next.y = current.y;
        }
        else if(orient == 2){
          next.x = current.x;
          next.y = current.y + 1;
        }
        else if(orient == 1){
          next.x = current.x - 1;
          next.y = current.y;
        }
        else if(orient == 0){
          next.x = current.x;
          next.y = current.y - 1;
        }
        if(!isMember(next, visited, visitedSize)){
          frontier.push(next);
        }
      }
      if(wallR == 1){ //no wall on right
        if(orient == 3){
          next.x = current.x - 1;
          next.y = current.y;
        }
        else if(orient == 2){
          next.x = current.x;
          next.y = current.y - 1;
        }
        else if(orient == 1){
          next.x = current.x + 1;
          next.y = current.y;
        }
        else if(orient == 0){
          next.x = current.x;
          next.y = current.y + 1;
        }
        if(!isMember(next, visited, visitedSize)){
          frontier.push(next);
        }
      }
      if(frontwall < 200){ //no wall in front
        if(orient == 3){
          next.x = current.x;
          next.y = current.y - 1;
        }
        else if(orient == 2){
          next.x = current.x + 1;
          next.y = current.y;
        }
        else if(orient == 1){
          next.x = current.x;
          next.y = current.y + 1;
        }
        else if(orient == 0){
          next.x = current.x - 1;
          next.y = current.y;
        }
        if(!isMember(next, visited, visitedSize)){
          frontier.push(next);
        }
      }

      n_coor[0] = frontier.peek().x;
      n_coor[1] = frontier.peek().y;
      Serial.println(isMember(next, visited, visitedSize));
      Serial.print("NEXT:   ");
      Serial.print(frontier.peek().x);
      Serial.println(frontier.peek().y);
      if(frontier.count()>length){
        goback = 0;
        orient = reorient(c_coor, n_coor, orient);
        Serial.print("Orient:   ");
        Serial.println((int)orient);
        Serial.print("new frontier element");
        Serial.print(frontier.peek().x);
        Serial.println(frontier.peek().y);
        Serial.println("Stuck here");
        path.push(current);
      }
      else{
        goback = 1;
      }


//      //delay(1000);

    }
    else{
      next = frontier.peek();
      if (!isMember(next, visited, visitedSize)){
        if(frontier.count() > 0){
          Square tmp = path.pop();
          if(squareCompare(current, tmp)){
            tmp = path.pop();
          }
          frontier.push(tmp);
          n_coor[0] = frontier.peek().x;
          n_coor[1] = frontier.peek().y;
          Serial.print("NEXT:   ");
          Serial.print(frontier.peek().x);
          Serial.println(frontier.peek().y);
          orient = reorient(c_coor, n_coor, orient);
          Serial.print("Orient:   ");
          Serial.println((int)orient);
          goback = 0;
          Serial.println("Stuck here3");
        }
      }
      else{
        frontier.pop();
      }
    }
    followline();
  }

}



bool isMember (Square a, Square visit[], int sizeV){
  for(int i = 0; i < sizeV; i ++){
    if (squareCompare(a,visit[i])){
      return true;
    }
  }
  return false;
}

bool squareCompare (Square a, Square b){
  if(a.x == b.x){
    if(a.y == b.y){
      return true;
    }
  }
  return false;
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
//  else{
//    next_o = curr_o;
//  }

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
//  else{
//    stp();
//  }

  curr_o = next_o;
  Serial.println((int) curr_o);

  stp();
  return curr_o;
}

void readSensor(){
  inLeft = analogRead(A0);
  inRight = analogRead(A1);
  frontwall = analogRead(A2);
  outLeft = digitalRead(OutLineLeft);
  outRight = digitalRead(OutLineRight);
  wallL = digitalRead(LeftWall);
  wallR = digitalRead(RightWall);
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
  readSensor();
}

void left() {
  leftservo.write(85);
  rightservo.write(0);
  readSensor();
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

void followline(){
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
  stp();
}
