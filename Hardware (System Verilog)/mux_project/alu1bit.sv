// 1-bit ALU template
module alu1bit (
    input logic a,           // Input bit a
    input logic b,           // Input bit b
    input logic cin,         // Carry in
    input logic [1:0] op,    // Operation
    output logic s,          // Output S
    output logic cout        // Carry out
);
logic c0,_nor,_xor,d,op0_not;


OR2 #(.Tpdhl(1), .Tpdlh(1)) or_inst1(.Z(c0),.A(a), .B(b));
NOT #(.Tpdhl(5), .Tpdlh(5)) not_inst1(.Z(_nor),.A(c0));


XOR2 #(.Tpdhl(4), .Tpdlh(4)) xor_inst1(.Z(_xor),.A(a), .B(b));

NOT #(.Tpdhl(5), .Tpdlh(5)) not_inst2(.Z(op0_not),.A(op[0]));
fas fas_inst1(.a(a),.b(b),.cin(cin),.a_ns(op0_not),.s(d),.cout(cout));

mux4 mux4_inst1(.d0(_nor),.d1(_xor),.d2(d),.d3(d),.sel(op),.z(s));




endmodule
