.globl my_ili_handler

.text
.align 4, 0x90
my_ili_handler:
  ####### Some smart student's code here #######

  # pushq (all registers Im going to use) - store old regs
  pushq %rbx
  pushq %rdi
  pushq %rax
  pushq %r15

  #get opcode from the rip saved in the stuck at 8(%rsp) {if there is no err_code then (%rsp)}
  movq 32(%rsp), %rbx	# old_rip -> rbx
  
  xor %r15, %r15

  cmpb $0x0F, (%rbx)
  jne CALL_WHAT_TO_DO_HW2

  inc %r15
  
  CALL_WHAT_TO_DO_HW2:
    xor %rdi, %rdi
    movb (%rbx, %r15, 1), %dil
    pushq %r15
    call what_to_do
    popq %r15

  test %rax, %rax
  je CALL_DEFAULT_HANDLER_HW2

  movq %rax, %rdi
  inc %r15
  addq 32(%rsp), %r15
  movq %r15, 32(%rsp)

  
  # popq (all registers Ive used) - resore old regs
  popq %r15
  popq %rax
  addq $8, %rsp		# insted of "popq %rdi"
  popq %rbx

  jmp FINISH_HW2

  CALL_DEFAULT_HANDLER_HW2:
    # popq (all registers Ive used) - resore old regs
    popq %r15
    popq %rax
    popq %rdi
    popq %rbx

    jmp *old_ili_handler
	
  FINISH_HW2:
iretq
