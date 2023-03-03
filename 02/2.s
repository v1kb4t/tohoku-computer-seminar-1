	.file	"2.c"
	.text
	.globl	g
	.data
	.align 8
	.type	g, @object
	.size	g, 8
g:
	.long	2576980378
	.long	1076074905
	.globl	pi
	.align 8
	.type	pi, @object
	.size	pi, 8
pi:
	.long	776530087
	.long	1074340351
	.section	.rodata
.LC3:
	.string	"x = %lf, y = %lf\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -40(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -32(%rbp)
	movsd	g(%rip), %xmm0
	divsd	-32(%rbp), %xmm0
	call	sqrt@PLT
	movq	%xmm0, %rax
	movq	%rax, -24(%rbp)
	movsd	.LC2(%rip), %xmm0
	movsd	%xmm0, -16(%rbp)
	movl	$1, -44(%rbp)
	jmp	.L2
.L3:
	movsd	-24(%rbp), %xmm0
	movapd	%xmm0, %xmm1
	mulsd	-16(%rbp), %xmm1
	cvtsi2sd	-44(%rbp), %xmm0
	mulsd	%xmm1, %xmm0
	call	sin@PLT
	movapd	%xmm0, %xmm1
	movsd	-40(%rbp), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -8(%rbp)
	cvtsi2sd	-44(%rbp), %xmm0
	mulsd	-16(%rbp), %xmm0
	movsd	-8(%rbp), %xmm1
	leaq	.LC3(%rip), %rdi
	movl	$2, %eax
	call	printf@PLT
	addl	$1, -44(%rbp)
.L2:
	cmpl	$10, -44(%rbp)
	jle	.L3
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	2576980378
	.long	1070176665
	.align 8
.LC1:
	.long	0
	.long	1071644672
	.align 8
.LC2:
	.long	2576980378
	.long	1069128089
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
