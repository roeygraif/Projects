// 2->1 multiplexer template
module mux2 (
    input logic d0,          // Data input 0
    input logic d1,          // Data input 1
    input logic sel,         // Select input
    output logic z           // Output
);

logic g0,g1,c0,c1,b0,b1,seln;
NOT #(.Tpdhl(5), .Tpdlh(5)) not_inst1(.Z(g0),.A(d0));
NOT #(.Tpdhl(5), .Tpdlh(5)) not_inst2(.Z(g1),.A(d1));
NOT #(.Tpdhl(5), .Tpdlh(5)) not_inst3(.Z(seln),.A(sel));
OR2 #(.Tpdhl(1), .Tpdlh(1)) or_inst1(.Z(c0),.A(sel), .B(g0));
OR2 #(.Tpdhl(1), .Tpdlh(1)) or_inst2(.Z(c1),.A(seln), .B(g1));
NOT #(.Tpdhl(5), .Tpdlh(5)) not_inst4(.Z(b0),.A(c0));
NOT #(.Tpdhl(5), .Tpdlh(5)) not_inst5(.Z(b1),.A(c1));
OR2 #(.Tpdhl(1), .Tpdlh(1)) or_inst3(.Z(z),.A(b0), .B(b1));

endmodule
