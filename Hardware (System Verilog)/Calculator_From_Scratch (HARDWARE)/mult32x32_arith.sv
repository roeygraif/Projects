// 32X32 Multiplier arithmetic unit template
module mult32x32_arith (
    input logic clk,             // Clock
    input logic reset,           // Reset
    input logic [31:0] a,        // Input a
    input logic [31:0] b,        // Input b
    input logic a_sel,           // Select one 2-byte word from A
    input logic b_sel,           // Select one 2-byte word from B
    input logic [1:0] shift_sel, // Select output from shifters
    input logic upd_prod,        // Update the product register
    input logic clr_prod,        // Clear the product register
    output logic [63:0] product  // Miltiplication product
);

logic [15:0]a_temp;
logic [15:0]b_temp;
logic [31:0]ans_mult;
logic [63:0]shift;
logic [63:0]add_in1;
logic [63:0]add_in2;
logic [63:0]add_out;


always_comb begin
	case(a_sel)
		0: a_temp = a[15:0];
		1: a_temp = a[31:16];
	endcase
	case(b_sel)
		0: b_temp = b[15:0];
		1: b_temp = b[31:16];
	endcase
	
	ans_mult = a_temp*b_temp;
	
	//case(shift_sel)
		//	00: shift = ans_mult;
			//01: shift = ans_mult<<16;
	//		10: shift = ans_mult<<32;
		//	11: shift = 64'b0;
	//endcase
	case(shift_sel)
		2'b01: shift = {16'b0,ans_mult,16'b0};
		2'b10: shift = {ans_mult,32'b0};
		default: shift = {32'b0,ans_mult};
	endcase 
end

assign add_in1 = shift;
assign add_in2 = product;

always_comb begin
	add_out = add_in1 + add_in2;
end

always_ff @(posedge clk, posedge reset) begin
	if(reset == 1'b1) begin
		product <= 64'b0;
		end 
	else if(clr_prod == 1'b1) begin
		product <= shift;
		end
	else if(upd_prod == 1'b1) begin
		product <= add_out;
		end
end

endmodule