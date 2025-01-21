// 64-bit ALU test bench template
module alu64bit_test;


    logic [63:0] A;
    logic [63:0] B;    
    logic CIN;    
    logic [1:0] OP;    
    logic [63:0] S;   
    logic COUT;

alu64bit uut(
	.a(A),
	.b(B),
	.cin(CIN),
	.op(OP),
	.s(S),
	.cout(COUT)
	
);

 initial begin
    A = {64{'0}};
    B = {64{'1}};
	CIN = 0;
	OP[0] = 0;
	OP[1] = 1;
	
    #5000

	A[0] = 1;
    B = {64{'1}};
	CIN = 0;
	OP[0] = 0;
	OP[1] = 1;
	
    #5000
      
    ;end
    



endmodule
