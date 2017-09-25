# Milestone 2

## Objective
The primary goal of Milestone 2 is to be able to distinguish between three treasure frequencies--7kHz, 12kHz, and 17kHz--using our FFT with the IR phototransistor. Additionally, we implemented a short-range wall sensor in order to autonomously detect walls.

## Procedure

### Treasure Detection
Most of the work for this lab had been completed in [Lab 2](https://sofyacalvin.github.io/ece3400-group3/labs/lab2.html), in which we mapped the Fourier transforms of the IR signal detected. To complete this milestone, we added LEDs to our circuit to individually light up upon detection of a treasure. We referenced our data from the previous lab:

![IR Data (amplified)](../images/lab2/Treasure(7kHz,12kHz,17kHz).jpg)

This data shows that bins 48, 81, and 114 correspond to the 7kHz, 12kHz, and 17kHz treasures (respectively.)

### Autonomous Wall Detection

[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
