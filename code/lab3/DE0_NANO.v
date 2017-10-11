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

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
    reg         CLOCK_25;
    wire        reset; // active high reset signal 

    wire [9:0]  PIXEL_COORD_X; // current x-coord from VGA driver
    wire [9:0]  PIXEL_COORD_Y; // current y-coord from VGA driver
    reg [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 
	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off

	 // current highlighted square
	 wire highlighted_x;
	 wire highlighted_y;	 
	 //Switch input through GPIO pins
	 assign highlighted_x = GPIO_0_D[33];
	 assign highlighted_y = GPIO_0_D[31];
	 
   // array to store state of maze blocks
   //   (2 bit entries, 3 bit row numbers, 4 bit col numbers)
   reg [1:0] maze_state [2:0][3:0];
	
	parameter bricky = 13'd14400;
	reg [7:0] COLOR_DATA [0:bricky-1];
   
   
   
   
   
   
   
    //=======================================================
    //  ALWAYS block for PIXEL_COLOR
    //=======================================================
   //always @ (*) begin
		//case(PIXEL_COORD_Y / 120)
			//4'd0 : 												// row A
				//case(PIXEL_COORD_X / 120)
					//4'd0 : PIXEL_COLOR = 8'b111_000_00;
					//4'd1 : PIXEL_COLOR = 8'b111_001_00;
					//4'd2 : PIXEL_COLOR = 8'b111_010_00;
					//4'd3 : PIXEL_COLOR = 8'b111_100_00;
					//4'd4 : PIXEL_COLOR = 8'b111_110_00;
					//4'd5 : PIXEL_COLOR = 8'b111_111_10;
					//default: PIXEL_COLOR = 8'b111_111_11;
					//endcase
			//4'd1 : 												// row B
				//case(PIXEL_COORD_X / 120)
					//4'd0 : PIXEL_COLOR = 8'b000_000_00;
					//4'd1 : PIXEL_COLOR = 8'b000_001_00;
					//4'd2 : PIXEL_COLOR = 8'b000_010_00;
					//4'd3 : PIXEL_COLOR = 8'b000_100_00;
					//4'd4 : PIXEL_COLOR = 8'b000_110_00;
					//4'd5 : PIXEL_COLOR = 8'b000_111_10;
					//default: PIXEL_COLOR = 8'b0;
					//endcase
			//4'd2 : 												// row C
				//case(PIXEL_COORD_X / 120)
					//4'd0 : PIXEL_COLOR = 8'b111_111_00;
					//4'd1 : PIXEL_COLOR = 8'b110_111_00;
					//4'd2 : PIXEL_COLOR = 8'b101_111_00;
					//4'd3 : PIXEL_COLOR = 8'b100_111_00;
					//4'd4 : PIXEL_COLOR = 8'b011_111_00;
					//4'd5 : PIXEL_COLOR = 8'b001_111_10;
					//default: PIXEL_COLOR = 8'b0;
					//endcase
			//4'd3 : 												// row D
				//case(PIXEL_COORD_X / 120)
					//4'd0 : PIXEL_COLOR = 8'b111_000_11;
					//4'd1 : PIXEL_COLOR = 8'b111_001_11;
					//4'd2 : PIXEL_COLOR = 8'b111_010_11;
					//4'd3 : PIXEL_COLOR = 8'b111_100_11;
					//4'd4 : PIXEL_COLOR = 8'b111_110_11;
					//4'd5 : PIXEL_COLOR = 8'b111_111_01;
					//default: PIXEL_COLOR = 8'b0;
					//endcase
			//default: PIXEL_COLOR = 8'b000_111_00;
			//endcase
	 //end
	initial
	$readmemh ("brick.list", COLOR_DATA);
 
      always @ (*) begin
		case(PIXEL_COORD_Y / 120)
			4'd0 : 												// row A
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = COLOR_DATA[{PIXEL_COORD_Y*(PIXEL_COORD_X-1)+PIXEL_COORD_X}];
					//4'd0 : PIXEL_COLOR = COLOR_DATA[{PIXEL_COORD_X}];
					4'd1 : PIXEL_COLOR = 8'b111_001_00;
					4'd2 : PIXEL_COLOR = 8'b111_010_00;
					4'd3 : PIXEL_COLOR = 8'b111_100_00;
					//4'd4 : PIXEL_COLOR = 8'b111_110_00;
					//4'd5 : PIXEL_COLOR = 8'b111_111_10;
					default: PIXEL_COLOR = 8'b111_111_11;
					endcase
			4'd1 : 												// row B
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = 8'b000_000_00;
					4'd1 : PIXEL_COLOR = 8'b000_001_00;
					4'd2 : PIXEL_COLOR = 8'b000_010_00;
					4'd3 : PIXEL_COLOR = 8'b000_100_00;
					//4'd4 : PIXEL_COLOR = 8'b000_110_00;
					//4'd5 : PIXEL_COLOR = 8'b000_111_10;
					default: PIXEL_COLOR = 8'b0;
					endcase
			4'd2 : 												// row C
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = 8'b111_111_00;
					4'd1 : PIXEL_COLOR = 8'b110_111_00;
					4'd2 : PIXEL_COLOR = 8'b101_111_00;
					4'd3 : PIXEL_COLOR = 8'b100_111_00;
					//4'd4 : PIXEL_COLOR = 8'b011_111_00;
					//4'd5 : PIXEL_COLOR = 8'b001_111_10;
					default: PIXEL_COLOR = 8'b0;
					endcase
			4'd3 : 												// row D
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = 8'b111_000_11;
					4'd1 : PIXEL_COLOR = 8'b111_001_11;
					4'd2 : PIXEL_COLOR = 8'b111_010_11;
					4'd3 : PIXEL_COLOR = 8'b111_100_11;
					//4'd4 : PIXEL_COLOR = 8'b111_110_11;
					//4'd5 : PIXEL_COLOR = 8'b111_111_01;
					default: PIXEL_COLOR = 8'b0;
					endcase
			default: PIXEL_COLOR = 8'b000_111_00;
			endcase
	 end

	 
    // Module outputs coordinates of next pixel to be written onto screen
    VGA_DRIVER driver(
		  .RESET(reset),
        .CLOCK(CLOCK_25),
        .PIXEL_COLOR_IN(PIXEL_COLOR),
        .PIXEL_X(PIXEL_COORD_X),
        .PIXEL_Y(PIXEL_COORD_Y),
        .PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
        .H_SYNC_NEG(GPIO_0_D[7]),
        .V_SYNC_NEG(GPIO_0_D[5])
    );
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	 //assign PIXEL_COLOR = (PIXEL_COORD_X > 50 && PIXEL_COORD_X < 150 && PIXEL_COORD_Y > 50 && PIXEL_COORD_Y < 150) ? 8'b000_111_000 : 8'b000_000_000;
 	 //assign PIXEL_COLOR = 8'b111_000_00; // Red
	 assign LED[0] = led_state;
	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
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

