#include <Goertzel.h>

int sensorPin = A0;
int led = 13;

const float TARGET_FREQUENCY = 660;
const int N = 100;
const float THRESHOLD = 1000;
const float SAMPLING_FREQUENCY = 8900;
Goertzel goertzel = Goertzel(TARGET_FREQUENCY, N, SAMPLING_FREQUENCY);

bool start = false;

void setup() {
  //Serial.begin(9600);
  //pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  check_mic();
//  while(start){
//    Serial.println("going");
//  }
//  
}

void check_mic(){
  goertzel.sample(sensorPin); //Will take n samples
  float magnitude = goertzel.detect();  //check them for target_freq
  if (magnitude > THRESHOLD){ //if you're getting false hits or no hits adjust this
    //digitalWrite(LED_BUILTIN, HIGH); //if found, enable led
    start = true;
  }
  else{
    //digitalWrite(LED_BUILTIN, HIGH); //if found, enable led
  }
   
}

