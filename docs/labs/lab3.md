# Lab 3: FPGA Video Controller and Sound Generation
Graphics team: Sofya Calvin, Amanda Trang, Dylan Machado

Audio team: Emmett Milliken, David Kim

## Objective 
This lab has two main goals: one, to take external inputs from the Arduino to the FPGA and display them on a screen through VGA; and two, to generate a three-tone sound to a speaker via the 8-bit DAC. The graphics portion is intended to be a stepping stone toward a final goal of mapping the maze on-screen during competition, while the audio portion will eventually signal the completion of the maze.

## Procedure

### Graphics
#### DAC on VGA connectors

+ calculating resistor values

#### Drawing one box



#### Updating array dependent on inputs

+ drawing the whole colorful grid

#### Reading external inputs to FPGA

With an objective of taking in at least two inputs from the Arduino, we took a simple route of outputting toggling digital signals from the Arduino on loop. Outputting to digital pins 12 and 13, we alternated between sending (0,0), (0,1), (1,0), and (1,1) with 1.5 second intervals. This would create the desired four states.

```
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(1500);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  delay(1500);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  delay(1500);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  delay(1500);
}
```

We also knew the Arduino runs on a 5V scale, whereas the FPGA uses 3.3V. We designed a simple voltage divider to pull down the voltage as follows:

schematic!

These values were calculated using [Ohms Law Calculator](http://www.ohmslawcalculator.com/voltage-divider-calculator). We connected the pins from the Arduino to this circuit, and the output of the voltage divider to the FPGA.

We wanted to ensure our signal was toggling as desired, so we hooked it up to the oscilloscope to view the signals from each pin. The oscilloscope showed us that it was toggling as desired:

![Toggling signal](../images/lab3/toggle.png)

In order to check if our signals were being read correctly, we wrote the LEDs on the FPGA to toggle in accordance with the two signals (i.e. LED1 turned on when switch_1 (from the Arduino) went high, and the same thing for LED2 and switch_2). The debugging process of this is described later in this report. The following is a quick clip of what the LEDs looked like with the toggling signal:

![FPGA LEDs reading signals][http://img.youtube.com/vi/l2PiRnfkoK0/0.jpg](https://www.youtube.com/watch?v=l2PiRnfkoK0)

The left two blinking LEDs correspond to the changing signals, while the rightmost LED blinking was from the Lab 3 template code, in order to show it was running.

#### Mapping external inputs on screen

Wanting to use these signals to change the colors on-screen, we returned to Quartus to modify the existing colored grid code. Out of simplicity, we modified the first square on the first two rows--turning them white when its respective signal went high.

```
always @ (posedge CLOCK_50) begin
	 		case(PIXEL_COORD_Y / 120)
			4'd0 : 												// row A
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = (switch_1) ? 8'b111_111_11: 8'b111_000_00;

...

			4'd1 : 												// row B
							case(PIXEL_COORD_X / 120)
								4'd0 : PIXEL_COLOR = (switch_2) ? 8'b111_111_11: 8'b111_111_00;					
```

This gave us four different states: neither square being white (0,0), one white with one colored (0,1) or (1,0), and both white (1,1). Here is a video of the toggling squares:

![Two toggling squares][http://img.youtube.com/vi/GtVpXq7ru7g/0.jpg](https://www.youtube.com/watch?v=GtVpXq7ru7g)

We additionally wanted to have four different squares change colors, for the clear distinction of the four different states. We changed the square A1 (top left) to turn white on (0,0), B1 white on (0,1), C1 white on (1,0), and D1 (bottom left) on (1,1). 

```
4'd0 : 												// row A
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = (~switch_1 && ~switch_2) ? 8'b111_111_11: 8'b111_000_00;

...

4'd1 : 												// row B
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = (~switch_1 && switch_2) ? 8'b111_111_11: 8'b111_111_00;					

...

4'd2 : 												// row C
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = (switch_1 && ~switch_2) ? 8'b111_111_11: 8'b111_111_00;

...

4'd3 : 												// row D
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = (switch_1 && switch_2) ? 8'b111_111_11: 8'b111_000_11;
```					

![Four toggling squares](http://img.youtube.com/vi/NvecpIrvSZ8/0.jpg)](https://www.youtube.com/watch?v=NvecpIrvSZ8)

Debugging: 

We had a few issues with using the correct pins on the FPGA. We first didn't distinguish between GPIO_0 and GPIO_1, and then did not know the correct orientation of the pinout (i.e. where pin 1 was), then could not interface with the pin itself. 


From the oscilloscope check, we were confident the signal was toggling as desired at 3.3V. Then, we tried debugging using the LEDs on the FPGA. After a series of seeing the LEDs constantly high for some arbitrary reason, we realized we were not, in fact, reading from GPIO_1 pins 15 and 17, but rather 5 and 7--the declaration of the "_1" in "GPIO_15" was confusing.

After this, we switched to outputting on the screen. Our logic had been correct to change boxes on-screen.


### Audio
#### Connecting the FPGA output to the the speaker
#### Outputting a square wave to the speaker
#### Outputting three tones via DAC



[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
