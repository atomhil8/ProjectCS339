# Tests all supported instructions: ADD, ADDI, SUB, MUL, AND, OR, SLL, SRL, LW, SW, BEQ, J, NOP

# Load some initial values
addi $t0, $zero, 10       # $t0 = 10
addi $t1, $zero, 5        # $t1 = 5
addi $t2, $zero, 3        # $t2 = 3
 
# Arithmetic
add $t3, $t0, $t1         # $t3 = 10 + 5 = 15
sub $t4, $t0, $t1         # $t4 = 10 - 5 = 5
mul $t5, $t0, $t2         # $t5 = 10 * 3 = 30
 
# Immediate arithmetic
addi $t6, $t3, 2          # $t6 = 15 + 2 = 17
 
# Bitwise
and $s0, $t0, $t1         # $s0 = 10 & 5 = 0
or  $s1, $t0, $t1         # $s1 = 10 | 5 = 15
 
# Shifts
sll $s2, $t1, 2           # $s2 = 5 << 2 = 20
srl $s3, $t0, 1           # $s3 = 10 >> 1 = 5
 
# Memory: store then load
sw $t3, 0($zero)          # MEM[0] = 15
sw $t5, 4($zero)          # MEM[4] = 30
lw $s4, 0($zero)          # $s4 = MEM[0] = 15
lw $s5, 4($zero)          # $s5 = MEM[4] = 30
 
# NOP
nop
 
# Branch: $t1 == $s3 (both are 5), so branch is taken to "finish"
beq $t1, $s3, finish
 
# These should be skipped if beq works correctly
addi $s6, $zero, 99
addi $s7, $zero, 99
 
finish:
# Jump to end
j end
 
# This should be skipped if j works correctly
addi $t8, $zero, 99
 
end:
nop

# -----------------------------------------------
# EXPECTED FINAL REGISTER STATE:
# $t0 = 10
# $t1 = 5
# $t2 = 3
# $t3 = 15
# $t4 = 5
# $t5 = 30
# $t6 = 17
# $s0 = 0
# $s1 = 15
# $s2 = 20
# $s3 = 5
# $s4 = 15
# $s5 = 30
# $s6 = 0  (skipped by beq)
# $s7 = 0  (skipped by beq)
# $t8 = 0  (skipped by j)
#
# EXPECTED FINAL MEMORY STATE:
# MEM[0] = 15
# MEM[4] = 30
# -----------------------------------------------