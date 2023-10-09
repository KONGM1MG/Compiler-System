	.arch armv7-a
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"isprime.c"
	.text
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%d\000"
	.global	__aeabi_idivmod
	.align	2
.LC1:
	.ascii	"yes!/n\000"
	.align	2
.LC2:
	.ascii	"no!/n\000"
	.text
	.align	1
	.global	main
	.arch armv7-a
	.syntax unified
	.thumb
	.thumb_func
	.fpu vfpv3-d16
	.type	main, %function
main:
    @这一部分是为了建立函数运行的环境
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #16
	add	r7, sp, #0
	ldr	r2, .L11
.LPIC3:
	add	r2, pc
	ldr	r3, .L11+4
	ldr	r3, [r2, r3]
	ldr	r3, [r3]
	str	r3, [r7, #12]
	mov	r3,#0
	mov	r3, r7
	mov	r1, r3
	ldr	r3, .L11+8
.LPIC0:
	add	r3, pc
	mov	r0, r3
	bl	__isoc99_scanf(PLT)
	movs	r3, #1
	str	r3, [r7, #4]
	ldr	r3, [r7]
	cmp	r3, #1
	bgt	.L2
	movs	r3, #0
	str	r3, [r7, #4]
	b	.L3
.L2:
	movs	r3, #2
	str	r3, [r7, #8]
	b	.L4
.L6:
	ldr	r3, [r7]
	ldr	r1, [r7, #8]
	mov	r0, r3
	bl	__aeabi_idivmod(PLT)
	mov	r3, r1
	cmp	r3, #0
	bne	.L5
	movs	r3, #0
	str	r3, [r7, #4]
	b	.L3
.L5:
	ldr	r3, [r7, #8]
	adds	r3, r3, #1
	str	r3, [r7, #8]
.L4:
	ldr	r3, [r7, #8]
	mul	r2, r3, r3
	ldr	r3, [r7]
	cmp	r2, r3
	ble	.L6
.L3:
	ldr	r3, [r7, #4]
	cmp	r3, #1
	bne	.L7
	ldr	r3, .L11+12
.LPIC1:
	add	r3, pc
	mov	r0, r3
	bl	printf(PLT)
	b	.L8
.L7:
	ldr	r3, .L11+16
.LPIC2:
	add	r3, pc
	mov	r0, r3
	bl	printf(PLT)
.L8:
	movs	r3, #0
	ldr	r1, .L11+20
.LPIC4:
	add	r1, pc
	ldr	r2, .L11+4
	ldr	r2, [r1, r2]
	ldr	r1, [r2]
	ldr	r2, [r7, #12]
	eors	r1, r2, r1
	mov	r2, #0
	beq	.L10
	bl	__stack_chk_fail(PLT)
.L10:
	mov	r0, r3
	adds	r7, r7, #16
	mov	sp, r7
	@ sp needed
	pop	{r7, pc}
.L12:
	.align	2
.L11:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC3+4)
	.word	__stack_chk_guard(GOT)
	.word	.LC0-(.LPIC0+4)
	.word	.LC1-(.LPIC1+4)
	.word	.LC2-(.LPIC2+4)
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC4+4)
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0"
	.section	.note.GNU-stack,"",%progbits
