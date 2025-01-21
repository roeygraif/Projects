.global hamming_weight, negate_first_k, bring_balance_to_the_word, read_input

.section .bss
.lcomm path, 1024

.section .text
read_input:
    push %rdi     #rdi = codeword
    lea path(%rip), %rdi
    call get_path            #path has the link to the text file
    lea path(%rip), %rdi
    movq $2, %rax
    movq $0, %rsi #read only mode
    syscall      #open file
    
    
    movq %rax, %rdi       #rdi = descriptor to file
    pop %rsi              #rsi = codeword
    push %rax             #save descripto file for late
    push %rsi             #save codeword again
    until_we_find_a:
        movq $1, %rdx
        xor %rax, %rax
        push %rsi
        #push %rdi
        syscall                          #read from file
        #pop %rdi
        pop %rsi
        cmpb $0xa, (%rsi)
        je found_a
        inc %rsi
        jmp until_we_find_a
    found_a:
    movb $0x0, (%rsi)     #make it null terminated
    #inc %rsi              #rsi stands on the begining ot the code
    pop %rdi
    push %rdi
    call atam_atol      #returns in rax the number after translation
    
    
    
    movq %rax, %rdx       #rdx = number of char to read
    xor %rax, %rax
    pop %rsi              #rsi = codeword
    pop %rdi              #rdi = file descriptor
    #push %rsi             #we might need to delete this
    syscall               #read codeword
    
    
    movq $3, %rax         
    syscall               #close text
    
    
    
    shr $3, %rdx
    movq %rdx, %rax
    ret
        
    
    
           
    

        
        

hamming_weight:
    xor %rax, %rax
    do_it_again:
    test %rsi, %rsi  #if rsi = 0
    je end_ham
    dec %rsi
    movq (%rdi), %rcx    #rcx has the 64 bit we are currently counting
    add $8, %rdi    #might need to be hexa
    movq $64, %rbx
    count_bits:
        movq $1, %rdx
        and %rcx, %rdx
        add %rdx, %rax     
        shr %rcx
        
        
        dec %rbx
        test %rbx, %rbx    #if rbx not zero
        jne count_bits
    jmp do_it_again
    
    
  end_ham:
  ret

negate_first_k:       #problem is here when k = 64
    movq %rdi, %rax
    movq $-1, %rbx    #make rbx 111111...1
    movq $64, %rdx
    sub %rsi, %rdx      #rdx = 64 - k
    movq %rdx, %rsi
    cmp $0 , %rsi
    jne sal_all_the_way
    xor %rbx, %rax      #ret rax
    ret
    sal_all_the_way:
        shr %rbx
        dec %rsi
        test %rsi, %rsi
        jnz sal_all_the_way
    xor %rbx, %rax      #ret rax
  ret

bring_balance_to_the_word:     #the problem is here

    movq %rdi, %r10
    movq %rsi, %r11
    sal $6, %r11           #r11 = number of bits
    shr %r11                #r11 = len/2 = hamming_weight
    
    xor %rbx, %rbx
    xor %rcx, %rcx
    
        push %rsi
        push %rdi
        call hamming_weight
        pop %rdi
        pop %rsi
        
        cmp %rax, %r11     #if there is balance were done
        je done
        
        
    xor %rbx, %rbx          #rbx is from 0 to len   
    next:
    movq $1, %rcx          #rcx is k (might need to be rcx = 1)
    movq (%r10), %r12       #r12 is the old reg
    algo:
        movq %r12, (%r10)
        
        
        
        
        
 
        
        #save on stack
        push %rsi
        push %rbx
        push %rcx
        push %rdi    
        movq (%r10), %rdi 
        movq %rcx, %rsi
        call negate_first_k
        movq %rax, (%r10)
        #pop stack
        pop %rdi
        pop %rcx
        pop %rbx
        pop %rsi
        
    
    
        #save on stack
        push %rsi
        push %rbx
        push %rcx
        push %rdi
        call hamming_weight
        #pop stack
        pop %rdi
        pop %rcx
        pop %rbx
        pop %rsi
        
        cmp %rax, %r11     #if there is balance were done
        je done
        
        
    
        inc %rcx
        cmp $65, %rcx     #why 65 makes infinity loop                       ##############################################
        jne algo
    inc %rbx
    add $8, %r10
    jmp next



    done:
    sal $6, %rbx
    add %rcx, %rbx
    movq %rbx, %rax
  ret
