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
input  [7:0] DATA_IN; 

output [2:0] RADIO_X;
output [1:0] RADIO_Y;
output [2:0] VALUE; 


reg [2:0] X;
reg [1:0] Y;
reg [2:0] val;

assign RADIO_X = X;
assign RADIO_Y = Y;
assign VALUE = val;

always @ (DATA_IN) begin
	X <= DATA_IN[7:5];
	Y <= DATA_IN[4:3];
	val <= DATA_IN[2:0];
end

endmodule
