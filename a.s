# Standard program preamble
	.globl main

	.text

main:

	# Handling variable storage
	li $t0, 15
	# Handling division
	move $t3, $t0
	li $t4, 5
	div $t1, $t3, $t4
	# Handling multiplication
	move $t5, $t1
	li $t6, 2
	mul $t2, $t5, $t6
	# Handling return
	move $a0, $t2
	li $v0, 1
	syscall
	# Standard program exit
	li $v0, 10
	syscall
