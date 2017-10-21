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
	parameter orange = 8'b111_011_00;
	parameter yellow = 8'b100_111_00;
	parameter green = 8'b000_111_00;
	parameter blue = 8'b000_000_11;
	parameter purple = 8'b011_000_11;
	parameter wall = 8'b111_100_10;
	
   parameter weird_purple = 8'b010_000_01;
	parameter dark_lavender = 8'b010_010_10;
	parameter maroon = 8'b011_001_00;
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
	 
    reg [1:0] maze_state[0:4][0:3];
	
	 //reg [1:0] current_row;
	 //reg [2:0] current_col;
	 
	 wire [2:0] radio_x;
	 wire [1:0] radio_y;
	 wire [1:0] radio_value;
	 
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
	
	reg [2:0] grid_x;
	reg [1:0] grid_y;
	reg [5:0] grid_counter; 
	reg [1:0] maze_vector [0:19]; 

	 
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
			.DATA_IN({GPIO_1_D[9],GPIO_1_D[11],GPIO_1_D[13],GPIO_1_D[15],GPIO_1_D[17],GPIO_1_D[19],GPIO_1_D[21]}),
			.RADIO_X(radio_x),
			.RADIO_Y(radio_y),
			.VALUE(radio_value)
	);
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	
//	 assign LED[0] = maze_state[4][0][0];
//	 assign LED[1] = maze_state[4][0][1];
//	 assign LED[2] = maze_state[4][1][0];
//	 assign LED[3] = maze_state[4][1][1];
//	 assign LED[4] = maze_state[4][2][0];
//	 assign LED[5] = maze_state[4][2][1];
//	 assign LED[6] = maze_state[4][3][0];
//	 assign LED[7] = maze_state[4][3][1];
//	 

	 assign LED[0] = radio_value[0];
	 assign LED[1] = radio_value[1];
	 assign LED[2] = radio_y[0];
	 assign LED[3] = radio_y[1];
	 assign LED[4] = radio_x[0];
	 assign LED[5] = radio_x[1];
	 assign LED[6] = radio_x[2];
	 
	 
	 initial begin
		for(i = 0; i < 5; i = i+1) begin
			for (j = 0; j < 4; j = j+1) begin
				maze_state[i][j] = 2'd0;
			end
		end
	 end
	 	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
//	always @ (value) begin
//		maze_state[radio_x][radio_y] <= value;
//	end
	 
	 
	 
	 always @ (posedge CLOCK_25) begin
	 grid_y <= PIX_Y/120;
	 grid_x <= PIX_X/120;
	 grid_counter <= radio_x + 5*radio_y;
	 
	 for(i = 0; i < 5; i = i+1) begin
			for (j = 0; j < 4; j = j+1) begin
				if ((i+5*j) <= grid_counter) maze_state[i][j] <= radio_value;
				else begin
					if (radio_value == 0) begin
						maze_state[i][j] <= 3;
					end
					else begin
						maze_state[i][j] <= radio_value - 1;
					end
				end
				
			end
		end
	 
	 
	 

//		for(i = 0; i < 5; i = i+1) begin
//			for (j = 0; j < 4; j = j+1) begin
//				if (i == radio_x && j == radio_y) begin
//					maze_state[i][j] <= radio_value;
//					end
//				else begin
//					if (radio_value == 0) begin
//						maze_state[i][j] <= 3;
//					end
//					else begin
//						maze_state[i][j] <= radio_value - 1;
//					end
//				end
//				if(i < radio_x && j <= radio_y)begin
//					maze_state[i][j] <= radio_value;
//					end
//				
//			end
//		end

		//maze_state[radio_x][radio_y] <= radio_value;

			
	 	case(grid_y)
			4'd0 : begin												// row A
				case(grid_x)
					4'd0 : begin
						case(maze_state[grid_x][grid_y])
							2'd0: PIXEL_COLOR <= red;
							2'd1: PIXEL_COLOR <=  green;
							2'd2: PIXEL_COLOR <=  blue;
							2'd3: PIXEL_COLOR <=  white;
						endcase
						end
					4'd1 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd2 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd3 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd4 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd5 : PIXEL_COLOR <= black;
					default: PIXEL_COLOR <= gray;
					endcase
					end
			4'd1 : 	begin											// row B
				case(grid_x)
					4'd0 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd1 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd2 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd3 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd4 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd5 : PIXEL_COLOR <= black;
					default: PIXEL_COLOR <= gray;
					endcase
					end
			4'd2 : 	begin											// row C
								case(grid_x)
					4'd0 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd1 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd2 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd3 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd4 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd5 : PIXEL_COLOR <= black;
					default: PIXEL_COLOR <= gray;
					endcase
					end
			4'd3 : 		begin										// row D
				case(grid_x)
					4'd0 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd1 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd2 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd3 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd4 : begin
                case(maze_state[grid_x][grid_y])
                  2'd0: PIXEL_COLOR <= red;
                  2'd1: PIXEL_COLOR <=  green;
                  2'd2: PIXEL_COLOR <=  blue;
                  2'd3: PIXEL_COLOR <=  white;
                endcase
							 end
					4'd5 : PIXEL_COLOR <= black;
					default: PIXEL_COLOR <= gray;
					endcase
					end
			default: PIXEL_COLOR <= orange;
			endcase
	 end
//	 always @ (CLOCK_50) begin
//		if (PIX_X/120 == radio_x && PIX_Y/120 == radio_y) PIXEL_COLOR = green;
//		else PIXEL_COLOR = maroon;
//		end
//		
	 
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
