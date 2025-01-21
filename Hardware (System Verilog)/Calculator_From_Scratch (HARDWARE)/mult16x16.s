# Operands to multiply
.data
a: .word 0xBAD
b: .word 0xFEED

.text
main:   # Load data from memory
		la      t3, a
        lw      t3, 0(t3)
        la      t4, b
        lw      t4, 0(t4)
        
        # t6 will contain the result
        add		t6, x0, x0

        # Mask for 16x8=24 multiply
        ori		t0, x0, 0xff
        slli	t0, t0, 8
        ori		t0, t0, 0xff
        slli	t0, t0, 8
        ori		t0, t0, 0xff
        
####################
# Start of your code
# Use the code below for 16x8 multiplication
#   mul		<PROD>, <FACTOR1>, <FACTOR2>
#   and		<PROD>, <PROD>, t0


	add 	s0, x0, t3
	slli    s0, s0, 24
	srli    s0, s0, 24
	add 	s1, x0, t3
	srli    s1, s1, 8
	
	add 	s2, x0, t4
	slli    s2, s2, 24
	srli    s2, s2, 24
	add 	s3, x0, t4
	srli    s3, s3, 8
	
    mul		s4 , s2, s0
    and		s4, s4, t0
	
	mul		s5 , s2, s1
    and		s5, s5, t0
	slli    s5, s5, 8
	
	
	
	mul		s6 , s3, s0
    and		s6, s6, t0
	slli    s6, s6, 8
	
	mul		s7 , s3, s1
    and		s7, s7, t0
	slli    s7, s7, 16
	
	
	
	add t6, s4, s5
	add t6, t6, s6
	add t6, t6, s7
	





# End of your code
####################
		
finish: addi    a0, x0, 1
        addi    a1, t6, 0
        ecall # print integer ecall
        addi    a0, x0, 10
        ecall # terminate ecall


