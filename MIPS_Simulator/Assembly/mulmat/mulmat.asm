    add $s0, $imm1, $zero, $zero, -4, 0        # Initialize rows
    beq $zero, $zero, $zero, $imm2, 0, rows    # Jump to rows

rows:
    add $s0, $imm1, $s0, $zero, 4, 0           # s0 += 4 (Increment rows by 4)
    add $s1, $imm1, $zero, $zero, -1, 0        # Initialize columns
    blt $zero, $s0, $imm1, $imm2, 16, cols     # Check if we're done with the rows
    beq $zero, $zero, $zero, $imm2, 0, endloop # If done, jump to end.

cols:
    add $s1, $imm1, $s1, $zero, 1, 0           # t1 += 1 (Increment columns by1)
    add $a0, $s0, $zero, $zero, 0, 0           # a0 = t0 (Set a0 to t0 for our function)
    add $a1, $s1, $zero, $zero, 0, 0           # a1 = t1 (Set a1 to t1 for our function)
    bge $zero, $s1, $imm1, $imm2, 4, rows      # Dont execute multiplication if col > 3
    jal $ra, $zero, $zero, $imm2, 0, dotprod   # Jump to dotprod and return to next line
    beq $zero, $zero, $zero, $imm2, 0, cols    # Redo cols

dotprod:
    add $v0, $zero, $zero, $zero, 0, 0         # Reset $v0 = 0 
    mac $s2, $a0, $imm1, $a1, 1, 0             # Calculate save offset, s2 = (row_idx 0, 4, 8) + (col_idx)

    # Multiplication 1/4
    lw $t0, $imm1, $a0, $zero, 0x100, 0        # t0 = a_<row>_<col + 0>
    lw $t1, $imm1, $a1, $zero, 0x110, 0        # t1 = b_<row + 0>_<col>
    mac $v0, $t0, $t1, $v0, 0, 0               # v0 += t0 * t1
    add $a0, $imm1, $a0, $zero, 1, 0           # a0 += 1 
    add $a1, $imm1, $a1, $zero, 4, 0           # a1 += 4 

    # Multiplication 2/4
    lw $t0, $imm1, $a0, $zero, 0x100, 0        # t0 = a_<row>_<col + 1>
    lw $t1, $imm1, $a1, $zero, 0x110, 0        # t1 = b_<row + 1>_<col>
    mac $v0, $t0, $t1, $v0, 0, 0               # v0 += t0 * t1
    add $a0, $imm1, $a0, $zero, 1, 0           # a0 += 1 
    add $a1, $imm1, $a1, $zero, 4, 0           # a1 += 4 

    # Multiplication 3/4
    lw $t0, $imm1, $a0, $zero, 0x100, 0        # t0 = a_<row>_<col + 2>
    lw $t1, $imm1, $a1, $zero, 0x110, 0        # t1 = b_<row + 2>_<col>
    mac $v0, $t0, $t1, $v0, 0, 0               # v0 += t0 * t1
    add $a0, $imm1, $a0, $zero, 1, 0           # a0 += 1 
    add $a1, $imm1, $a1, $zero, 4, 0           # a1 += 4 

    # Multiplication 4/4
    lw $t0, $imm1, $a0, $zero, 0x100, 0        # t0 = a_<row>_<col + 3>
    lw $t1, $imm1, $a1, $zero, 0x110, 0        # t1 = b_<row + 3>_<col>
    mac $v0, $t0, $t1, $v0, 0, 0               # v0 += t0 * t1
    sw $v0, $s2, $imm1, $zero, 0x120, 0        # Save result ($v0)
    beq $zero, $zero, $zero, $ra, 0, 0         # Return!

endloop:
    halt $zero, $zero, $zero, $zero, 0, 0      # End program. Observe the beautiful matrix.
