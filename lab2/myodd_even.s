.data
format_str:     .asciz "%d\000"
num: .word 0
even_str: .asciz "偶数\n"
odd_str: .asciz "奇数\n"
.text
.global main

main:
    @ 保存寄存器
    push {r4, lr}

    @ 读取整数并存储到寄存器r0
    ldr     r0, =format_str
    ldr     r1, =num
    bl      __isoc99_scanf

    @ 判断num是否为偶数
    ldr r2, =num   @ 
    ldr     r2, [r2]
    and r3, r2, #1   @ 使用位与操作检查最低位
    cmp r3, #0
    beq even

odd:
    @ 如果flag为0，打印"奇数"并跳到程序结束
    ldr r0, =odd_str
    bl printf
    b end

even:
    @ 如果flag为1，打印"偶数"并跳到程序结束
    ldr r0, =even_str
    bl printf

end:
    @ 恢复寄存器并退出程序
    pop {r4, lr}
    mov r0, #0
    bx lr


