module RADIO_READ(
	RESET,
	CLOCK,
	DATA_IN,
	DATA_OUT
);

/******
* I/O *
******/

input CLOCK; 
input RESET;
input  [7:0] DATA_IN; 


output [7:0] DATA_OUT; 

/****************************
* MEMORY AND INTERNAL WIRES *
*****************************/

reg  [9:0] pixel_count;
reg  [9:0] line_count;

/************************
* LOGIC AND CONNECTIONS *
************************/
assign PIXEL_X    = pixel_count;
assign PIXEL_Y    = line_count;

assign PIXEL_COLOR_OUT = (pixel_count<(`VISIBLE_SCREEN_WIDTH) /*&& (line_count<`VISIBLE_SCREEN_HEIGHT)*/)
									? (PIXEL_COLOR_IN) : (8'b00000000) ; //ROUTE THE INPUT COLOR TO OUTPUT IF WITHIN VISIBLE BOUNDS
									
assign H_SYNC_NEG = (pixel_count>=656 && pixel_count<752) ? (1'b0) : (1'b1); //ACTIVATE THE H SYNC PULSE AFTER FRONT PORCH

assign V_SYNC_NEG = (line_count>=490 && line_count<492) ? (1'b0) : (1'b1); //ACTIVATE THE V SYNC PULSE AFTER FRONT PORCH

always @(posedge CLOCK) begin
	if (RESET) begin
		pixel_count <= 10'b0;
		line_count <= 10'b0;
	end
	else if (pixel_count == (`TOTAL_SCREEN_WIDTH-1)) begin
		pixel_count <= 0;
		if (line_count == (`TOTAL_SCREEN_HEIGHT-1)) begin
		line_count <= 0;
		end 
		else begin
		line_count <= line_count + 1;
		end
	end 
	else begin
		pixel_count <= pixel_count + 1;
		line_count <= line_count;
	end
end

endmodule
