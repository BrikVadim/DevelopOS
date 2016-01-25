	.text
	.global loader

	.set FLAGS,	0x0
	.set MAGIC,	0x1BADB002
	.set CHECKSUM,	-(MAGIC + FLAGS)

	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

	.set STACKSIZE, 0x4000
	.lcomm stack, STACKSIZE
	.comm mbd, 4
	.comm magic, 4

loader:
	movl $(stack + STACKSIZE), %esp
	movl %eax, magic
	movl %ebx, mbd
	call kernel_main
	cli
hang:
	hlt
	jmp hang
