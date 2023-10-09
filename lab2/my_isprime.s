.data
format_str:     .asciz "%d\000"
yes_msg:        .asciz "yes!\n"
no_msg:         .asciz "no!\n"
num:            .word 0  @ 分配一个4字节的整数变量并初始化为0

.text
.global main
main:
    @ 保存寄存器
    push    {r4, r5, r6, lr}

    @ 为输入num分配空间并读取输入
    ldr     r0, =format_str
    ldr     r1, =num
    bl      __isoc99_scanf
    
    @ 设置flag为1
    mov     r4, #1

    @ 如果num<=1，则将flag设置为0
    ldr     r5, =num
    ldr     r5, [r5]
    cmp     r5, #1
    ble     num_leq_1

    @ 初始化循环计数器i为2
    mov     r6, #2

loop:
    @ 计算i * i
    mul     r0, r6, r6

    @ 如果 i * i > num，则跳出循环
    cmp     r0, r5
    bgt     end_loop

    @ 计算num % i
    ldr     r0, =num
    ldr     r0, [r0]
    mov     r1, r6
    bl      divide

    @ 如果num % i == 0，则将flag设置为0并跳出循环
    cmp     r1, #0
    beq     num_leq_1

    @ 增加i的值
    add     r6, r6, #1
    b       loop

end_loop:
    @ 检查flag是否为1，然后打印相应的消息
    cmp     r4, #1
    bne     print_no
    ldr     r0, =yes_msg
    bl      printf
    b       exit_program

print_no:
    ldr     r0, =no_msg
    bl      printf

exit_program:
    @ 恢复寄存器并退出程序
    pop     {r4, r5, r6, pc}

num_leq_1:
    @ 设置flag为0
    mov     r4, #0
    b       end_loop

divide:
    @ 除法函数，将r0除以r1，结果存储在r0中，余数存储在r1中
    push    {r4, r5, r6, lr}
    mov     r4, r0
    mov     r5, r1
    mov     r6, #0
    b       loop_divide
loop_divide:
    cmp     r4, r5
    blt     done_divide
    sub     r4, r4, r5
    add     r6, r6, #1
    b       loop_divide
done_divide:
    mov     r0, r6
    mov     r1, r4
    pop     {r4, r5, r6, pc}
