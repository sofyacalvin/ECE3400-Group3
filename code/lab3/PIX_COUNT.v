module PIX_COUNT (
	X,
	Y,
	PIX_NUM
);

/******
* I/O *
******/

input [9:0] X; //HORIZONTAL POSITION OF THE PIXEL;
input [9:0] Y; //VERTICLE POSITION OF THE PIXEL;
output [13:0] PIX_NUM; 

/****************************
* MEMORY AND INTERNAL WIRES *
*****************************/
reg [2:0] row;
reg [2:0] col;
reg [13:0] pix_num_count;

/************************
* LOGIC AND CONNECTIONS *
************************/
assign PIX_NUM = pix_num_count;

always @ (*) begin
	row = Y % 120;
	col = X % 120;
	pix_num_count = ((Y-1)-row*120)*120 + ((X-1)-col*120);
end

endmodule
