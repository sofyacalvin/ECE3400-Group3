#include <QueueList.h>
#include <StackArray.h>
#include <Servo.h>

#define LOG_OUT 1 // use the log output function
#define FFT_N 64 // set to 64 point fft

#include <FFT.h> // include the library

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

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

//analog wall sensor ->
//wall: > 250; no wall: < 250
int frontwall;

//digital wall sensors ->
//0 = wall; 1 = no wall
int wallL;
int wallR;

//signal to use dfs or backtrack using bfs
int goback;

//orientation 
char orient;

//mircophone pin
int sensorPin = A5;

//initialize treasure variable
int treasures = B000;

//initialize Servos
Servo leftservo;
Servo rightservo;

//initialize maze information array to keep track of walls 
Square_wall maze[ROWS][COLS];

//intialize starting node
Square start;

//initialize frontier stack for dfs and path stack for backtracking (using shortest path)
StackArray <Square> frontier;
StackArray <Square> path;

//Radio variables
RF24 radio(9,10);
const uint64_t pipes[2] = { 0x0000000006LL, 0x0000000007LL };


void setup() {
  //initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  radio_init(); //initialize radio
  leftservo.attach(5);
  rightservo.attach(6);
  stp();
  
  //set up digital pins
  pinMode(OutLineLeft, INPUT);
  pinMode(LeftWall, INPUT);
  pinMode(RightWall, INPUT);
  pinMode(OutLineRight, INPUT);
  pinMode(8, INPUT);

  //set the starting node and add to frontier
  start.x = 3;
  start.y = 0;
  frontier.push(start);

  //set the starting orientation
  orient = 1;

  //set up walls in the maze information array
  initialize_walls();
}


// the loop routine runs over and over again forever:
void loop() {
//  while(1){
//    Serial.print("Left:  ");
//    Serial.print(analogRead(A0));
//    Serial.print("     Right:  ");
//    Serial.println(analogRead(A1));
//    delay(1000);
//  }

  //660 hz signal detection variable
  bool start_signal = false;
  //counter to check that the 660 hz signal is dectected for a while
  int i = 0;

  //FFT constants that mess with Servos if they dont get reset
  int reset1 = ADCSRA;
  int reset2 = ADMUX;
  int reset3 = DIDR0;
  int reset4 = TIMSK0;
  
  //FFT setup stuff from example code in lab
  //TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe7; // set the adc to free running mode
  ADMUX = 0x45; // use adc5
  DIDR0 = 0x06; // turn off the digital input for adc5
  
  
  Serial.println("looking for 660Hz");
  while ((!start_signal) || !(i == 10)){    
    Serial.println("still looking");
    delay(10);
    start_signal = check_mic();
    if (start_signal) {
      i++;
    }
    else{
       if (i == 0) {
        i = 0;
       }
       else {
        i--;
       }
    }

    //push button for manual start (overrides the start_signal)
    if(digitalRead(8)){
      start_signal = true;
      i = 10;
    }
  }

  //reset FFT stuff that mess with Servos
  //TIMSK0 = reset4; // turn off timer0 for lower jitter
  ADCSRA = reset1; // set the adc to free running mode
  ADMUX = reset2; // use adc3
  DIDR0 = reset3;
  
  Serial.println("Heard 660Hz");

  //run main maze algorithm
  dfs();
}

/********************************HELPER FUNCTIONS******************************/
/* helper function that returns a bool based on whether or not a 660 hz tone 
 * was detected. Uses FFT to check if the microphone is detecting the 660 hz
 * tone.*/
bool check_mic(){
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FFT_N*2 ; i += 2) { // save 64 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf7; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
//Print statements for debugging
/*
//    Serial.println("start here");
//    //Serial.println(ADMUX);
//    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
//      Serial.println(fft_log_out[i]); // send out the data      
//    }*/
    if (fft_log_out[5] > 90) {    //660Hz
      return true;
    }      
    else {
      return false;
    }
}

/* helper function that initializes the radio stuff. Copied
 * from the radio lab.*/
void radio_init() {
  printf_begin();
  radio.begin();
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(16);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  //radio.printDetails();
}

/* Main maze mapping algorthm. Uses depth first search (DFS) for unexplored parts of
 * the maze and when back tracking uses breadth first search (BFS) to find the 
 * shortest path. */
