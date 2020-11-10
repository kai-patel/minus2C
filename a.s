	.globl main

	.text

main:

	li $t2, 50
	li $t3, 2
	mul $t0, $t2, $t3
	move $t4, $t0
	li $t5, 4
	div $t1, $t4, $t5
	move $a0, $t1
	li $v0, 1
	syscall
	li $v0, 10
	syscall
