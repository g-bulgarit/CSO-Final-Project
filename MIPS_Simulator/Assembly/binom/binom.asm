	# a0 = n, a1 = k, $v0 = result

	sll $sp, $imm1, $imm2, $zero, 1, 11		# Allocate 2048 bytes on stack
	lw $a0, $zero, $imm2, $zero, 0, 0x100		# Load n
	lw $a1, $zero, $imm2, $zero, 0, 0x101		# Load k
	jal $ra, $zero, $zero, $imm2, 0, binom		# calc $v0 = binom(x)
	sw $zero, $zero, $imm2, $s1, 0, 0x102		# store bin(n,k) in address
	halt $zero, $zero, $zero, $zero, 0, 0		# halt

binom:
	add $sp, $sp, $imm2, $zero, 0, -4		# adjust stack for 4 items
	sw $zero, $sp, $imm2, $s0, 0, 3			# save $s0
	sw $zero, $sp, $imm2, $ra, 0, 2			# save return address
	sw $zero, $sp, $imm2, $a1, 0, 1			# save k (argument)
	sw $zero, $sp, $imm2, $a0, 0, 0			# save n (argument)

	beq $zero, $a1, $imm1, $imm2, 0, basecase		# jump to L1 if k==0
	beq $zero, $a0, $a1, $imm2, 0, basecase		# jump to L1 if k == n
	beq $zero, $zero, $zero, $imm2, 0, L1		# jump to L2

basecase:
	add $s1, $imm2, $s1, $zero, 0, 1		# add 1 to result
	beq $zero, $zero, $zero, $imm2, 0, recover # jump to L2


L1:
	# Prepare for binom(n-1, k)
	sub $a0, $a0, $imm2, $zero, 0, 1		# calculate n - 1
	jal $ra, $zero, $zero, $imm2, 0, binom	# calc $v0=binom(n-1, k)

	# Prepare for binom(n-1, k-1)
	add $s0, $v0, $imm2, $zero, 0, 0		# $s0 = binom(n-1, k) (save result so far)
	lw $a0, $sp, $imm2, $zero, 0, 0			# restore $a0 = n from stack
	lw $a1, $sp, $imm2, $zero, 0, 1			# restore $a1 = k from stack

	sub $a0, $a0, $imm2, $zero, 0, 1		# calculate n - 1
	sub $a1, $a1, $imm2, $zero, 0, 1		# calculate k - 1
	jal $ra, $zero, $zero, $imm2, 0, binom	# calc binom(n-1, k-1)

	# add $v0, $s1, $s0, $v0, 0, 0			# $v0 = binom(n-1, k-1) + binom(n-1, k)
	# add $s1, $zero, $zero, $zero, 0, 0 		# s1=0
	lw $a0, $sp, $imm2, $zero, 0, 0			# restore $a0 = n
	lw $a1, $sp, $imm2, $zero, 0, 1			# restore $a1 = k
	lw $ra, $sp, $imm2, $zero, 0, 2			# restore $ra from stack
	lw $s0, $sp, $imm2, $zero, 0, 3			# restore $s0 from stack

	# add $sp, $sp, $imm2, $zero, 0, 4		# pop 4 items from stack
	# beq $zero, $zero, $zero, $ra, 0, 0		# and return to main

recover:
	add $sp, $sp, $imm2, $zero, 0, 4		# pop 4 items from stack
	beq $zero, $zero, $zero, $ra, 0, 0		# and return to main