void dfs(){
  int visitedSize = 0;
  
  //keeps track of nodes that added multiple elments into the frontier.
  StackArray <Square> backto; 

  Square visited[20];
  Square current;
  Square next;

  //done signal; to send over radio
  char done;

  //wall info; to send over radio
  char walls;

  char c_coor [] = {0,0}; //current coordinates
  char n_coor [] = {0,0}; //next coordinates
  
  int newsquare; //signal to tell if it is the first visit to current node.

  //run algorthm in loop until there are no elements in the frontier
  //which would mean that all explorable parts of the maze has been explored
  while(!frontier.isEmpty()){
    readSensor();
    if (outLeft == 0 && outRight == 0){ //check that it is at an intersection.
      if(goback == 0){
        current = frontier.pop();
        Serial.print("CURRENT:   ");
        Serial.print(current.x);
        Serial.println(current.y);
        c_coor[0] = current.x;
        c_coor[1] = current.y;

        //add to visited if first time at node
        if (!isMember(current, visited, visitedSize)){
          visited[visitedSize] = current;
          visitedSize ++;
          newsquare = 1;
        }
        else{
          newsquare = 0;
        }

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
          
          // if the neighbor to the left of node is not visited add to the frontier
          if(!isMember(next, visited, visitedSize)){
            frontier.push(next);
          }
        }
        else{ //if there is wall add to maze info that there is wall to the left
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

          // if the neighbor to the left of node is not visited add to the frontier
          if(!isMember(next, visited, visitedSize)){
            frontier.push(next);
          }
        }
        else{ //if there is wall add to maze info that there is wall to the right
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
        if(frontwall < 170){ //no wall in front
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

          // if the neighbor to the left of node is not visited add to the frontier
          if(!isMember(next, visited, visitedSize)){
            frontier.push(next);
          }
        }
        else{ //if there is wall add to maze info that there is wall in front
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
        
        // if something has been added to the frontier
        if(frontier.count()>length){
          n_coor[0] = frontier.peek().x;
          n_coor[1] = frontier.peek().y;

          // if 2 elements were added add node to backto
          if((frontier.count() - length) == 2){
              backto.push(current);
          }
          // if 3 elements were added add node to backto 2 times
          else if((frontier.count() - length) == 3){
              backto.push(current);
              backto.push(current);
          }
          goback = 0;
          orient = reorient(c_coor, n_coor, orient);       
        }
        else{ // if no new elements have been added 
          goback = 1;
        }
      }
      else{ // if signal was recieved to go back
        // node to go back to (node where robot added mutiple elements to frontier)
        Square back;
        
        next = frontier.peek();

        // if the next element in the frontier has not been visited
        if (!isMember(next, visited, visitedSize)){
          
          /* if it is a new square then you must run the shorest path function to 
           * find the path back.*/
          if(newsquare == 1){
            back = backto.pop();
            frontier.pop();
            shortest_path(current,back,maze); 
          }
          
          Square tmp = path.pop();

          //follow path back until you get to the correct backto node
          while (!squareCompare(tmp,back)){
            n_coor[0] = tmp.x;
            n_coor[1] = tmp.y;
            orient = reorient(c_coor, n_coor, orient);
            followline();
            c_coor[0] = n_coor[0];
            c_coor[1] = n_coor[1];
            tmp = path.pop();
          }

          /* do above one more time because while loop ends when tmp is equal to 
           * back node but it doesn't move to the back node*/
          n_coor[0] = tmp.x;
          n_coor[1] = tmp.y;
          orient = reorient(c_coor, n_coor, orient);
          frontier.push(tmp);
          goback = 0;
        }
        else{ // if the next element in the frontier has not been visited
          frontier.pop();
          backto.pop();
        } 
      }
      followline();
    }
    else{ // if not at intersection
    followline();
    }
//   Serial.print("size");
//  Serial.println(frontier.count());
  }

  // edge case to check if treasure is on the front side of dead end in the maze.
  if (frontier.isEmpty()){
    if(frontwall > 170 && wallL == 0 && wallL == 0){
      done = 0;
      flip();
      delay(600);
      stp();
      walls = B0000;
      walls = checkWalls(current.x,current.y);
      treasures = B000;
      find_treasures();
      for  (int i = 0; i < 3; i++ ){
      radio_send(done, treasures, walls, orient + 1, current.x, current.y);
      }
    }
  }

//  print used for debugging 
//  Serial.print("size");
//  Serial.println(frontier.count());

  //send done signal through radio
  done = 1;
  walls = B000;
  walls = checkWalls(current.x,current.y);
  treasures = B000;
  find_treasures();
  for  (int i = 0; i < 3; i++ ){
    radio_send(done, treasures, walls, orient + 1, current.x, current.y);
  }  

  //once maze is mapped just stop and do nothing
  while(frontier.isEmpty()){
    stp();
  }
}

/* Helper function that uses breadth first search to find the shortest path from one node to
 * another. However only considers visited locations to find the path because there is no 
 * information about the unexplored areas. In most mazes the shortest path ends up being 
 * the path that dfs took.*/
void shortest_path (Square a, Square b, Square_wall maze[ROWS][COLS]){
  // initialize queue for bfs
  QueueList <Square> queue;

  //make temp intersection
  Square blank;
  blank.x = 1287;
  blank.y = 1287;
  
  Square visited [20];
  int visitedSize = 0;

  // initialize best_path array
  Square_dist best_path [ROWS][COLS];
  // set array to max values
  for (int i = 0; i < ROWS; i++){
    for (int j = 0; j < COLS; j++){
      best_path[i][j].dist = 21;
      best_path[i][j].prev = blank;
    }
  }
  queue.push(a);
  bool dontstop = true;
  Square current;
  Square next;
  best_path[a.x][a.y].dist = 0;

  // keep looping until the desired node was mapped
  while ((!queue.isEmpty())|| dontstop){
    current = queue.pop();
    if (!isMember(current, visited, visitedSize)){
      visited[visitedSize] = current;
      visitedSize ++;
    }

    if (squareCompare(b, current)){
      dontstop = false;
    }

    /* Update best path array with the shortest path to each node
     * from current node*/
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
  //make path back.
  current = b;
  while(!squareCompare(a, current)){
    path.push(current);
    current = best_path[current.x][current.y].prev;
  }
}

/* This function initializes the walls in the maze information array 
 * so that the borders are assigned as walls and everything node inside 
 * the maze has no walls initiallly. */
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

/* This function returns the value to send through the radio 
 * about where the walls are at each intersection.
 * ex) B0000 -> no walls
 *     B1000 -> north wall only
 *     B1010 -> north and south wall
 *     B0110 -> east and south wall. */
char checkWalls (int x, int y) {
  char temp = B0000;
  if(maze[x][y].n == 1)
      {
        temp = temp + 8;
      }
      if(maze[x][y].e == 1)
      {
        temp = temp + 4;
      }
      if(maze[x][y].s == 1)
      {
        temp = temp + 2;
      }
      if(maze[x][y].w == 1)
      {
        temp = temp + 1;
      }
   return temp;
}

/* helper function to see if a Square a is in an array. */
bool isMember (Square a, Square visit[], int sizeV){
  for(int i = 0; i < sizeV; i ++){
    if (squareCompare(a,visit[i])){
      return true;
    }
  }
  return false;
}

/* helper function to check if Square a is equal to Square b. */
bool squareCompare (Square a, Square b){
  if(a.x == b.x){
    if(a.y == b.y){
      return true;
    }
  }
  return false;
}

/* Helper function that takes in the current node, the next node and 
 * and the current orietation and re adjusts the robot to face the 
 * next node. This helper function also checks for treasures after 
 * each adjustment and sends the intersection info over the radio.
 * This function returns the new orientation of the robot. */
char reorient(char current[], char next[], char curr_o) {
  char next_o = 0;
  char diff[] = {0, 0};
  char done = 0;
  char walls;
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
  
  if (next_o - curr_o == 0){ //straight
    walls = B000;
    walls = checkWalls(current[0],current[1]);
    forward();
    delay(200);
    stp();
    treasures = B000;
    find_treasures();
    for  (int i = 0; i < 3; i++ ){
      radio_send(done, treasures, walls, curr_o, current[0], current[1]);
    } 
  }
  else if (next_o - curr_o == 1 || next_o - curr_o == -3){ //turn right
      walls = B000;
      walls = checkWalls(current[0],current[1]);
      forward();
      delay(180);
      stp();
      //check treasure and send
      treasures = B000;
      find_treasures();
      for  (int i = 0; i < 3; i++ ){
        radio_send(done, treasures, walls, curr_o, current[0], current[1]);
      } 
      right();
      delay(680);
      left();
      delay(70);
      stp();
      //check treasure and send
      treasures = B000;
      find_treasures();
      for  (int i = 0; i < 3; i++ ){
        radio_send(done, treasures, walls, next_o, current[0], current[1]);
      } 


  }
  else if (abs(next_o - curr_o) == 2){ //flip
    walls = B000;
    walls = checkWalls(current[0],current[1]);
    flip();
    delay(580);
    stp();
    treasures = B000;
    find_treasures();
    char tmp;
    if (next_o == 0){
       tmp = 4;
    }
    else{
       tmp = next_o - 1;
    }
    for  (int i = 0; i < 3; i++ ){
      radio_send(done, treasures, walls, tmp, current[0], current[1]);
    }
    flip();
    delay(700);  
    stp();
    treasures = B000;
    find_treasures();
    for  (int i = 0; i < 3; i++ ){
      radio_send(done, treasures, walls, next_o, current[0], current[1]);
    }  
  }
  else if (next_o - curr_o == -1 || next_o - curr_o == 3){ //left
      walls = B000;
      walls = checkWalls(current[0],current[1]);
      forward();
      delay(180);
      stp();
      //check treasure and send
      treasures = B000;
      find_treasures();
      for  (int i = 0; i < 3; i++ ){
         radio_send(done, treasures, walls, curr_o, current[0], current[1]);
      }
      left();
      delay(640);
      forward();
      delay(70);
      stp();
      //check treasure and send
       treasures = B000;
      find_treasures();
      for  (int i = 0; i < 3; i++ ){
         radio_send(done, treasures, walls, next_o, current[0], current[1]);
      }
  }
  stp();
  return next_o;
}

/* helper function that finds the tresures at 7kHz, 12kHz and 17kHz using FFT
 * takes measurements from each treasure sensor 5 times. Updates global variable 
 * treasure with the findings.
 * ex) B000 -> no treasure
 *     B100 -> 17kHz
 *     B010 -> 12kHz
 *     B001 -> 7kHz
 */
void find_treasures() {
  int reset1 = ADCSRA;
  int reset2 = ADMUX;
  int reset3 = DIDR0;
  int reset4 = TIMSK0;
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x43; // use adc3
  DIDR0 = 0x04; // turn off the digital input for adc3

  // measure each treasure sensor 5 times.
  for(int i = 0; i < 10; i++){
    if (ADMUX == 0x43){
      ADMUX = 0x44;       //a4
      DIDR0 = 0x05;
    }
    else if (ADMUX == 0x44){
      ADMUX = 0x43;       //a3
      DIDR0 = 0x04;
    }
  
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FFT_N*2; i += 2) { // save 64 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
    
    //print for debugging
//    Serial.println("start here");
////    Serial.println(ADMUX);
//    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
//      Serial.println(fft_log_out[28]); // send out the data    
//        
//    }
    
    if (fft_log_out[12] > 120) {         //7kHz
      treasures |= 1;
    }
    else if (fft_log_out[20] > 135) {    //12kHz      
      treasures |= 2;
    }
    else if (fft_log_out[28] > 120) {    //17kHz
      treasures |= 4;
    }        
  }

  TIMSK0 = reset4; // turn off timer0 for lower jitter
  ADCSRA = reset1; // set the adc to free running mode
  ADMUX = reset2; // use adc3
  DIDR0 = reset3;
}

/* helper function to send information through the radio. packet size of 16.*/
bool radio_send(char done, char treasures, char walls, char curr_o, char x_coord, char y_coord){
    // First, stop listening so we can talk.
    radio.stopListening();
    
    // Take the time, and send it.  This will block until complete
    unsigned long time = millis();
 
    unsigned int new_data = 0;
    
    new_data = 1 << 15 | done << 14 | treasures << 11 | walls << 7 | curr_o << 5 | y_coord << 2 | x_coord;
    // * | D | T T T | W W W W | O O | Y Y Y | X X
//    printf("Now sending new map data\n");
    bool ok = radio.write( &new_data, sizeof(unsigned int) );

    if (ok)
      printf("ok... \n");
    else
      printf("failed.\n\r");

    delay(70);

    // Now, continue listening
    radio.startListening();
    
  }

/* helper function to read all the sensors.*/
void readSensor(){
  inLeft = analogRead(A0);
  inRight = analogRead(A1);
  frontwall = analogRead(A2);
  outLeft = digitalRead(OutLineLeft);
  outRight = digitalRead(OutLineRight);
  wallL = digitalRead(LeftWall);
  wallR = digitalRead(RightWall);
}

/***********************moving helper functions***********************/
void forward(){
  leftservo.write(180);
  rightservo.write(0);
  readSensor();
}

void right() {
  leftservo.write(130);
  rightservo.write(180);
  readSensor();
}

void left() {
  leftservo.write(65);
  rightservo.write(0);
  readSensor();
}

// fast right turn for 180 degree turns
void flip() {
  leftservo.write(180);
  rightservo.write(180);
  readSensor();
}

void stp() {
  leftservo.write(90);
  rightservo.write(90);
  readSensor();
}

void followline(){
  while (outLeft == 1 || outRight == 1){ //while both outer sensors see white
    readSensor();
    if (abs(inLeft-inRight)<30){ //if inner are similar
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
