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
Sending the whole maze on each loop is evidently not the most efficient way to do it--particularly in relation to power consumption, if this were a larger-scale project. Instead, we chose to send only new, changing data (i.e. new position, discovered treasure, etc.). By initializing x and y coordinate variables as well as a "state" variable (called *x_coord, y_coord* and *pos_data* respectively), we will be able to simple increment the desired values to display the robot's position.

|   | 0          | 1          | 2          | 3          | 4          |
|---|------------|------------|------------|------------|------------|
| 0 | 000 00 xx | 001 00 xx | 010 00 xx | 100 001 xx | 101 001 xx |
| 1 | 000 01 xx | 001 01 xx | 010 01 xx | 100 010 xx | 101 010 xx |
| 2 | 000 10 xx | 001 10 xx | 010 10 xx | 100 011 xx | 101 011 xx |
| 3 | 000 11 xx | 001 11 xx | 010 11 xx | 100 100 xx | 101 100 xx |

This table is the binary representation of our 4x5 grid. 

To send the new data, we created a new variable (called *new_data*) as the packet to send to the base station. This is a 7-bit piece of information, in which the first three bits display the x position, the next two are y position, and the last two are state data. In order to display the data this way, we shift the x and y position data to fit next to the data. The debugging for the packet data is described later in this report.

`
new_data = x_coord << 4 | y_coord << 2 | pos_data;
// x x x | y y | d d
`

To actually send this data, we follow a similar process in sending the whole maze. We send the new data through radio.write() again. 

`
printf("Now sending new map data\n");
bool ok = radio.write( &new_data, sizeof(unsigned char) );
if (ok)
  printf("ok... \n");
else
  printf("failed.\n\r");
// Now, continue listening
radio.startListening();
`

To verify the data being transmitted and received, we simply read the data back and parse the string back into bits--which may not be the most efficient way to do it, but made debugging much simpler by seeing the binary represenation of the packet instead of binary.

`
String got_string = String(bitRead(got_data_t, 6)) + String(bitRead(got_data_t, 5)) + String(bitRead(got_data_t, 4)) + " " + String(bitRead(got_data_t, 3)) + String(bitRead(got_data_t, 2)) + " " + String(bitRead(got_data_t, 1)) + String(bitRead(got_data_t, 0));
// Spew it
Serial.println("Got response " + got_string);
`

On the receiving end, we similarly declare the variable of the data received (*got_data*) and verify if it was receieved. Using radio.read() we are able to take that data, print the result to the serial monitor. 

`
unsigned char got_data;      
bool done = false;
while (!done) {
// Fetch the payload, and see if this was the last one.
done = radio.read( &got_data, sizeof(unsigned char) );
// Spew it
// Print the received data in binary
String bin_string = String(bitRead(got_data, 6)) + String(bitRead(got_data, 5)) + String(bitRead(got_data, 4)) + " " + String(bitRead(got_data, 3)) + String(bitRead(got_data, 2)) + " " + String(bitRead(got_data, 1)) + String(bitRead(got_data, 0));
printf("Got payload... ");
Serial.println(bin_string);
// Delay just a little bit to let the other unit
// make the transition to receiver
delay(20);
`


For now, we are simulating exploration by methodically incrementing the data to travel the entire grid.

### FPGA

#### Displaying full 4x5 grid
As in lab 3, we split the screen into rows and then columns using nested case statements. These nested case statements create the necessary 4x5 grid and allow us to set the pixel color within each square. Previously the color had been hard coded, but now is determined by data sent from the Ardiuno.
In order to store the incoming data, we created a 4x5 array of 2-bit values. This array is updated every time the FPGA recieves information from the Arduino. 

#### Arduino and FPGA maze communication

#### Displaying exploration

[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
