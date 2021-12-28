    add $s0, $imm1, $zero, $zero, -4, 0 
    beq $zero, $zero, $zero, $imm2, 0, rows     # Jump to rows

rows:
    add $s0, $imm1, $s0, $zero, 4, 0           # t0 += 1
    add $s1, $imm1, $zero, $zero, -1, 0        # t1 = min_offset - 1
    blt $zero, $s0, $imm1, $imm2, 16, cols      # check a_idx < 16
    beq $zero, $zero, $zero, $imm2, 0, endloop # else

cols:
    add $s1, $imm1, $s1, $zero, 1, 0           # t1 += 1
    add $a0, $s0, $zero, $zero, 0, 0           # a0 = t0 
    add $a1, $s1, $zero, $zero, 0, 0           # a1 = t1 
    bge $zero, $s1, $imm1, $imm2, 4, rows      # Don't execute dot prod
    jal $ra, $zero, $zero, $imm2, 0, dotprod   #
    beq $zero, $zero, $zero, $imm2, 0, cols    # else

dotprod:
    # dotproduct(row_idx: a0, col_idx: a1, sum_so_far:) -> return: res
    add $v0, $zero, $zero, $zero, 0, 0
    # Calculate C idx
    mac $s2, $a0, $imm1, $a1, 1, 0            # s2 = (row_idx 0, 4, 8) + (col_idx)

    # Iteration one
    lw $t0, $imm1, $a0, $zero, 0x100, 0       # t0 = a_row
    lw $t1, $imm1, $a1, $zero, 0x110, 0       # t1 = b_col

    mac $v0, $t0, $t1, $v0, 0, 0              # 
    add $a0, $imm1, $a0, $zero, 1, 0        # s0 += 1 
    add $a1, $imm1, $a1, $zero, 4, 0        # s1 += 4 

    # Iteration two
    lw $t0, $imm1, $a0, $zero, 0x100, 0       # t0 = a_row
    lw $t1, $imm1, $a1, $zero, 0x110, 0       # t1 = b_col

    mac $v0, $t0, $t1, $v0, 0, 0              # t2 += a_1_1 * b_1_1
    add $a0, $imm1, $a0, $zero, 1, 0        # s0 += 1 
    add $a1, $imm1, $a1, $zero, 4, 0        # s1 += 4 

    # Iteration three
    lw $t0, $imm1, $a0, $zero, 0x100, 0       # t0 = a_row
    lw $t1, $imm1, $a1, $zero, 0x110, 0       # t1 = b_col

    mac $v0, $t0, $t1, $v0, 0, 0              # t2 += a_1_1 * b_1_1
    add $a0, $imm1, $a0, $zero, 1, 0        # s0 += 1 
    add $a1, $imm1, $a1, $zero, 4, 0        # s1 += 4

    # Iteration four
    lw $t0, $imm1, $a0, $zero, 0x100, 0       # t0 = a_row
    lw $t1, $imm1, $a1, $zero, 0x110, 0       # t1 = b_col
    mac $v0, $t0, $t1, $v0, 0, 0              # t2 += a_1_1 * b_1_1

    #   sw: MEM[R[rs]+R[rt]] = R[rm] + R[rd]
    sw $v0, $s2, $imm1, $zero, 0x120, 0       # Save result to correct place
    # now v0 contains the result for the cell
    beq $zero, $zero, $zero, $ra, 0, 0       # return


endloop:
    halt $zero, $zero, $zero, $zero, 0, 0      # End program. Observe the beautiful matrix.
