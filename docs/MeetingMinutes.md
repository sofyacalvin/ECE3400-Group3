# Meeting Minutes

## September 14 

* Attendance: everyone is here
* Potentially adding gears to increase speed 
  * Distribute weight over bearings in future?
* Servos go different speeds backwards: need to modify speeds
* Speed of turning
  * Not to implement a full point turn, but partial
* Sensors
  * Do we want wall sensors running at all times or @ every intersection? 
    * Will decide later
* Return to lab 2
  * Need amplifier
  * Distinguish 660Hz tone from either side
  * "nicely merged?"
* Milestone
  * speed of servos/turning
* Emmett, sofya, dylan: Friday afternoon lab
* David, amanda: Saturday @ 2 lab

## September 15
* Attendence: everyone is here
* Milestone 1
  * Fixed common ground issue
    * Which fixed significant speed difference of servos
      * Switched to pin 11 for right servo, need to change figure 8 code accordingly
  * Can find running average of sensor data
    * Long run: memory issue
  * Time-based turn versus sensor-based turn
  * Test figure 8 code in lab later today
    * Additionally fix turns if time permits
  * Add rubber bands
  * Build amplifier circuit for treasures
  * Video of line following and figure 8
* Lab 2
  * Saturday lab
  * Band pass filter for microphone
    * Wasn't working with Arduino
  * FFT audio and treasure code on Github
* Reports
  * Emmett: Fourier part, audio
  * Amanda: oscilloscope testing, treasures
  * Sofya: hardware, schematics
  * Dylan: software (milestone)
  * David: wiring, testing, debugging, improvements
  
## September 21
* Attendance: everyone is here
* Milestone 2
* Analog: 4 line sensors, 3 wall sensors, 2 IR,  (+1 microphone in future)
  * Schmitt Trigger
    * What can be digital
    * 6 analog pins: 2 line, 2 IR,  1 wall (front facing)
* Digital: 2 line (outer), 2 wall (sides)
* Change line sensors?
  * 5: like in lecture, one in center--potential for feedback control (PID)? 
    * Slightly more precise, more memory vs current implementation?
  * Implement PID with our current 4 line sensors?
    * To worry about in the future
* For now:
  * Use 3 short sensors
  * Add 2 side wall sensors to be digital 
  * Front wall sensor to be analog
  * Switch 2 outside line to digital
  * Add 2 IR sensors: both analog
    * Add code to “signal”
    
## September 28
Attendance: everyone is here
* Monday status:
  * No line sensor Schmitt triggers yet
  * Inputs/outputs for Arduino are all unplugged but power and ground currently ready
  * Extended Arduino pins for easier access
* Lab 3: lab due date?
  * Fall break??
  * Prelims :(
  * Thursday meeting next week 
    * Meet on Friday instead
  * Make sure Quartus works
  * Prelab!
* Team assessment—due Saturday
