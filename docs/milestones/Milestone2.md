# Milestone 2

## Objective
The primary goal of Milestone 2 is to be able to distinguish between three treasure frequencies--7kHz, 12kHz, and 17kHz--using our FFT with the IR phototransistor. Additionally, we implemented a short-range wall sensor in order to autonomously detect walls.

## Procedure

### Treasure Detection
Most of the work for this lab had been completed in [Lab 2](../labs/lab2.md), in which we mapped the Fourier transforms of the IR signal detected. To complete this milestone, we added LEDs to our preexisting IR detection circuit (with amplification) to individually light up upon the correct signal of a treasure.

The circuit with the LEDs can be viewed below:

![IR and LED circuit](../images/milestone2/IR_distinguish.jpg)

We referenced our amplified treasure detection data from the previous lab:

![IR Data (amplified)](../images/lab2/Treasure(7kHz,12kHz,17kHz).jpg)

This data shows that bins 48, 81, and 114 correspond to the 7kHz, 12kHz, and 17kHz treasures, respectively. On average, the frequency of these bins is between 140-160--to be safe, we set our threshold of detection at 130. We added a simple conditional to check the value of the desired bins as follows:

```
... 

if (fft_log_out[48] > 130){         //7kHz, white
      digitalWrite(9, HIGH);  
    }
    else if (fft_log_out[81] > 130){    //12kHz, green
      digitalWrite(10, HIGH); 
    }
    else if (fft_log_out[114] > 130) {  //17kHz, red
      digitalWrite(11, HIGH);
    }
   else {                               //turn all off
      digitalWrite(9, LOW);   
      digitalWrite(10, LOW);  
      digitalWrite(11, LOW);
   }
```

We set up digital pin 9 to be a white LED, 10 to green, and 11 to red. These correspond to 7, 12, and 17kHz. When the treasures are presented in front of the IR phototransistor, the idea was that the corresponding LED would light up. Our test was successful, and can be seen in the following video:

[![Treasure Detection](http://img.youtube.com/vi/pzk-h_Un0u4/0.jpg)](http://www.youtube.com/watch?v=pzk-h_Un0u4)

For additional testing, we set the treasure to 8kHz, 13khz, and 18kHZ to ensure our bins were small enough to not detect extraneous frequencies. This was also successful--the LEDs did not light up. 

The full code for this section is pasted below:

```
/*
fft_adc_serial.pde
guest openmusiclabs.com 7.7.14
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.
*/

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
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
    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      Serial.println(fft_log_out[i]); // send out the data
    }
    if (fft_log_out[48] > 130){         //7kHz, white
      digitalWrite(9, HIGH);  
    }
    else if (fft_log_out[81] > 130){    //12kHz, green
      digitalWrite(10, HIGH); 
    }
    else if (fft_log_out[114] > 130) {  //17kHz, red
      digitalWrite(11, HIGH);
    }
   else {                               //turn all off
      digitalWrite(9, LOW);   
      digitalWrite(10, LOW);  
      digitalWrite(11, LOW);
   }
  }
}
```

In the future, we will have two IR detectors (one on each side) and mount them on Brooklynn. We will likely maintain the simple LED setup to flag when a treasure is detected.

### Autonomous Wall Detection
The other goal of this milestone was to get Brooklynn to detect a wall autonomously. To do this, we attached a short range IR sensor to the front of Brooklynn. The sensor was then attached to our Arduino via an analog pin for us to calibrate its sensor values. From there, we used snippets of our previous line detection and figure 8 code from Milestone 1 in addition to new wall direction code to make the robot turn left when a wall was detected in front of the robot at the intersection, since walls in front of Brooklynn will only occur at intersections. Our resulting code looked like the pseudocode below:

```
**line following code**
if (at an intersection and a wall is detected in front)
	turn left
The full code can be found at the bottom of this page.
```
In addition, a video of Brooklynn autonomously detecting walls and turning is below:

[![Wall Detection - Left Turn](http://img.youtube.com/vi/U8qxujkT1dE/0.jpg)]https://www.youtube.com/watch?v=U8qxujkT1dE)
In the future, we will use two more short range IR sensors, one on each side of Brooklynn, to detect walls on her respective sides. This will help in mapping out the maze, as well as allow Brooklynn to determine which direction she needs to turn when at an intersection. These sensors have been mounted and correctly detect walls like the front sensor, but are not in use at the moment.


## Build Changes

### Schmitt Triggers

[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
