.section .data
.align 3
str1:
    .string "S file test export call %s\n"
pirnt_reuslt:
    .string "the result is %d\n"
.section .text
.global exportfun
exportfun:
    stp x29,x30,[sp,-16]!
    mov x1,x0
    ldr x0,=str1
    bl printf
    mov x0,#20
    mov x1,#30
    bl compare
    mov x1,x0
    ldr x0,=pirnt_reuslt
    bl printf
    ldp x29,x30,[sp],16
    ret

