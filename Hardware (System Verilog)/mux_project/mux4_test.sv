// 4->1 multiplexer test bench template
module mux4_test;
logic d0;
logic d1;
logic d2;
logic d3;
logic s0;
logic s1;
logic [1:0] sel;

logic z;

mux4 uut(
	.d0(d0),
	.d1(d1),
	.d2(d2),
	.d3(d3),
	.sel(sel),
	.z(z)
);

 initial begin
    d0 = 0;
    d1 = 0;
	d2 = 0;
	d3 = 0;
	sel[0] = 0;
	sel[1] = 0;
        
    #100
    d0 = 1;
    d1 = 0;
	d2 = 0;
	d3 = 0;
	sel[0] = 0;
	sel[1] = 0;
        
    #100
    d0 = 0;
    d1 = 0;
	d2 = 0;
	d3 = 0;
	sel[0] = 0;
	sel[1] = 0;   


        #100;        
    end
    

endmodule
