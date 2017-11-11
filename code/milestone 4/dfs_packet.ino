#define LOG_OUT 1 // use the log output function
#define FFT_N 64 // set to 64 point fft

#include <FFT.h>
#include <StackArray.h>
#include <Servo.h>
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

RF24 radio(9,10);
const uint64_t pipes[2] = { 0x0000000006LL, 0x0000000007LL };

unsigned char walls = B1111; // N E S W
unsigned char treasures = 0; // 17 | 12 | 7
unsigned char done = 0;

Square visited[20];

StackArray <Square> frontier;
StackArray <Square> path;
Square current;
int visitedSize;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x43; // use adc3
  DIDR0 = 0x04; // turn off the digital input for adc3
  
  printf_begin();
  leftservo.attach(5);
  rightservo.attach(6);
  pinMode(OutLineLeft, INPUT);
  pinMode(LeftWall, INPUT);
  pinMode(RightWall, INPUT);
  pinMode(OutLineRight, INPUT);
  Square start;
  start.x = 0;
  start.y = 0;
  frontier.push(start);
  orient = 1;
  
  radio.begin();

  // optionally, increase the delay between retries & # of retries
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
  radio.setPayloadSize(15);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
}


// the loop routine runs over and over again forever:
void loop() {
  walls = B1111;
  treasures = B000;
  dfs();
}

/********************************HELPER FUNCTIONS******************************/
void dfs(){
  Square current;
  Square next;
  char c_coor [] = {0,0};
  char n_coor [] = {0,0};
  //followline();

  while(!frontier.isEmpty()){

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
        path.push(current);
      }
      int length = frontier.count();
      if(wallL == 1){ //no wall on left
        if(orient == 3){ // W
          walls &= ~(1 << 1);
          next.x = current.x + 1;
          next.y = current.y;
        }
        else if(orient == 2){ // S
          walls &= ~(1 << 2);
          next.x = current.x;
          next.y = current.y + 1;
        }
        else if(orient == 1){
          walls &= ~(1 << 3);
          next.x = current.x - 1;
          next.y = current.y;
        }
        else if(orient == 0){
          walls &= ~(1 << 0);
          next.x = current.x;
          next.y = current.y - 1;
        }
        if(!isMember(next, visited, visitedSize)){
          frontier.push(next);
        }
      }
      if(wallR == 1){ //no wall on right
        if(orient == 3){
          walls &= ~(1 << 3);
          next.x = current.x - 1;
          next.y = current.y;
        }
        else if(orient == 2){
          walls &= ~(1 << 0);
          next.x = current.x;
          next.y = current.y - 1;
        }
        else if(orient == 1){
          walls &= ~(1 << 1);
          next.x = current.x + 1;
          next.y = current.y;
        }
        else if(orient == 0){
          walls &= ~(1 << 2);
          next.x = current.x;
          next.y = current.y + 1;
        }
        if(!isMember(next, visited, visitedSize)){
          frontier.push(next);
        }
      }
      if(frontwall < 200){ //no wall in front
        if(orient == 3){
          walls &= ~(1 << 0);
          next.x = current.x;
          next.y = current.y - 1;
        }
        else if(orient == 2){
          walls &= ~(1 << 1);
          next.x = current.x + 1;
          next.y = current.y;
        }
        else if(orient == 1){
          walls &= ~(1 << 2);
          next.x = current.x;
          next.y = current.y + 1;
        }
        else if(orient == 0){
          walls &= ~(1 << 3);
          next.x = current.x - 1;
          next.y = current.y;
        }
        if(!isMember(next, visited, visitedSize)){
          frontier.push(next);
        }
      }
      if(frontier.count()>length){
        goback = 0;
      }
      else{
        goback = 1;
      }
      n_coor[0] = frontier.peek().x;
      n_coor[1] = frontier.peek().y;
      Serial.print("NEXT:   ");
      Serial.print(next.x);
      Serial.println(next.y);

      radio_send(orient, current.x, current.y);
      
      orient = reorient(c_coor, n_coor, orient);
//      followline();
    }
    else{
      next = frontier.pop();
      if (!isMember(next, visited, visitedSize)){
        if(frontier.count() > 0){
          frontier.push(path.pop());
          goback = 0;
        }
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

void radio_send(char curr_o, char x_coord, char y_coord){
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    unsigned long time = millis();
    unsigned int new_data;

    new_data = done << 14 | treasures << 11 | walls << 7 | curr_o << 5 | x_coord << 2 | y_coord;
    // * | D | T T T | W W W W | O O | X X X | Y Y
        
    printf("Now sending new map data\n");
    bool ok = radio.write( &new_data, sizeof(unsigned int) );
    
    if (ok)
      printf("ok... \n");
    else
      printf("failed.\n\r");

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {

    unsigned long got_data_t;
      radio.read( &got_data_t, sizeof(unsigned long) );
      String got_string = "x= " +  String(bitRead(got_data_t, 14)) + String(bitRead(got_data_t, 13)) + String(bitRead(got_data_t, 12)) + String(bitRead(got_data_t, 11)) + String(bitRead(got_data_t, 10)) + String(bitRead(got_data_t, 9)) + String(bitRead(got_data_t, 8)) + String(bitRead(got_data_t, 7)) + String(bitRead(got_data_t, 6)) + ", y= " + String(bitRead(got_data_t, 5)) + String(bitRead(got_data_t, 4)) + String(bitRead(got_data_t, 3)) + ", data= " + String(bitRead(got_data_t, 2)) + String(bitRead(got_data_t, 1)) + String(bitRead(got_data_t, 0));
      Serial.println("Got response " + got_string);
      }

    // Try again 1s later
    delay(250); 
}

void find_treasures() {
  //while(1) { // reduces jitter
    if (ADMUX == 0x43){
      ADMUX = 0x44;
      DIDR0 = 0x05;
    }
    else if (ADMUX == 0x44){
      ADMUX = 0x43;
      DIDR0 = 0x04;
    }
  
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 128 ; i += 2) { // save 256 samples
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
    //Serial.println("start");
    //Serial.println(ADMUX);
    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      //Serial.println(fft_log_out[i]); // send out the data      
    }
    
    if (fft_log_out[12] > 100) { //7kHz
      treasures |= 1;
    }
    if (fft_log_out[20] > 100) { //12kHz      
      treasures |= 1 << 1;
    }
    if (fft_log_out[28] > 100) { //17kHz
      treasures |= 1 << 2;
    }        
    
    Serial.println(treasures); // send out the data
    
  }
