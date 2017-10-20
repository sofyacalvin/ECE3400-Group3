module RADIO_READ(
	RESET,
	CLOCK,
	DATA_IN,
	RADIO_X,
	RADIO_Y,
	VALUE
);

/******
* I/O *
******/

input CLOCK; 
input RESET;
input  [6:0] DATA_IN; 

output [2:0] RADIO_X;
output [1:0] RADIO_Y;
output [1:0] VALUE; 


reg [2:0] X;
reg [1:0] Y;
reg [1:0] val;

assign RADIO_X = X;
assign RADIO_Y = Y;
assign VALUE = val;

always @ (DATA_IN) begin
	X <= DATA_IN[6:4];
	Y <= DATA_IN[3:2];
	val <= DATA_IN[1:0];
end

endmodule
