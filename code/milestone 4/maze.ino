#include <QueueList.h>
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

typedef struct{
  int n; int w; int s; int e;
} Square_wall;

typedef struct{
  int dist;
  Square prev;
} Square_dist;

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

Square_wall maze[ROWS][COLS];


Square start;

StackArray <Square> frontier;
StackArray <Square> path;

// Square current;
// int visitedSize;

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
  initialize_walls();
}


// the loop routine runs over and over again forever:
void loop() {
  dfs();

}

/********************************HELPER FUNCTIONS******************************/
void dfs(){
  int visitedSize = 0;
  //StackArray <Square> path;
  StackArray <Square> backto;
  Square visited[20];
  Square current;
  Square next;
  //current = start;
  char c_coor [] = {0,0};
  char n_coor [] = {0,0};
  //followline();
  int newsquare;

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
        newsquare = 1;
      }
      else{
        newsquare = 0;
      }
      Serial.print("VISITED SIZE: ");
      Serial.println(visitedSize);
      int length = frontier.count();
      if(wallL == 1){ //no wall on left
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
      else{
        if(orient == 3){
          maze[current.x][current.y].s = 1;
        }
        else if(orient == 2){
          maze[current.x][current.y].e = 1;
        }
        else if(orient == 1){
          maze[current.x][current.y].n = 1;
        }
        else if(orient == 0){
          maze[current.x][current.y].w = 1;
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
      else{
        if(orient == 3){
          maze[current.x][current.y].n = 1;
        }
        else if(orient == 2){
          maze[current.x][current.y].w = 1;
        }
        else if(orient == 1){
          maze[current.x][current.y].s = 1;
        }
        else if(orient == 0){
          maze[current.x][current.y].e = 1;
        }
      }
      if(frontwall < 180){ //no wall in front
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
      else{
        if(orient == 3){
          maze[current.x][current.y].w = 1;
        }
        else if(orient == 2){
          maze[current.x][current.y].s = 1;
        }
        else if(orient == 1){
          maze[current.x][current.y].e = 1;
        }
        else if(orient == 0){
          maze[current.x][current.y].n = 1;
        }
      }

      n_coor[0] = frontier.peek().x;
      n_coor[1] = frontier.peek().y;
      Serial.print("NEXT:   ");
      Serial.print(frontier.peek().x);
      Serial.println(frontier.peek().y);
      if(frontier.count()>length){
        if((frontier.count() - length) > 1){
//          for (int i = 0; i < (frontier.count() - length) - 1; i++){
//            backto.push(current);
//          }
            backto.push(current);


          //          Serial.print("add to backto:   ");
          //          Serial.print(current.x);
          //          Serial.println(current.y);
        }
        goback = 0;
        orient = reorient(c_coor, n_coor, orient);
        //path.push(current);
      }
      // else if(newsquare = 1){
      //
      // }
      else{
        goback = 1;
      }


    }
    else{
      Square back;
      Serial.print("newSquare:  ");
      Serial.println(newsquare);

      //      if(newsquare == 1){
      //        Serial.println("start helper here");
      //        back = backto.pop();
      //        Serial.print("BACK:   ");
      //        Serial.print(back.x);
      //        Serial.println(back.y);
      //        shortest_path(current,back,maze);
      //        Serial.println("finish helper");
      //      }
      //      Serial.print("PATH:   ");
      //      Serial.print(path.peek().x);
      //      Serial.println(path.peek().y);
     if(frontier.count() > 0){
      next = frontier.peek();
      Serial.println("stuck here?");
      if (!isMember(next, visited, visitedSize)){
        Serial.println("stuck here");
        if(newsquare == 1){
          Serial.println("start helper here");
          back = backto.pop();
          Serial.print("BACK:   ");
          Serial.print(back.x);
          Serial.println(back.y);
          shortest_path(current,back,maze);
          Serial.println("finish helper");
        }

        Square tmp = path.pop();
        Serial.println("stuck in loop");
        while (!squareCompare(tmp,back)){
          //Square tmp = path.pop();
          // if(squareCompare(current, tmp)){
          //   tmp = path.pop();
          // }
          //frontier.push(tmp);
          n_coor[0] = tmp.x;
          n_coor[1] = tmp.y;
          Serial.print("NEXT:   ");
          Serial.print(tmp.x);
          Serial.println(tmp.y);
          orient = reorient(c_coor, n_coor, orient);
          followline();
          c_coor[0] = n_coor[0];
          c_coor[1] = n_coor[1];
          tmp = path.pop();
        }
        Serial.println("fail here");
        n_coor[0] = tmp.x;
        n_coor[1] = tmp.y;
        orient = reorient(c_coor, n_coor, orient);
        frontier.push(tmp);
        goback = 0;
      }
      else{
        frontier.pop();
        //backto.pop();
        Serial.print("Frontier size:  ");
        Serial.print(frontier.count());
        Serial.print("      ");
        Serial.print("backto size:  ");
        Serial.println(backto.count());
      }
      }
      else{
        Serial.println("EMPTY FRONTIER");
        }
    }
    followline();
  }

}

void shortest_path (Square a, Square b, Square_wall maze[ROWS][COLS]){
  QueueList <Square> queue;
  Square blank;
  blank.x = 1287;
  blank.y = 1287;
  Square visited [20];
  int visitedSize = 0;
  Square_dist best_path [ROWS][COLS];
  //initialize best_path array
  for (int i = 0; i < ROWS; i++){
    for (int j = 0; j < COLS; j++){
      best_path[i][j].dist = 21;
      best_path[i][j].prev = blank;
    }
  }
  queue.push(a);
  bool stop = true;
  Square current;
  Square next;
  best_path[a.x][a.y].dist = 0;
  Serial.println("stuck in first while");
  while ((!queue.isEmpty())||stop){
    current = queue.pop();
    if (!isMember(current, visited, visitedSize)){
      visited[visitedSize] = current;
      visitedSize ++;
    }
    if (squareCompare(b, current)){
      stop = false;
    }
    if(maze[current.x][current.y].n == 0){
      int temp = best_path[current.x][current.y].dist + 1;
      next.x = current.x - 1;
      next.y = current.y;
      if (best_path[next.x][next.y].dist > temp){
        best_path[next.x][next.y].dist = temp;
        best_path[next.x][next.y].prev = current;
      }
      if(!isMember(next, visited, visitedSize)){
        queue.push(next);
      }
    }
    if(maze[current.x][current.y].w == 0){
      int temp = best_path[current.x][current.y].dist + 1;
      next.x = current.x;
      next.y = current.y - 1;
      if (best_path[next.x][next.y].dist > temp){
        best_path[next.x][next.y].dist = temp;
        best_path[next.x][next.y].prev = current;
      }
      if(!isMember(next, visited, visitedSize)){
        queue.push(next);
      }
    }
    if(maze[current.x][current.y].s == 0){
      int temp = best_path[current.x][current.y].dist + 1;
      next.x = current.x + 1;
      next.y = current.y;
      if (best_path[next.x][next.y].dist > temp){
        best_path[next.x][next.y].dist = temp;
        best_path[next.x][next.y].prev = current;
      }
      if(!isMember(next, visited, visitedSize)){
        queue.push(next);
      }
    }
    if(maze[current.x][current.y].e == 0){
      int temp = best_path[current.x][current.y].dist + 1;
      next.x = current.x;
      next.y = current.y + 1;
      if (best_path[next.x][next.y].dist > temp){
        best_path[next.x][next.y].dist = temp;
        best_path[next.x][next.y].prev = current;
      }
      if(!isMember(next, visited, visitedSize)){
        queue.push(next);
      }

    }

  }
  Serial.println("stuck in second while");
  //make path back.
  current = b;
  while(!squareCompare(a, current)){
    path.push(current);
    current = best_path[current.x][current.y].prev;
    Serial.print("current:   ");
    Serial.print(path.peek().x);
    Serial.println(path.peek().y);
  }

}

void initialize_walls(){
  for (int i = 0; i < ROWS; i++){
    for (int j = 0; j < COLS; j++){
      maze[i][j].n = 0;
      maze[i][j].s = 0;
      maze[i][j].e = 0;
      maze[i][j].w = 0;
      if (i == 0){
        maze[i][j].n = 1;
      }

      if (i == 3){
        maze[i][j].s = 1;
      }

      if (j == 0){
        maze[i][j].w = 1;
      }

      if (j == 4){
        maze[i][j].e = 1;
      }



    }
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
  //  Serial.println((int) curr_o);

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
