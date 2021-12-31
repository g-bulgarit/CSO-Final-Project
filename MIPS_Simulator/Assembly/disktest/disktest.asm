# 128 ints per sector (128 words) = 512bytes
# $s2 = copySectorIdx
# $t2 = diskstatus
    add $s2, $zero, $zero, $imm1, 6, 0   # s2 = 7 (copySectorIdx = 6)

loop1:
    blt $zero, $s2, $imm, $imm2, 0, end  # check if we are done

check_ready_1:
    in $t2, $imm1, $zero, $zero, 17, 0  # 1 if busy
    beq $zero, $t2, $imm1, $imm2, 1, check_ready_1 # disk busy
    
    out $zero, $imm1, $zero, $s2, 15, 0  # Set disk sector = $s2
    out $zero, $imm1, $zero, $zero, 16, 0  # Set mem addr = $zero
    out $zero, $imm1, $zero, $imm2, 14, 1  # Set diskcmd = read

check_ready_2:
    in $t2, $imm1, $zero, $zero, 17, 0  # 1 if busy
    beq $zero, $t2, $imm1, $imm2, 1, check_ready_2 # disk busy

    add $s2, $zero, $zero, $imm1, 1, 0  # increment idx
    out $zero, $imm1, $zero, $s2, 15, 0  # Set disk sector = $s2
    out $zero, $imm1, $zero, $zero, 16, 0  # Set mem addr = $zero
    out $zero, $imm1, $zero, $imm2, 14, 2  # Set diskcmd = read

    sub $s2, $zero, $zero, $imm1, 2, 0  # increment idx by 2
    bne $zero, $zero, $zero, $imm2, 0, loop1

end:
    halt $zero, $zero, $zero, $zero, 0, 0
# Initialization:
# CopySectorIdx = 6

# copy_sector():
#     A: while diskcmd != 0:
#         goto A
#   
#     do stuff
#
#     B: while diskcmd != 0:
#         goto B
#   
#     do stuff
#
#

# while (CopySectorIdx >= 0):
#     copy_sector()
#     CopySectorIdx -= 1



# copy-sector function():
#   Check disk status (while true)
#   READ:
#       set disk sector to copy FROM
#       set ram[0] to be the buffer for the data
#       read sector into ram (diskcmd = 1)
#
#   Check disk status (while true)
#   WRITE:
#       set disksector to copy TO
#       diskcmd = 1
