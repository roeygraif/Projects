// 32X32 Multiplier FSM
module mult32x32_fast_fsm (
    input logic clk,              // Clock
    input logic reset,            // Reset
    input logic start,            // Start signal
    input logic a_msw_is_0,       // Indicates MSW of operand A is 0
    input logic b_msw_is_0,       // Indicates MSW of operand B is 0
    output logic busy,            // Multiplier busy indication
    output logic a_sel,           // Select one 2-byte word from A
    output logic b_sel,           // Select one 2-byte word from B
    output logic [1:0] shift_sel, // Select output from shifters
    output logic upd_prod,        // Update the product register
    output logic clr_prod         // Clear the product register
);


typedef enum {idle_st, ab_lsb, a_lsb, b_lsb, ab_msb} s_type;
s_type current_state;
s_type next_state;

always_ff @(posedge clk, posedge reset) begin	
	if(reset == 1'b1) begin
		current_state <= idle_st;
	end
	else begin
		current_state <= next_state;
		end
end
	
	
	
	
always_comb begin
	busy = 1'b1;
	upd_prod = 1'b1;
	clr_prod = 1'b0;
	
	case(current_state)
	idle_st: begin
		if(start == 1'b1) begin
			next_state = ab_lsb;
		end
		else begin
			next_state = idle_st;
		end
		a_sel = 1'b0;
		b_sel = 1'b0;
		shift_sel = 2'b00;
		busy = 1'b0;
		upd_prod = 1'b0;
	end
	
	
	ab_lsb: begin
		next_state = a_lsb;
		if(a_msw_is_0 == 1'b1 && b_msw_is_0 == 1'b1) begin
			next_state = idle_st;
		end
		if(a_msw_is_0 == 1'b0 && b_msw_is_0 == 1'b1) begin
			next_state = b_lsb;
		end
		
		a_sel = 1'b0;
		b_sel = 1'b0;
		shift_sel = 2'b00;
		clr_prod = 1'b1;
	end
	
	
	a_lsb: begin
		if(a_msw_is_0 == 1'b1 && b_msw_is_0 == 1'b0) begin
			next_state = idle_st;
		end
		else begin
			next_state = b_lsb;
		end
		a_sel = 1'b0;
		b_sel = 1'b1;
		shift_sel = 2'b01;
	end
	
	
	b_lsb: begin
		if(a_msw_is_0 == 1'b0 && b_msw_is_0 == 1'b1) begin
			next_state = idle_st;
		end
		else begin
			next_state = ab_msb;
		end
		a_sel = 1'b1;
		b_sel = 1'b0;
		shift_sel = 2'b01;
	end


	ab_msb: begin
		if(start == 1'b0) begin
			next_state = idle_st;
		end
		else begin
			next_state = ab_lsb;
		end
		a_sel = 1'b1;
		b_sel = 1'b1;
		shift_sel = 2'b10;
	end
	endcase
end


endmodule
