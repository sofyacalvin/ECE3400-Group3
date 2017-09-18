# Lab 2: Analog Circuitry and FFTs
Acoustic team: David Kim, Dylan Machado, Emmett Milliken

Optical team: Sofya Calvin, Amanda Trang


## Objective 
The goal of this lab was to get familiar with the microphone and the IR sensor that we would be adding to our robot. We did this by using the Open Music Labs Arduino FFT library to map the Fourier Transforms of the signals that the sensors were detecting. Additionally, we utilized op-amps to create filters and amplifiers for the audio and optical signals. In the future, these will be incorporated onto the robot to detect a 660Hz start signal as well as IR-emitting treasures throughout the map.

## Procedure
We initially split into two teams: Acoustic and Optical. The Acoustic team (David, Dylan, Emmett) focused on the microphone circuit, while the Optical team (Sofya, Amanda) worked with the IR sensor. Each team was to do the Fourier analysis corresponding to their circuit. As we did not finish during our lab hours, much of the lab was completed with varying members of the team during open lab.

### FFT analysis
text

### Amplifier/microphone circuit
text

### 660Hz tone detection
text

### IR sensor circuit
The goal of the Optical team was to detect a 7kHz IR beacon through the Arduino and perform a Fourier analysis on the signal. We first created a simple circuit (shown below) to detect the IR-emitting treasure. 

![Phototransistor Circuit](../images/lab2/Lab2_Phototransistor.jpg "Phototransistor Circuit")

![Phototransistor Photo](../images/lab2/simple_IR.jpg "Phototransistor Photo")

//how to change size??

The operating principle of the phototransistor is that it allows more current to pass with the more light it receives; and, similarly, less current to pass with less light. We were able to test the functionality of this simple circuit by using the oscilloscope. The output voltage would correctly lower when the phototransistor was covered (i.e. exposed to no light). 

We set the treasures by attaching the output to the oscilloscope and adjusting the amplitude and frequency. This is indicated by channel 2 in blue in the image below. By holding the treasure against the phototransistor, we could see the effect in the output on the oscilloscope, as seen on channel 1 in yellow. The Fourier analysis will be discussed in the following section.

![Treasure on Oscilloscope](../images/lab2/treasure_osc.jpg "Treasure on Oscilloscope")

After initial testing, we determined that the signal received from the treasure was too weak at a realistic distance, and decided to implement an amplifier circuit. Using the LM358 op-amp and 1kΩ and 100Ω resistors, we created a simple non-inverting amplifier with a gain of approximately 10.

![Non-inverting Amplifier](../images/lab2/noninvrt.gif "Non-inverting Amplifier")

//not done yet i'm just saving it lol

### IR treasure blinking at 7kHz, 12kHz and 17kHz
text

[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
