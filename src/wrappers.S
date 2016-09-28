.code64 
.text

.macro PUSH_ALL
    pushq %rax 
    pushq %rbx 
    pushq %rcx 
    pushq %rdx 
    pushq %rsi 
    pushq %rdi 
    pushq %rbp 
    pushq %r8  
    pushq %r9 
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13 
    pushq %r14
    pushq %r15
.endm

.macro POP_ALL
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9 
    popq %r8  
    popq %rbp 
    popq %rdi 
    popq %rsi 
    popq %rdx 
    popq %rcx 
    popq %rbx 
    popq %rax 
.endm

.global handler_wrappers
.extern interrupt_handler

.macro DEFINE_WRAPPER n
	wrapper\n:
		PUSH_ALL
		movq $\n, %rdi    
	    movq $0, %rsi
		cld
		call interrupt_handler
		POP_ALL
		iretq
.endm

.macro DEFINE_EXCEPTION_WRAPPER n
	wrapper\n:
		PUSH_ALL
		movq $\n, %rdi
		movq 120(%rsp), %rsi // 15 pushed registres * 8 bytes = 120
		cld
		call interrupt_handler
		POP_ALL
		addq $8, %rsp // like-a-pop from stack	
		iretq
.endm

DEFINE_WRAPPER 0
DEFINE_WRAPPER 1 
DEFINE_WRAPPER 2
DEFINE_WRAPPER 3
DEFINE_WRAPPER 4
DEFINE_WRAPPER 5
DEFINE_WRAPPER 6
DEFINE_WRAPPER 7
DEFINE_EXCEPTION_WRAPPER 8
DEFINE_WRAPPER 9
DEFINE_EXCEPTION_WRAPPER 10
DEFINE_EXCEPTION_WRAPPER 11
DEFINE_EXCEPTION_WRAPPER 12
DEFINE_EXCEPTION_WRAPPER 13
DEFINE_EXCEPTION_WRAPPER 14
DEFINE_WRAPPER 15
DEFINE_WRAPPER 16
DEFINE_EXCEPTION_WRAPPER 17
DEFINE_WRAPPER 18
DEFINE_WRAPPER 19
DEFINE_WRAPPER 20
DEFINE_WRAPPER 21
DEFINE_WRAPPER 22
DEFINE_WRAPPER 23
DEFINE_WRAPPER 24
DEFINE_WRAPPER 25
DEFINE_WRAPPER 26
DEFINE_WRAPPER 27
DEFINE_WRAPPER 28
DEFINE_WRAPPER 29
DEFINE_WRAPPER 30
DEFINE_WRAPPER 31
DEFINE_WRAPPER 32
DEFINE_WRAPPER 33
DEFINE_WRAPPER 34
DEFINE_WRAPPER 35
DEFINE_WRAPPER 36
DEFINE_WRAPPER 37
DEFINE_WRAPPER 38
DEFINE_WRAPPER 39
DEFINE_WRAPPER 40
DEFINE_WRAPPER 41
DEFINE_WRAPPER 42
DEFINE_WRAPPER 43
DEFINE_WRAPPER 44
DEFINE_WRAPPER 45
DEFINE_WRAPPER 46
DEFINE_WRAPPER 47

handler_wrappers:
    .quad wrapper0
    .quad wrapper1 
    .quad wrapper2
    .quad wrapper3
    .quad wrapper4
    .quad wrapper5
    .quad wrapper6
    .quad wrapper7
    .quad wrapper8
    .quad wrapper9
    .quad wrapper10
    .quad wrapper11
    .quad wrapper12
    .quad wrapper13
    .quad wrapper14
    .quad wrapper15
    .quad wrapper16
    .quad wrapper17
    .quad wrapper18
    .quad wrapper19
    .quad wrapper20
    .quad wrapper21
    .quad wrapper22
    .quad wrapper23
    .quad wrapper24
    .quad wrapper25
    .quad wrapper26
    .quad wrapper27
    .quad wrapper28
    .quad wrapper29
    .quad wrapper30
    .quad wrapper31
    .quad wrapper32
    .quad wrapper33
    .quad wrapper34
    .quad wrapper35
    .quad wrapper36
    .quad wrapper37
    .quad wrapper38
    .quad wrapper39
    .quad wrapper40
    .quad wrapper41
    .quad wrapper42
    .quad wrapper43
    .quad wrapper44
    .quad wrapper45
    .quad wrapper46
    .quad wrapper47

