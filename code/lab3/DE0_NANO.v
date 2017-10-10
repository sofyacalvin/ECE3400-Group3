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
	 
	 always @ (*) begin
	 
		case(PIXEL_COORD_Y / 120)
			4'd0 : 												// row A
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = 8'b111_000_00;
					4'd1 : PIXEL_COLOR = 8'b111_001_00;
					4'd2 : PIXEL_COLOR = 8'b111_010_00;
					4'd3 : PIXEL_COLOR = 8'b111_100_00;
					4'd4 : PIXEL_COLOR = 8'b111_110_00;
					4'd5 : PIXEL_COLOR = 8'b111_111_10;
					default: PIXEL_COLOR = 8'b111_111_11;
					endcase
			4'd1 : 												// row B
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = 8'b000_000_00;
					4'd1 : PIXEL_COLOR = 8'b000_001_00;
					4'd2 : PIXEL_COLOR = 8'b000_010_00;
					4'd3 : PIXEL_COLOR = 8'b000_100_00;
					4'd4 : PIXEL_COLOR = 8'b000_110_00;
					4'd5 : PIXEL_COLOR = 8'b000_111_10;
					default: PIXEL_COLOR = 8'b0;
					endcase
			4'd2 : 												// row C
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = 8'b111_111_00;
					4'd1 : PIXEL_COLOR = 8'b110_111_00;
					4'd2 : PIXEL_COLOR = 8'b101_111_00;
					4'd3 : PIXEL_COLOR = 8'b100_111_00;
					4'd4 : PIXEL_COLOR = 8'b011_111_00;
					4'd5 : PIXEL_COLOR = 8'b001_111_10;
					default: PIXEL_COLOR = 8'b0;
					endcase
			4'd3 : 												// row D
				case(PIXEL_COORD_X / 120)
					4'd0 : PIXEL_COLOR = 8'b111_000_11;
					4'd1 : PIXEL_COLOR = 8'b111_001_11;
					4'd2 : PIXEL_COLOR = 8'b111_010_11;
					4'd3 : PIXEL_COLOR = 8'b111_100_11;
					4'd4 : PIXEL_COLOR = 8'b111_110_11;
					4'd5 : PIXEL_COLOR = 8'b111_111_01;
					default: PIXEL_COLOR = 8'b0;
					endcase
			default: PIXEL_COLOR = 8'b000_111_00;
			endcase
	 end
	 
//	 always @ (*) begin
//
//		if (PIXEL_COORD_X < 20 || PIXEL_COORD_X > 620) begin				// empty space on sides black
//			PIXEL_COLOR = 8'b000_000_00;
//		end
//		
//		else if (PIXEL_COORD_X > 20 && PIXEL_COORD_X < 30) begin
//					PIXEL_COLOR = 8'b111_111_11;											// white outer walls (left+right)
//		end
//		else if (PIXEL_COORD_X > 610 && PIXEL_COORD_X < 620)  begin
//			PIXEL_COLOR = 8'b111_111_11;											// white outer walls (left+right)
//		end
//		else if ( PIXEL_COORD_Y < 10) begin
//			PIXEL_COLOR = 8'b111_111_11;
//		end
//		else if (PIXEL_COORD_Y > 470 ) begin
//			PIXEL_COLOR = 8'b111_111_11;											// white outer walls (top+bottom)
//		end
//		
//		
////		if (PIXEL_COORD_Y > 10 && PIXEL_COORD_Y < 110 ) begin				// ----1st row----
////			if (PIXEL_COORD_X >= 30 && PIXEL_COORD_X <= 130) begin			// 1st col (A1)
////				PIXEL_COLOR = 8'b111_000_00; 												// red
////			end
////				else if (PIXEL_COORD_X > 130 && PIXEL_COORD_X < 150) begin		// -wall- (A1/A2)
////					PIXEL_COLOR = 8'b000_111_00; 												//gray
////				end
////			else if (PIXEL_COORD_X >= 150 && PIXEL_COORD_X <= 250) begin		// 2nd col (A2)
////				PIXEL_COLOR = 8'b000_111_00; 												// green
////			end
////				else if (PIXEL_COORD_X > 250 && PIXEL_COORD_X < 270) begin		// -wall- (A2/A3)
////					PIXEL_COLOR = 8'b000_111_00; 												//gray
////				end
////			else if (PIXEL_COORD_X >= 270 && PIXEL_COORD_X <= 370) begin		// 3rd col (A3)
////				PIXEL_COLOR = 8'b000_111_00; 												// green
////			end
////				else if (PIXEL_COORD_X > 370 && PIXEL_COORD_X < 390) begin		// -wall- (A3/A4)
////					PIXEL_COLOR = 8'b000_111_00; 												//gray
////				end
////			else if (PIXEL_COORD_X >= 390 && PIXEL_COORD_X <= 490) begin		// 4th col (A4)
////				PIXEL_COLOR = 8'b000_111_00; 												// green
////			end
////				else if (PIXEL_COORD_X > 490 && PIXEL_COORD_X < 510) begin		// -wall- (A4/A5)
////					PIXEL_COLOR = 8'b000_111_00; 												//gray
////				end
////			else if (PIXEL_COORD_X >= 510 && PIXEL_COORD_X <= 610) begin		// 5th col (A5)
////				PIXEL_COLOR = 8'b000_111_00; 												// green
////			end
////			else begin																		// else
////				PIXEL_COLOR = 8'b111_111_00; 												// yellow
////			end
////		end
////		else if (PIXEL_COORD_X >= 240 && PIXEL_COORD_X < 480)	begin		// ---2nd row---
////			if (PIXEL_COORD_Y < 240) begin											// 1st col
////				PIXEL_COLOR = 8'b111_000_11; 												// blue
////			end
////			else if (PIXEL_COORD_Y >= 240 && PIXEL_COORD_Y < 480) begin		// 2nd col
////				PIXEL_COLOR = 8'b111_111_11; 												// white
////			end
////			else begin																		// else
////				PIXEL_COLOR = 8'b000_111_11; 												// purple
////			end
////		end
//		else begin																		// ---else---
//				PIXEL_COLOR = 8'b000_000_00;											// black
//		end
//	 end
	 
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
