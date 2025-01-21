// 32X32 Multiplier test template
module mult32x32_fast_test;

    logic clk;            // Clock
    logic reset;          // Reset
    logic start;          // Start signal
    logic [31:0] a;       // Input a
    logic [31:0] b;       // Input b
    logic busy;           // Multiplier busy indication
    logic [63:0] product; // Miltiplication product


mult32x32_fast uut(
	.clk(clk),
	.reset(reset),
	.start(start),
	.a(a),
	.b(b),
	.busy(busy),
	.product(product)
	
);

 initial begin
 
 
 
    clk = 1'b1; 
	reset = 1'b1; 
	a = 32'd0;
	b = 32'd0;  
	#8
	reset = 1'b0;	
    #2
	a = 32'd315111401;
    b = 32'd318652716;
	start = 1'b1;
	#2
	start = 1'b0;
	#10
	start = 1'b1;
    a = 32'b0100000100101100;
    b = 32'b0011011111101001;
	#2
	start = 1'b0;
	



	
    end
	
	
always begin
	#1 clk = ~clk;
end
    



endmodule
