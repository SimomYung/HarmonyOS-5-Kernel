
.global arm_signal_process

.macro PUSH reg1, reg2
    stp     \reg1, \reg2, [sp, #-16]!
.endm

.macro POP reg1, reg2
    ldp     \reg1, \reg2, [sp], #16
.endm

.macro PUSH_REGS
    PUSH    x28, x29
    PUSH    x26, x27
    PUSH    x24, x25
    PUSH    x22, x23
    PUSH    x20, x21
    PUSH    x18, x19
    PUSH    x16, x17
    PUSH    x14, x15
    PUSH    x12, x13
    PUSH    x10, x11
    PUSH    x8,  x9
    PUSH    x6,  x7
    PUSH    x4,  x5
    PUSH    x2,  x3
    PUSH    x0,  x1
.endm

.macro POP_REGS
    POP     x0,  x1
    POP     x2,  x3
    POP     x4,  x5
    POP     x6,  x7
    POP     x8,  x9
    POP     x10, x11
    POP     x12, x13
    POP     x14, x15
    POP     x16, x17
    POP     x18, x19
    POP     x20, x21
    POP     x22, x23
    POP     x24, x25
    POP     x26, x27
    POP     x28, x29
.endm

.global arm_do_signal
.hidden arm_do_signal
.type arm_do_signal,%function
arm_do_signal:
    PUSH_REGS
    sub sp, sp, #16
    bl arm_signal_process
    add sp, sp, #16
    POP_REGS
    b __restore

.global __restore
.hidden __restore
.type __restore,%function
__restore:
    mov x8,#119
    svc 0x0

.global __restore_rt
.hidden __restore_rt
.type __restore_rt,%function
__restore_rt:
    mov x8,#173
    svc 0x0
