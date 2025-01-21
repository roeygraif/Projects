// Full Adder/Subtractor test bench template
module fas_test;


logic A;
logic B;
logic CIN;
logic ANS;
logic S;
logic COUT;


fas uut(
	.a(A),
	.b(B),
	.cin(CIN),
	.a_ns(ANS),
	.s(S),
	.cout(COUT)
	
);

 initial begin
    A = 0;
    B = 0;
	CIN = 0;
	ANS = 0;    
    #100
     
	A = 0;
    B = 1;
	CIN = 0;
	ANS = 0;    
    #100
	
	
    ;end
    



endmodule
