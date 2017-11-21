# Milestone 4

## Objective
The objective of Milestone 4 is to prepare Brooklynn for the final competition: displaying walls and treasures as the robot explores, and displaying a *done* signal as well as playing a *done* tone when the maze has been fully mapped. Additionally, we spent a significant portion of time completing unfinished tasks and improving previous work.

## Procedure

### Backtracking algorithm

### Implementing SPI
We have reformatted our packets to send the data we will actually need to display the correct data. We are using a 16-bit packet, like so:

```
valid | done | 17kHz | 12kHz | 7kHz | north | east | south | west | orientation | orientation | x | x | x | y | y
```

We use a valid bit in order to determine which data to "throw away," such as any startup inconsistencies or the 16b'0 signal that may send at the beginning. The done bit is self-explanatory--this will be 1 when the robot has finished exploration and 0 otherwise. The following three bits describe if the robot has detected a treasure, and the respective bits describe the frequency. The next four bits describe if the robot has detected walls, and what direction the wall is in. Orientation follows the same guidelines as we used in [Milestone 3](./docs/labs/lab3.md), as do the x and y coordinates. 

SPI has been a source of some pain on this team. In [Lab 4](./docs/labs/lab4.md), we utilized parallel communication, under the assumption it would be a simpler implementation. We recognized that we would not have enough digital pins to do so, and made the switch to SPI (which uses less pins on the Arduino) for this milestone. 


-------------- CHECK SPI STUFF ------------------------------

Our SPI uses three main lines--Master Out Slave In (MOSI), Clock (SCK), and Slave Select (SS). We connected the receiving Arduino to the FPGA. When the SS pin is low, it communicates with the master (i.e. the Arduino). A new driver was written to read in the data from the radio module, check which slave to send to, send the data, and check for a successful transfer.

Debugging:
Oddly, we were having an issue with using the MISO pin, despite there only being one master and one slave. When the line was connected to the Arduino, data transmission would 

### Displaying data

Once the FPGA receives this packet of data, it parses the data back into the respective sections, e.g. valid bit, done, treasures, etc. A new driver was written to update the maze graphic. After initializing our maze with walls to zeros, we iteratively check if each square needs to be updated, and if not, to keep it the same. This is where we update the color of the square if a treasure is present:

```
if ( treasures == 3'd0 ) begin //no treasures
		if ( maze_state[maze_x][maze_y] == 3'd0 ) maze_state[maze_x][maze_y] <= 3'd1;
		else maze_state[maze_x][maze_y] <= maze_state[maze_x][maze_y];
	end
	else begin
		case(treasures)
			3'b001: maze_state[maze_x][maze_y] <= 3'd4; //7kHz
			3'b010: maze_state[maze_x][maze_y] <= 3'd5; //12kHz
			3'b100: maze_state[maze_x][maze_y] <= 3'd6; //17kHz
			default: maze_state[maze_x][maze_y] <= 3'd7;
		endcase
	end
```

Without this iteration, an inferred latch occurs. The walls are assigned based on the location of the square, the orientation of the robot, and the wall data received through SPI. Once everything is assigned, the colors are updated to reflect the respective state of the square or wall. These colors are predefined and/or are 

To test if the graphics were updating correctly based on the data, we created a MATLAB script to create packets that would simulate our manually assigned mazes. Below is a partial maze test.

youtube link to one

After working out the bugs, we added an arrow to show the current orientation. The following map corresponds to the successful run below it:

map
youtube link

We currently have an issue with the top row not displaying walls correctly. However, the maze display otherwise responds correctly to the inputs. We plan to fix this error before the final competition.

Debugging:
We had a lot of issues with displaying the data. Our primary issue was seemingly arbitrary walls and squares showing up when they had been neither explored nor called at all. The root of this issue had been the clock speeds, in which we were running our grid driver slower than SPI and VGA output. The changing signals became stable once we set the clock speeds to be the same.

### Finish tune

[Return to home](https://sofyacalvin.github.io/ece3400-group3/)
