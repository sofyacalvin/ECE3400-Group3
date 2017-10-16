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
	 
    reg[7:0]    address;


	 //for multifrequency output
	 reg[7:0]    address1;
	 reg[7:0]    address2;
	 reg[7:0]    address3;
	 reg[7:0]    address4;
	 reg[7:0]    address5;
	 reg[7:0]    address6;

	 //for multifrequency output
	 wire[7:0]    out1;
	 wire[7:0]    out2;
	 wire[7:0]    out3;
	 wire[7:0]    out4;
	 wire[7:0]    out5;
	 wire[7:0]    out6;

	 //for multifrequency output
	 reg[7:0]   final;

	 //for multifrequency output
	 //assign {GPIO_1_D[8],GPIO_1_D[10],GPIO_1_D[12],GPIO_1_D[14],GPIO_1_D[16],GPIO_1_D[18],GPIO_1_D[20],GPIO_1_D[22]} = final;
	

    wire [9:0]  PIXEL_COORD_X; // current x-coord from VGA driver
    wire [9:0]  PIXEL_COORD_Y; // current y-coord from VGA driver
    wire [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 
	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off
	 
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
	
	 SINE_ROM sine (
			.addr(address),
			.clk(CLOCK_25),
			.q({GPIO_1_D[8],GPIO_1_D[10],GPIO_1_D[12],GPIO_1_D[14],GPIO_1_D[16],GPIO_1_D[18],GPIO_1_D[20],GPIO_1_D[22]})
	 
	 );
	
	 //multifrequency output
	 /*SINE_ROM sine1 (
			.addr(address1),
			.clk(CLOCK_25),
			.q(out1)
	 );
	 
	  SINE_ROM sine2 (
			.addr(address2),
			.clk(CLOCK_25),
			.q(out2)
	 );
	 
	  SINE_ROM sine3 (
			.addr(address3),
			.clk(CLOCK_25),
			.q(out3)
	 );
	 
	 SINE_ROM sine4 (
			.addr(address4),
			.clk(CLOCK_25),
			.q(out4)
	 
	 );
	 
	  SINE_ROM sine5 (
			.addr(address5),
			.clk(CLOCK_25),
			.q(out5)
	 
	 );
	 
	  SINE_ROM sine6 (
			.addr(address6),
			.clk(CLOCK_25),
			.q(out6)
	 
	 );*/
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	 assign PIXEL_COLOR = 8'b000_111_00; // Green
	 assign LED[0] = led_state;
	 
	// Local parameter
	//660hz sine wave
	localparam CLKDIVIDER_660 = 25000000/660/256;
	
	//notes in Dm11 chord
	localparam CLKDIVIDER_D = 25000000/294/256;
	localparam CLKDIVIDER_F = 25000000/349/256;
	localparam CLKDIVIDER_A = 25000000/440/256;
	localparam CLKDIVIDER_C = 25000000/523/256;
	localparam CLKDIVIDER_E = 25000000/660/256;
	localparam CLKDIVIDER_G = 25000000/784/256;
	
	//440hz square wave
	localparam CLKDIVIDER_440 = 25000000/440/2;

	// Sound variables
   //reg square_440;                       // 440 Hz square wave
   //assign GPIO_0_D[2] = square_440;
	
	//counter for the 6 different notes for multifrequency output
	reg [15:0] counter1;
	reg [15:0] counter2;
	reg [15:0] counter3;
	reg [15:0] counter4;
	reg [15:0] counter5;
	reg [15:0] counter6;
	
	//temp counter for switching between 3 different frequencies
	reg [15:0] count;
	
	
	reg [15:0] counter;
	
	reg [24:0] duration;
	reg [1:0] note;
	
	

	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
	 // Generate 25MHz clock for VGA, FPGA has 50 MHz clock
    always @ (posedge CLOCK_50) begin
        CLOCK_25 <= ~CLOCK_25; 
		  
    end // always @ (posedge CLOCK_50)

	 /* 660 hz sine wave */	 
	 /*always @ (posedge CLOCK_25) begin
 		if (counter == 0) begin
 			counter <= CLKDIVIDER_660 - 1;
 			
 			if (address == 255) begin
 			address <= 0;
 			
 			end
 			else begin
 				address <= address + 1;
 			end
 		end
 		else begin
 			counter <= counter - 1;
 		end
 	 end*/
	 
	/* 3 distinct tones played for 1 sec at a time*/ 
	always @ (posedge CLOCK_25) begin
		if (duration == 0) begin
			duration <= ONE_SEC;
			if (note == 0) begin
				count <= CLKDIVIDER_C - 16'b1;
				note <= 2;
			end
			else if (note == 1) begin
				count <= CLKDIVIDER_G - 16'b1;
				note <= note - 2'b1;
			end
			else if (note == 2) begin
				count <= CLKDIVIDER_E - 16'b1;
				note <= note - 2'b1;
			end
		end
		else begin
			if (counter == 0) begin
				counter <= count - 16'b1;
				if (address == 255) begin
				address <= 8'b0;
				end
				else begin
					address <= address + 8'b1;
				end
			end
			else begin
				counter <= counter - 16'b1;
			end
			duration <= duration - 25'b1;
		end
		
	 end
/**********************************************TEST MULTIFREQUENCY OUTPUT******************************************/	 
	 
	 /*always @ (posedge CLOCK_25) begin
		if (counter == 0) begin
				counter <= CLKDIVIDER_G - 16'b1;
				final <= ((out1 + out2 + out3 )/3);
		end
		else begin
				counter <= counter - 16'b1;
		end
		
	 end
	 always @ (posedge CLOCK_25) begin
		if (counter1 == 0) begin
			counter1 <= CLKDIVIDER_E - 1;
			
			if (address1 == 255) begin
			address1 <= 0;
			
			end
			else begin
				address1 <= address1 + 1;
			end
		end
		else begin
			counter1 <= counter1 - 1;
		end
		
	 end
	 
	 always @ (posedge CLOCK_25) begin
		if (counter2 == 0) begin
			counter2 <= CLKDIVIDER_C - 1;
			
			if (address2 == 255) begin
			address2 <= 0;
			
			end
			else begin
				address2 <= address2 + 1;
			end
		end
		else begin
			counter2 <= counter2 - 1;
		end
		
	 end
	 
	 always @ (posedge CLOCK_25) begin
		if (counter3 == 0) begin
			counter3 <= CLKDIVIDER_G - 1;
			
			if (address3 == 255) begin
			address3 <= 0;
			
			end
			else begin
				address3 <= address3 + 1;
			end
		end
		else begin
			counter3 <= counter3 - 1;
		end
		
	 end
	 
	/* always @ (posedge CLOCK_25) begin
		if (counter4 == 0) begin
			counter4 <= CLKDIVIDER_D - 1;
			
			if (address4 == 255) begin
			address4 <= 0;
			
			end
			else begin
				address4 <= address4 + 1;
			end
		end
		else begin
			counter4 <= counter4 - 1;
		end
		
	 end
	 
	 always @ (posedge CLOCK_25) begin
		if (counter5 == 0) begin
			counter5 <= CLKDIVIDER_F - 1;
			
			if (address5 == 255) begin
			address5 <= 0;
			
			end
			else begin
				address5 <= address5 + 1;
			end
		end
		else begin
			counter5 <= counter5 - 1;
		end
		
	 end
	 
	 always @ (posedge CLOCK_25) begin
		if (counter6 == 0) begin
			counter6 <= CLKDIVIDER_A - 1;
			
			if (address1 == 255) begin
			address6 <= 0;
			
			end
			else begin
				address6 <= address6 + 1;
			end
		end
		else begin
			counter6 <= counter6 - 1;
		end
		
	 end*/
/***********************************************************************************************************************/	 

	
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

	 


		
	 
// //Sound state machine (440hz square wave)
//always @ (posedge CLOCK_25) begin
//  if (counter == 0) begin
//    counter    <= CLKDIVIDER_440 - 1; // reset clock
//    square_440 <= ~square_440;        // toggle the square pulse
//  end
//  else begin
//    counter    <= counter - 1;
//    square_440 <= square_440;
//  end
//end	
endmodule
