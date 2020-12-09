	.globl main

	.text

main:

	# Handling < condition
	li $t3, 5
	bgt $t3, 2, label_a0
	# Handling addition
	li $t4, 5
	li $t5, 4
	add $t1, $t4, $t5
	# Handling return
	move $a0, $t1
	li $v0, 1
	syscall
	j label_b0
label_a0:
	# Handling subtraction
	li $t6, 5
	li $t7, 2
	sub $t2, $t6, $t7
	# Handling return
	move $a0, $t2
	li $v0, 1
	syscall
label_b0:
	# Handling return
	li $a0, 0
	li $v0, 10
	syscall
