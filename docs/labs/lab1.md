# Lab 1: Microcontrollers
Team 1: Amanda Trang, Emmett Milliken
Team 2: David Kim, Dylan Machado, Sofya Calvin

## Objective 
The goal of this lab was to introduce the concepts of the Arduino IDE as well as the Arduino Uno microcontroller itself. Additionally, we formed a basic structure for the robot and added a simple autonomous function.

## Procedure
We used the Arduino IDE to program write and upload our code to the Uno. To install the Arduino IDE, go to https://www.arduino.cc/en/Main/Software.

### Blinking an internal LED
In order to test whether or not our connection with the board was working, we used the example sketch, Blink (File>Examples>01.Basics>Blink). After uploading the sketch to the board, the on-board LED toggled on and off once per second, verifying that our connection and board were working. 

### Blinking an external LED
We then modified the Blink sketch to blink an external LED through a digital output pin. Since digital output pins on the Uno will stop working if they are used to source too much current, we added a 330 ohm resistor in series with an LED. 

### Reading the value of a potentiometer via the serial port

### Map the value of the potentiometer to the LED

### Map the value of the potentiometer to the servo

### Assemble your robot

### Driving your robot autonomously

## Conclusion

[Return to home](/docs/index.md)
