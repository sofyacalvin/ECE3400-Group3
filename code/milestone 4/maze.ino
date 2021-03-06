#define LOG_OUT 1 // use the log output function
#define FFT_N 64 // set to 64 point fft

#include <FFT.h> // include the library
int treasures = B000;

#include <StackArray.h>
#include <Servo.h>

#define ROWS 4
#define COLS 5

#define LeftWall 3
#define RightWall 4
#define OutLineLeft 2
#define OutLineRight 7

uint8_t *heapptr, *stackptr; // global variable

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

Square visited[21];
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

      n_coor[0] = frontier.peek().x;
      n_coor[1] = frontier.peek().y;
      if(frontier.count()>length){
        goback = 0;
        orient = reorient(c_coor, n_coor, orient);
        path.push(current);
//        check_mem(); 
//        Serial.write("heapptr: "); Serial.print((int)heapptr);
//        Serial.write(", stackptr: "); Serial.println((int)stackptr);
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
          orient = reorient(c_coor, n_coor, orient);
          goback = 0;
        }
      }
      else{
        frontier.pop();
      }
    }
    Serial.print("size  ");
    Serial.println(frontier.count());
    followline();
  }
//  Serial.print("size");
//  Serial.println(frontier.count());
  Serial.println("DONE");
  while(frontier.isEmpty()){
    stp();
  }

}


//void check_mem() {
//  stackptr = (uint8_t *)malloc(4);  // use stackptr temporarily
//  heapptr = stackptr;               // save value of heap pointer
//  free(stackptr);                   // free up the memory again (sets stackptr to 0)
//  stackptr =  (uint8_t *)(SP);      // save value of stack pointer
//}

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

  stp();
  return curr_o;
}

void find_treasures() {
  int reset1 = ADCSRA;
  int reset2 = ADMUX;
  int reset3 = DIDR0;
  int reset4 = TIMSK0;
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x43; // use adc3
  DIDR0 = 0x04; // turn off the digital input for adc3
  
  //while(1) { // reduces jitter
    if (ADMUX == 0x43){
      ADMUX = 0x44;       //a4
      DIDR0 = 0x05;
    }
    else if (ADMUX == 0x44){
      ADMUX = 0x43;       //a3
      DIDR0 = 0x04;
    }
  
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 128 ; i += 2) { // save 64 samples
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
    Serial.println("start");
    //Serial.println(ADMUX);
    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      //Serial.println(fft_log_out[i]); // send out the data      
    }
    
    if (fft_log_out[12] > 60) {         //7kHz
      treasures |= 1;
//      digitalWrite(7, HIGH);
    }
    else if (fft_log_out[21] > 50) {    //12kHz      
      treasures |= 1 << 1;
//      digitalWrite(7, HIGH);
    }
    else if (fft_log_out[28] > 53) {    //17kHz
      treasures |= 1 << 2;
//      digitalWrite(7, HIGH);
    }        
    else {
//      digitalWrite(7, LOW);
    }

    Serial.println((int)treasures); // send out the data
     TIMSK0 = reset4; // turn off timer0 for lower jitter
     ADCSRA = reset1; // set the adc to free running mode
     ADMUX = reset2; // use adc3
     DIDR0 = reset3;
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
  find_treasures();
  stp();
}
