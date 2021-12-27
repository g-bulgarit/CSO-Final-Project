    sll $sp, $imm1, $imm2, $zero, 1, 11		    # set $sp = 2048
    out $zero, $imm1, $zero, $imm2, 21, 255     # Set all pixels full white
    lw $s1, $imm1, $zero, $zero, 0x100, 0       # S0 = Circle diameter
    sub $s2, $imm1, $s1, $zero, 128, 0          # S2 =  min offset
    mac $s0, $s1, $imm1, $s2, 2, 0              # S0 =  max offset
    mac $s1, $s1, $s1, $zero, 0, 0              # S1 = rad^2

    add $t0, $imm1, $s2, $zero, -1, 0           # t0 = min_offset - 1, (t0 = rows, t1 = cols)
    beq $zero, $zero, $zero, $imm2, 0, rows     # Jump to rows

draw:
    out $zero, $imm1, $zero, $a0, 20, 0        # IOReg[20] = a0 (pixel offset)
    out $zero, $imm1, $zero, $imm2, 22, 1      # Set monitorcmd = 1
    beq $zero, $zero, $zero, $ra, 0, 0         # Return from function

rows:
    add $t0, $imm1, $t0, $zero, 1, 0           # t0 += 1
    add $t1, $imm1, $s2, $zero, -1, 0          # t1 = min_offset - 1
    blt $zero, $t0, $s0, $imm2, 0, cols        # check if t0 < max_offset
    beq $zero, $zero, $zero, $imm2, 0, endloop # else

cols:
    add $t1, $imm1, $t1, $zero, 1, 0           # t1 += 1
    blt $zero, $t1, $s0, $imm2, 0, logic       # check if t1 < max_offset
    beq $zero, $zero, $zero, $imm2, 0, rows    # else

logic:
    sub $t0, $t0, $imm1, $zero, 128, 0         # t0 cooridinate shift
    sub $t1, $t1, $imm1, $zero, 128, 0         # t1 cooridinate shift
    mac $t2, $t0, $t0, $zero, 0, 0             # t2 = row^2
    mac $t2, $t1, $t1, $t2, 0, 0               # t2 = (x-128)^2 + (y-128)^2
    jal $ra, $zero, $zero, $imm2, 0, recover   # Reset the values of t2 and t1 before proceeding
    mac $a0, $t0, $imm1, $t1, 256, 0           # a0 = monitor offset (rows*256 + cols)
    bgt $zero, $t2, $s1, $imm2, 0, cols        # if t2 > r^2, no draw!
    jal $ra, $zero, $zero, $imm2, 0, draw      # Draw! (can't reach this otherwise...)
    beq $zero, $zero, $zero, $imm2, 0, cols    # Move on to the next pixel

recover:
    add $t0, $t0, $imm1, $zero, 128, 0         # t0 cooridinate shift
    add $t1, $t1, $imm1, $zero, 128, 0         # t1 cooridinate shift
    beq $zero, $zero, $zero, $ra, 0, 0         # Jump back to the logic part

endloop:
    halt $zero, $zero, $zero, $zero, 0, 0      # End program. Observe the beautiful circle.