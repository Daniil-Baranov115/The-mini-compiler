	.text
	.file	"mini_compiler"
	.globl	compiled_fn
	.p2align	2
	.type	compiled_fn,@function
compiled_fn:
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	cmp	x0, #1
	stp	xzr, x0, [sp, #8]
	str	xzr, [sp, #24]
	b.eq	.LBB0_5
	cmp	x0, #2
	b.eq	.LBB0_4
	cmp	x0, #3
	b.ne	.LBB0_7
	mov	w8, #30
	b	.LBB0_6
.LBB0_4:
	mov	w8, #20
	b	.LBB0_6
.LBB0_5:
	mov	w8, #10
.LBB0_6:
	str	x8, [sp, #24]
	b	.LBB0_8
.LBB0_7:
	str	xzr, [sp, #24]
.LBB0_8:
	ldr	x0, [sp, #24]
	str	x0, [sp, #8]
	add	sp, sp, #32
	ret
.Lfunc_end0:
	.size	compiled_fn, .Lfunc_end0-compiled_fn
	.cfi_endproc

	.section	".note.GNU-stack","",@progbits
