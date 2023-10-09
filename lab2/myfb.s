.data
    n: .word 0
    a: .word 0
    b: .word 1
    i: .word 1
    fmt1:      .asciz "%d\000"
    fmt2: .asciz "%d\n"
.text
.global main

main:
    @ 保存寄存器
    push {r4, r5, r6, lr}

    @ 输出提示信息
    
    
    ldr r0, =fmt1
    ldr r1, =n
    bl  __isoc99_scanf


    ldr r1, =b
    ldr r2, =a
    ldr r3, =i

    @ 输出a和b的初始值
    ldr r0, =a
    ldr r1, [r0]
    ldr r0, =fmt2
    mov r2, r1
    bl printf

    ldr r0, =b
    ldr r1, [r0]
    ldr r0, =fmt2
    mov r2, r1
    bl printf

loop:
    @ 检查i是否小于n
    ldr r0, =i
    ldr r1, =n
    ldr r0, [r0]
    ldr r1, [r1]
    cmp r0, r1
    bge end

    @ 计算新的b值并输出
    
    ldr r0, =a
    ldr r2, [r0]
    ldr r0, =b
    ldr r1, [r0]
    cmp r1, r2  
    ble save_a 

    bl save_b

done:
    ldr r0, =b
    add r1, r1, r2
    str r1, [r0]

    

    ldr r0, =b
    ldr r1, [r0]
    ldr r0, =fmt2
    mov r2, r1
    bl printf

    @ 更新a和i
    ldr r0, =b
    ldr r1, [r0]
    str r1, [r0]
    
    ldr r0, =i
    ldr r1, [r0]
    add r1, r1, #1
    str r1, [r0]

    @ 继续循环
    b loop

end:
    @ 恢复寄存器并退出
    pop {r4, r5, r6, pc}



save_b:
    ldr r0, =a
    str r2, [r0]  ; @将 r2 的值保存到 a
    

save_a:
    ldr r0, =a
    str r1, [r0]  ; @将 r1 的值保存到 a
    b done

