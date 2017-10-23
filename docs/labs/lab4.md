# Lab 4: Radio Communication and Map Drawing
Radio team: Sofya Calvin, Amanda Trang, Dylan Machado

FPGA team: Emmett Milliken, David Kim

## Objective 
The objective of this lab

## Procedure

### Radio

#### Wireless sending between Arduinos

#### Sending whole maze

#### Sending new data


### FPGA

#### Displaying full 4x5 grid
As in lab 3, we split the screen into rows and then columns using nested case statements. These nested case statements create the necessary 4x5 grid and allow us to set the pixel color within each square. Previously the color had been hard coded, but now is determined by data sent from the Ardiuno.
In order to store the incoming data, we created a 4x5 array of 2-bit values. This array is updated every time the FPGA recieves information from the Arduino. 


Binary values: 

row | col | data
xxx | xx | xx

|   | 0          | 1          | 2          | 3          | 4          |
|---|------------|------------|------------|------------|------------|
| 0 | 000 00 xx | 001 00 xx | 010 00 xx | 100 001 xx | 101 001 xx |
| 1 | 000 01 xx | 001 01 xx | 010 01 xx | 100 010 xx | 101 010 xx |
| 2 | 000 10 xx | 001 10 xx | 010 10 xx | 100 011 xx | 101 011 xx |
| 3 | 000 11 xx | 001 11 xx | 010 11 xx | 100 100 xx | 101 100 xx |

#### Arduino and FPGA maze communication

#### Displaying exploration

[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
