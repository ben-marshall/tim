;
; This test is not designed to produce a useful result. It simply provides every version of an
; instruction and make sure that the hardware can fetch all of them properly.
;

LOAD  $R0 $R2 $R2
STORE $R0 $R2 $R2 
PUSH  $R0
POP   $R0
MOV   $R0 $R2
TEST  $R0 $R2
AND   $R0 $R2 $R2 
NAND  $R0 $R2 $R2 
OR    $R0 $R2 $R2 
NOR   $R0 $R2 $R2 
XOR   $R0 $R2 $R2 
LSL   $R0 $R2 $R2 
LSR   $R0 $R2 $R2 
NOT   $R0 $R2
IADD  $R0 $R2 $R2 
ISUB  $R0 $R2 $R2 
IMUL  $R0 $R2 $R2 
IDIV  $R0 $R2 $R2 
IASR  $R0 $R2 $R2 
FADD  $R0 $R2 $R2 
FSUB  $R0 $R2 $R2 
FMUL  $R0 $R2 $R2 
FDIV  $R0 $R2 $R2 
FASR  $R0 $R2 $R2 
NOP   
                ; Immediate versions of instructions.    
LOAD   $R1 $R2 0xAB4
STORE  $R1 $R2 0xAB4 
MOV    $R1 0xAB4 
AND    $R1 $R2 0xAB4 
NAND   $R1 $R2 0xAB4 
OR     $R1 $R2 0xAB4 
NOR    $R1 $R2 0xAB4 
XOR    $R1 $R2 0xAB4 
LSL    $R1 $R2 0xAB4 
LSR    $R1 $R2 0xAB4 
IADD   $R1 $R2 0xAB4 
ISUB   $R1 $R2 0xAB4 
IMUL   $R1 $R2 0xAB4 
IDIV   $R1 $R2 0xAB4 
IASR   $R1 $R2 0xAB4 
FADD   $R1 $R2 0xAB4 
FSUB   $R1 $R2 0xAB4 
FMUL   $R1 $R2 0xAB4 
FDIV   $R1 $R2 0xAB4 
FASR   $R1 $R2 0xAB4 

                ; Some conditionally execution instructions.
?T IASR  $R0 $R2 $R2 
?T FADD  $R0 $R2 $R2 
?F FSUB  $R0 $R2 $R2 
?A FMUL  $R0 $R2 $R2 
?Z FDIV  $R0 $R2 $R2 
?Z FASR  $R0 $R2 $R2 

RETURN
HALT

