//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template top-level module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

`define ONE_SEC 25000000

module DE0_NANO(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// LED //////////
	LED,

	//////////// KEY //////////
	KEY,

	//////////// SW //////////
	SW,

	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	GPIO_0_D,
	GPIO_0_IN,

	//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	GPIO_1_D,
	GPIO_1_IN,
);

	 //=======================================================
	 //  PARAMETER declarations
	 //=======================================================

	 localparam ONE_SEC = 25000000; // one second in 25MHz clock cycles
	 
   	
	parameter white = 8'b111_111_11;
	parameter gray = 8'b100_100_10;
	parameter black = 8'b000_000_00;
	parameter red = 8'b111_000_00;
	parameter orange = 8'b111_111_00;
	parameter yellow = 8'b100_111_00;
	parameter green = 8'b000_111_00;
	parameter blue = 8'b000_000_11;
	parameter purple = 8'b011_000_11;
	parameter wall = 8'b010_011_11;
	
   
	 //=======================================================
	 //  PORT declarations
	 //=======================================================

	 //////////// CLOCK //////////
	 input 		          		CLOCK_50;

	 //////////// LED //////////
	 output		     [7:0]		LED;

	 /////////// KEY //////////
	 input 		     [1:0]		KEY;

	 //////////// SW //////////
	 input 		     [3:0]		SW;

	 //////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_0_D;
	 input 		     [1:0]		GPIO_0_IN;

	 //////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_1_D;
	 input 		     [1:0]		GPIO_1_IN;
	 
	 //input	switch_1_in; 

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
    reg         CLOCK_25;
    wire        reset; // active high reset signal 

    wire [9:0]  PIX_X; // current x-coord from VGA driver
    wire [9:0]  PIX_Y; // current y-coord from VGA driver
    reg [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 
	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off
	 
    reg [2:0] maze_state[0:4][0:3];
	
	 //reg [1:0] current_row;
	 //reg [2:0] current_col;
	 
	 wire [2:0] radio_x;
	 wire [1:0] radio_y;
	 wire [2:0] radio_value;
	 
	 // switch setup
	 //wire switch_1;
	 //wire switch_2;
	 
	 //assign switch_1 = GPIO_1_D[4]; //arduino 12
	 //assign switch_2 = GPIO_1_D[7]; //arduino 13
	 
	 // current highlighted square
	 wire highlighted_x;
	 wire highlighted_y;	 
	 
	 //Switch input through GPIO pins
	 assign highlighted_x = GPIO_0_D[33];
	 assign highlighted_y = GPIO_0_D[31];

  //loop variables
	reg [2:0] i; 
	reg [2:0] j;
	

	 
	 

	 
    // Module outputs coordinates of next pixel to be written onto screen
    VGA_DRIVER driver(
		  .RESET(reset),
        .CLOCK(CLOCK_25),
        .PIXEL_COLOR_IN(PIXEL_COLOR),
        .PIXEL_X(PIX_X),
        .PIXEL_Y(PIX_Y),
        .PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
        .H_SYNC_NEG(GPIO_0_D[7]),
        .V_SYNC_NEG(GPIO_0_D[5])
    );
	 
	 RADIO_READ driver2(
			.RESET(reset),
			.CLOCK(CLOCK_25),
			.DATA_IN({GPIO_1_D[9],GPIO_1_D[11],GPIO_1_D[13],GPIO_1_D[15],GPIO_1_D[17],GPIO_1_D[19],GPIO_1_D[21],GPIO_1_D[23]}),
			.RADIO_X(radio_x),
			.RADIO_Y(radio_y),
			.VALUE(radio_value)
	);
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	
	 assign LED[0] = led_state;
	 assign LED[1] = switch_1;
	 assign LED[2] = switch_2;
	 
	 
	 
	 initial begin
		for(i = 0; i < 5; i = i+1) begin
			for (j = 0; j < 4; j = j+1) begin
				maze_state[i][j] = 3'd0;
			end
		end
	 end
	 	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
//	always @ (value) begin
//		maze_state[radio_x][radio_y] <= value;
//	end
	 
	 
	 
	 always @ (posedge CLOCK_50) begin

			maze_state[radio_x][radio_y] <= radio_value;
			//current_row <= 2'd3;
			//current_col <= 3'd2;
			//if (switch_1) maze_state[current_row][current_col] <= 2'd2;
			//else maze_state[current_row][current_col] = 2'd0;
			
			
	 		case(PIX_Y / 120)
			4'd0 : 												// row A
				case(PIX_X / 120)
					4'd0 : begin
                case(maze_state[PIX_X/120][PIX_Y/120])
                  3'd0: PIXEL_COLOR <= red;
                  3'd1: PIXEL_COLOR <=  orange;
                  3'd2: PIXEL_COLOR <=  yellow;
                  3'd3: PIXEL_COLOR <=  green;
                  3'd4: PIXEL_COLOR <=  blue;
                  3'd5: PIXEL_COLOR <=  purple;
                  3'd6: PIXEL_COLOR <=  white;
                  3'd7: PIXEL_COLOR <=  wall;
                endcase
							 end
					4'd1 : begin
                case(maze_state[PIX_X/120][PIX_Y/120])
                  3'd0: PIXEL_COLOR <= red;
                  3'd1: PIXEL_COLOR <=  orange;
                  3'd2: PIXEL_COLOR <=  yellow;
                  3'd3: PIXEL_COLOR <=  green;
                  3'd4: PIXEL_COLOR <=  blue;
                  3'd5: PIXEL_COLOR <=  purple;
                  3'd6: PIXEL_COLOR <=  white;
                  3'd7: PIXEL_COLOR <=  wall;
                endcase
							 end
					4'd2 : PIXEL_COLOR <= black;
					4'd3 : PIXEL_COLOR <= black;
					4'd4 : PIXEL_COLOR <= black;
					4'd5 : PIXEL_COLOR <= black;
					default: PIXEL_COLOR <= red;
					endcase
			4'd1 : 												// row B
				case(PIX_X / 120)
					4'd0 : begin
                case(maze_state[PIX_X/120][PIX_Y/120])
                  3'd0: PIXEL_COLOR <= red;
                  3'd1: PIXEL_COLOR <=  orange;
                  3'd2: PIXEL_COLOR <=  yellow;
                  3'd3: PIXEL_COLOR <=  green;
                  3'd4: PIXEL_COLOR <=  blue;
                  3'd5: PIXEL_COLOR <=  purple;
                  3'd6: PIXEL_COLOR <=  white;
                  3'd7: PIXEL_COLOR <=  wall;
                endcase
							 end
					4'd1 : begin
                case(maze_state[PIX_X/120][PIX_Y/120])
                  3'd0: PIXEL_COLOR <= red;
                  3'd1: PIXEL_COLOR <=  orange;
                  3'd2: PIXEL_COLOR <=  yellow;
                  3'd3: PIXEL_COLOR <=  green;
                  3'd4: PIXEL_COLOR <=  blue;
                  3'd5: PIXEL_COLOR <=  purple;
                  3'd6: PIXEL_COLOR <=  white;
                  3'd7: PIXEL_COLOR <=  wall;
                endcase
							 end
					4'd2 : PIXEL_COLOR <= black;
					4'd3 : PIXEL_COLOR <= black;
					4'd4 : PIXEL_COLOR <= black;
					4'd5 : PIXEL_COLOR <= black;
					default: PIXEL_COLOR <= red;
					endcase
			4'd2 : 												// row C
				case(PIX_X / 120)
					4'd0 : PIXEL_COLOR <= black;
					4'd1 : PIXEL_COLOR <= black;
					4'd2 : PIXEL_COLOR <= black;
					4'd3 : PIXEL_COLOR <= black;
					4'd4 : PIXEL_COLOR <= black;
					4'd5 : PIXEL_COLOR <= black;
					default: PIXEL_COLOR <= red;
					endcase
			4'd3 : 												// row D
				case(PIX_X / 120)
					4'd0 : PIXEL_COLOR <= black;
					4'd1 : PIXEL_COLOR <= black;
					4'd2 : PIXEL_COLOR <= black;
					4'd3 : PIXEL_COLOR <= black;
					4'd4 : PIXEL_COLOR <= black;
					4'd5 : PIXEL_COLOR <= black;
					default: PIXEL_COLOR <= red;
					endcase
			default: PIXEL_COLOR <= green;
			endcase
	 end
	 
	 
	 // Generate 25MHz clock for VGA, FPGA has 50 MHz clock
    always @ (posedge CLOCK_50) begin
        CLOCK_25 <= ~CLOCK_25; 
    end // always @ (posedge CLOCK_50)

	
	 // Simple state machine to toggle LED0 every one second
	 always @ (posedge CLOCK_25) begin
		  if (reset) begin
				led_state   <= 1'b0;
				led_counter <= 25'b0;
		  end
		  
		  if (led_counter == ONE_SEC) begin
				led_state   <= ~led_state;
				led_counter <= 25'b0;
		  end
		  else begin	
				led_state   <= led_state;
				led_counter <= led_counter + 25'b1;
		  end // always @ (posedge CLOCK_25)
	 end
	 

endmodule
