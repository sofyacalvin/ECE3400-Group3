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

## September 29
* Attendance: Amanda, Emmett, Dylan, Sofya
* Lab 3 
  * Voted on Piazza poll
  * Audio team: David, Emmett
  * Graphics team: Amanda, Dylan, Sofya
* Reminder to fill out Team Assessment by tomorrow!
* Schmitt triggers for line sensors?
  * Narrower threshold; just need to go to open lab sometime
* Team names?
  * New York themed??
  * Memes
* Modifying website structure
  * Adding sidebar

## October 12
* Lab 3
  * Graphics
    * Pixel grouping explanation
      * By row/column
    * Need to implement Arduino communication
    * Go in 7:30 lab today
  * Audio
    * Will go in Friday
  * Nicely merged code?
  
## October 19
* All going in to lab today
* No state array yet
  * Can take info from variable and map pictures to walls and squares
  * Can use any picture, run through Matlab thing
  * State array needs:
    * Walls: undiscovered, not there, there, 3 treasures
  * Robot needs to determine orientation
* Implement 4x5 grid with 1 byte packet for alb
  * Can implement walls and other information after getting lab 4 working
  
## October 20
* Emmett, Amanda to lab today
  * Try not powering FPGA before uploading code
* Discussion of issues currently with communication
  * Pin 0 not working?
  * FPGA not working??
* Future discussion:
  * Lab days to go in
  * Perf board for Schmitt triggers
  * Finish other Schmitt triggers
  * Treasure sensors mounting
    * Code and wiring needs to be integrated
  * Do we want to use mux?
    * Microphone
  * Mounting microphone
  * Look into exploration algorithms
  * Organize state machine--what it would do in each state
* Physical switch/potentiometer for starting orientation
  * Keep track of orientation and box
  
## October 26
* Lecture content/slides
* Schmitt Triggers are done
  * Outer two lines, side wall sensors
  * Need to change code to reflect this
* Milestone 3
  * Simulation first
* Thursday night lab:
  * Everyone but Sofya can make it for some time
  * Re-implement Lab 4/SPI
  * Graphics portion
* Friday afternoon:
  * David until 3, Dylan starting 2:30, Amanda can make it
* Saturday night:
  * David tentative
* Sofya will be back at some point around here
* Monday afternoon: 
  * normal lab, we’ll all be there
* Monday night:
  * Tentative Emmett
  * Tentative Amanda
* Tuesday afternoon:
  * David
  * Amanda at some point probably
  * Emmett at some point probably
* Wednesday:
  * Dylan
  * Emmett at some point probably
  
## October 27
* Put SPI on backburner for now
* Fixing actual turning and line sensing
  * Discussion on fastest turning
* Python simulation for milestone 3
* Dylan, David, Amanda will head to lab today
  * Fix robot, continue Python simulation

## November 2
* Simulation
  * Needs finish signal
    * Add in lab tonight
  * Need video
* Lab today
  * Emmett possibly (SPI)
  * "Go to desired square, turn toward desired direction"
    * Amanda
    * Dylan
  * Arduino logic
    * David
* Walk through simulation code to implement in Arduino later
  * Need to implement in Arduino/C
  * Robot is currently functional; just need to write code
* How to communicate maze?
  * X,Y vs index
  * Sending 12 bits
    * X X X | Y Y | N E S W | T T T 
    * 1 1 1 | 1 1 | whatever  = done signal?
    * FPGA will need to determine 9x7 coordinates vs 5x4


## November 3
* Discussion of turning/destination logic
  * Notes posted in Slack
  * Will go to lab at 1:30
* DFS implementation on Arduino
  * David
* Finish signal in simulation done
  * Video will be posted on youtube
* Writeup
  * Backtracking: Sofya
  * Turning: Dylan
  * Base simulation: Amanda
  * Conversion to Arduino: David

## November 10
* Discussion of attendance?
* Packet data 
  * / | D | T T T | W W W W | O O | X X X | Y Y
* Old stuff
  * Need to finish DFS on robot
  * Fix front wall sensor sensitivity
* Milestone 4
  * Mount treasure sensors
  * Send/receive full packet of data
    * Create packets, add lab 4 code
  * Write code from previous labs into main DFS code
    * Interrupt-based
    * Treasure sensing
  * Display… everything 😥
    * Walls
    * Treasures
    * Done signal
* Else
  * Mount microphone
   * Integrate code
* Lab schedule
  * Friday
    * Emmett
    * Amanda
  * Saturday
    * Sofya at 5
    * Amanda for a period of time
    * Emmett for a period of time
  * Monday
    * Lab section
  * Tuesday
    * David
    * Emmett
  * Wednesday
    * Amanda
    * Sofya
    * David
  * Thursday
    * Sofya
    * Amanda
    * Emmett
    * David
  * Friday
    * Sofya
    * David
    * Amanda
  * Saturday
    * Uhh
  * Monday
    * Mandatory team meeting
    * Lab section
    * 😥
* Ethics homework Monday after break
  * Next Thursday team meeting
* Team evals due Saturday

##November 16
* Servos and linear regulators are ordered and on the way
* Ethics assignment
  * Essay
    * ~100 words each by Sunday
    * Intro/stakeholders: Amanda
    * Utilitarian: Dylan
    * Justice: Emmett
    * Virtue: David
    * Economic/social/political constraints, creative solutions: Sofya
* 3D printed treasure sensors during lab
* Make website nicer
  * Bootstrap
* Make graphics for maze
* Need to finish treasure sensors
* Test radio send code
* Modify finish tone (themed?)
