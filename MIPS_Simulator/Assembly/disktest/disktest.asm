    add $s2, $zero, $zero, $imm1, 7, 0              # $s2 = 7 (Start sector is 7, will be moved to 8)

start:
    blt $zero, $s2, $imm1, $imm2, 0, end            # If the sector number is <0 we're done, go to halt

check_ready_1:
    in $t2, $imm1, $zero, $zero, 17, 0              # Check if disk is busy
    beq $zero, $t2, $imm1, $imm2, 1, check_ready_1  # If busy, go up one line
    
    out $zero, $imm1, $zero, $s2, 15, 0             # Set disk sector = $s2 for reading
    out $zero, $imm1, $zero, $zero, 16, 0           # Set mem addr = $zero
    out $zero, $imm1, $zero, $imm2, 14, 1           # Set diskcmd = read

check_ready_2:
    in $t2, $imm1, $zero, $zero, 17, 0              # Check if disk is busy
    beq $zero, $t2, $imm1, $imm2, 1, check_ready_2  # If busy, go up one line

    add $s2, $s2, $zero, $imm1, 1, 0                # increment idx by one for write target
    out $zero, $imm1, $zero, $s2, 15, 0             # Set disk sector = $s2 for writing
    out $zero, $imm1, $zero, $zero, 16, 0           # Set mem addr = $zero
    out $zero, $imm1, $zero, $imm2, 14, 2           # Set diskcmd = write

    sub $s2, $s2, $imm1, $zero, 2, 0                # Decrement idx by 2 to continue with program flow
    beq $zero, $zero, $zero, $imm2, 0, start        # Jump to start, unconditionally

end:
    halt $zero, $zero, $zero, $zero, 0, 0           # End program. Observe the beautiful diskout file. 
