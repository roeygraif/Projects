// 4->1 multiplexer template
module mux4 (
    input logic d0,          // Data input 0
    input logic d1,          // Data input 1
    input logic d2,          // Data input 2
    input logic d3,          // Data input 3
    input logic [1:0] sel,   // Select input
    output logic z           // Output
);
logic g0,g1;
mux2 inst1(.d0(d0),.d1(d1),.sel(sel[0]),.z(g0));
mux2 inst2(.d0(d2),.d1(d3),.sel(sel[0]),.z(g1));
mux2 inst3(.d0(g0),.d1(g1),.sel(sel[1]),.z(z));

endmodule
