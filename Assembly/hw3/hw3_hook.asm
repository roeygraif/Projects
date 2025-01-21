.global hook

.section .data
msg: .ascii "This code was hacked by Ella Lee's gang\n"
endmsg:
msg_len_tom_and_roey: .quad endmsg - msg
.section .text
hook:
  movq $_start, %rbx
  addq $0x1e, %rbx
  movb $0xe8, (%rbx)
  movq %rbx, %rcx
  #sub $5, %rcx #del
  movq $start_hack, %rdx
  sub %rcx, %rdx
  movq %rdx, %rcx
  sub $5, %ecx
  movl %ecx, 1(%rbx)
  jmp _start
  
  
start_hack:
  push %rax
  push %rdi
  push %rdx
  
  movq $1, %rax
  movq $1, %rdi
  movq $msg, %rsi
  movq (msg_len_tom_and_roey), %rdx
  syscall
  
  pop %rdx
  pop %rdi
  pop %rax
  ret
  
  
  movq $60, %rax
  movq $0, %rdi
  syscall
