# Lab 1: Microcontrollers
Team 1: Amanda Trang, Emmett Milliken
Team 2: David Kim, Dylan Machado, Sofya Calvin

## Objective 
The goal of this lab was to introduce the concepts of the Arduino IDE as well as the Arduino Uno microcontroller itself. Additionally, we formed a basic structure for the robot and added a simple autonomous function.

## Procedure
We used the Arduino IDE to program write and upload our code to the Uno. To install the Arduino IDE, go to https://www.arduino.cc/en/Main/Software.

### Blinking an internal LED
In order to test whether or not our connection with the board was working, we used the example sketch, Blink (File>Examples>01.Basics>Blink). After uploading the sketch to the board, the on-board LED toggled on and off once per second, verifying that our connection and board were working. 

```
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
```

### Blinking an external LED
We then modified the Blink sketch to blink an external LED through a digital output pin. Since digital output pins on the Uno will stop working if they are used to source too much current, we added a 330 ohm resistor in series with an LED. 

```
void setup() {
  // initialize digital pin 12 as an output.
  pinMode(12, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
```

### Reading the value of a potentiometer via the serial port

used code from https://www.arduino.cc/en/Tutorial/ReadAnalogVoltage
```
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.println(voltage);
```

### Map the value of the potentiometer to the LED

```
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(3,OUTPUT);
}

void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  analogWrite(3,sensorValue/4);
  // print out the value you read:
  Serial.println(voltage);
}
```

### Map the value of the potentiometer to the servo

```
#include <Servo.h>
Servo pin;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //pinMode(3,OUTPUT);
  pin.attach(9);
}

void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
 pin.write(sensorValue/5.68);
  // print out the value you read:
  Serial.println(sensorValue*180.0/1023.0);
}
```

### Assemble your robot

### Driving your robot autonomously
After assembling our simple robot, we wrote a simple program to attempt to run the robot in a square formation. We had to manually recalibrate the servos, as writing the value 90 to either one did not result in it stopping. We initialized both servos (named servoL and servoR for left and right, respectively):

```
#include <Servo.h>
Servo servoL;
Servo servoR;

void setup() {
  Serial.begin(9600);
  servoL.attach(9);
  servoR.attach(10);
  //pinMode(11, OUTPUT);
  //pinMode(12, OUTPUT);
}
```

and wrote a forward() function as well as a right() function.

```
void forward(int msec) {
  servoL.write(180);
  servoR.write(0);
  //digitalWrite(11, HIGH);
  //digitalWrite(12, HIGH);
  delay(msec);
}

void right(int msec) {
  servoL.write(180);
  servoR.write(90);
  //digitalWrite(11, HIGH);
  //digitalWrite(12, LOW);
  delay(msec);
}
```

We also added in two LEDs that flashed their respective colors to visually see whether the code was running "forward" or "right."  In reality, the robot went in a very questionable quadrilateral-like shape, as we did not have enough time to fine-tune the time values. Any code relating to the LEDs has been commented out due to an issue causing the servos to run incorrectly. If the LEDs are implemented in the future, we will debug this code.

```
void loop() {
  forward(5000);
  right(2000);
  forward(5000);
  right(2000);
  forward(5000);
  right(2000);
  forward(5000);
}
```


## Conclusion

[Return to home](/docs/index.md)
