# OPCODE RD RS RT RM IMM1 IMM2
# OUT = IORegister [R[rs]+R[rt]] = R[rm]

# OPCODE RD     RS     RT     RM IMM1 IMM2
    sll $sp, $imm1, $imm2, $zero, 1, 11		    # set $sp = 2048
    out $zero, $imm1, $zero, $imm2, 21, 255     # Set all pixels full white
    lw $s0, $imm1, $zero, $zero, 0x100, 0       # S0 = Circle diameter
    sra $s1, $s0, $imm1, $zero, 1, 0            # S1 = Circle radius
    sub $s2, $imm1, $s1, $zero, 128, 0          # Get min offset
    mac $s0, $s1, $imm1, $s2, 2, 0              # Get max offset

    # t0 = rows, t1 = cols
    add $t0, $imm1, $s2, $zero, -1, 0           # t0 = min_offset - 1
    beq $zero, $zero, $zero, $imm2, 0, rows     # Jump to rows

draw:
    # literally draw:
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
    blt $zero, $t1, $s0, $imm2, 0, logic   # check if t1 < max_offset
    beq $zero, $zero, $zero, $imm2, 0, rows    # else

logic:
    mac $a0, $t0, $imm1, $t1, 256, 0           # a0 = monitor offset (rows*256 + cols)
    jal $ra, $zero, $zero, $imm2, 0, draw
    beq $zero, $zero, $zero, $imm2, 0, cols

endloop:
    halt $zero, $zero, $zero, $zero, 0, 0