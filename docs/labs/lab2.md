# Lab 2: Analog Circuitry and FFTs
Acoustic team: David Kim, Dylan Machado, Emmett Milliken

Optical team: Sofya Calvin, Amanda Trang


## Objective 
The goal of this lab was to get familiar with the microphone and the IR sensor that we would be adding to our robot. We did this by using the Open Music Labs Arduino FFT library to map the Fourier Transforms of the signals that the sensors were detecting. Additionally, we utilized op-amps to create filters and amplifiers for the audio and optical signals. In the future, these will be incorporated onto the robot to detect a 660Hz start signal as well as IR-emitting treasures throughout the map.

## Procedure
We initially split into two teams: Acoustic and Optical. The Acoustic team (David, Dylan, Emmett) focused on the microphone circuit, while the Optical team (Sofya, Amanda) worked with the IR sensor. Each team was to do the Fourier analysis corresponding to their circuit. As we did not finish during our lab hours, much of the lab was completed with varying members of the team during open lab.

### FFT analysis
For both the acoustic and optical parts of this lab, fast fourier transforms (FFT) were used to find the frequency content of the sampled input signal from the sensors. 

The FFT library takes in an analog signal, samples it, and calculates the frequency content. The maximum frequency that the fft can accurately detect is half of the sampling frequency.

The FFT takes in 256 samples taken at equally spaced intervals, and outputs 256 values that represent the frequency content of the input signal. The 256 output values are the calculated frequency content within certain bins, or range of frequencies. The FFT of a real signal is symmetric over zero, only half of the outputs are unique. This is why sets of 128 values are output to serial.

In order to visualize the data coming out of the arduino, we copied sets of the data from the serial window into Excel. We initially spent a fair bit of time trying to get MATLAB to display the frequency content in real time, but ended up not being very successful.

Our Excel graphs of the frequency content of our signals can be found later on this page.

### Amplifier/microphone circuit
When initially setting up the microphone, we were somewhat confiused by the diagram given to us:
//insert pic here
We were conflused because the microphone we had was already soldered on a board with other components. 

So we ended up adding the filter in addition to what was already on the board. The good news is that it worked out ok. We were able to measure peak. We doubled the frequency of the tone, and there was a peak about twice as far up the spectrum.


### 660Hz tone detection
text
![Audio Spectrum](../images/lab2/lab2audiofreqspectrum.png)
### IR sensor circuit
The goal of the Optical team was to detect a 7kHz IR beacon through the Arduino and perform a Fourier analysis on the signal. We first created a simple circuit (shown below) to detect the IR-emitting treasure. 

![Phototransistor Circuit](../images/lab2/Lab2_Phototransistor.jpg "Phototransistor Circuit")

![Phototransistor Photo](../images/lab2/simple_IR.jpg "Phototransistor Photo")

The operating principle of the phototransistor is that it allows more current to pass with the more light it receives; and, similarly, less current to pass with less light. We were able to test the functionality of this simple circuit by using the oscilloscope. The output voltage would correctly lower when the phototransistor was covered (i.e. exposed to no light). 

We set the treasures by attaching the output to the oscilloscope and adjusting the amplitude and frequency. This is indicated by channel 2 in blue in the image below. By holding the treasure against the phototransistor, we could see the effect in the output on the oscilloscope, as seen on channel 1 in yellow. The Fourier analysis will be discussed in the following section.

![Treasure on Oscilloscope](../images/lab2/treasure_osc.jpg "Treasure on Oscilloscope")

After initial testing, we determined that the signal received from the treasure was too weak at a realistic distance, and decided to implement an amplifier circuit. Using the LM358 op-amp and 1kΩ and 100Ω resistors, we created a simple non-inverting amplifier with a gain of approximately 10.

![Non-inverting Amplifier](../images/lab2/noninvrt.gif "Non-inverting Amplifier")
![LM358](../images/lab2/opamp.png "LM358")

The actual implementation is as shown, with the treasure transmitting a signal at the top.

![Phototransistor filtered](../images/lab2/treasure_filtered.jpg "Phototransistor filtered")
//have a picture
![Optical Spectrum](../images/lab2/lab2treasurefreqspectrum.png)
### IR treasure blinking at 7kHz, 12kHz and 17kHz
After determining the circuit could detect the IR signal successfully (before the amplifier was implemented), we ran the data through the FFT from the Open Music Labs library. We worked off the example code offered on their site. We modified the ADC clock prescalar %%% by modifying the following line:

// @ David what did we end up using?

```
  ADCSRA = 0xe5; // set the adc to free running mode
```

// also @ david, feel free to talk about the calculation of bins or something here

//i'm gonna add more give me a minute

The code in its entirety can be viewed below:

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
  }
}
```

[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
