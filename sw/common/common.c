/*!
@ingroup sw-common
@{
@file common.c
@brief code source file for all common code shared across the tim toolchain.
*/

#include "common.h"

tim_asm_instruction * tim_LOAD  = "LOAD";
tim_asm_instruction * tim_STORE = "STORE"; 
tim_asm_instruction * tim_PUSH  = "PUSH"; 
tim_asm_instruction * tim_POP   = "POP"; 
tim_asm_instruction * tim_MOV   = "MOV"; 
tim_asm_instruction * tim_JUMP  = "JUMP"; 
tim_asm_instruction * tim_CALL  = "CALL"; 
tim_asm_instruction * tim_RETURN= "RETURN"; 
tim_asm_instruction * tim_TEST  = "TEST"; 
tim_asm_instruction * tim_HALT  = "HALT"; 
tim_asm_instruction * tim_AND   = "AND"; 
tim_asm_instruction * tim_NAND  = "NAND"; 
tim_asm_instruction * tim_OR    = "OR"; 
tim_asm_instruction * tim_NOR   = "NOR"; 
tim_asm_instruction * tim_XOR   = "XOR"; 
tim_asm_instruction * tim_LSL   = "LSL"; 
tim_asm_instruction * tim_LSR   = "LSR"; 
tim_asm_instruction * tim_NOT   = "NOT"; 
tim_asm_instruction * tim_IADD  = "IADD"; 
tim_asm_instruction * tim_ISUB  = "ISUB"; 
tim_asm_instruction * tim_IMUL  = "IMUL"; 
tim_asm_instruction * tim_IDIV  = "IDIV"; 
tim_asm_instruction * tim_IASR  = "IASR"; 
tim_asm_instruction * tim_FADD  = "FADD"; 
tim_asm_instruction * tim_FSUB  = "FSUB"; 
tim_asm_instruction * tim_FMUL  = "FMUL"; 
tim_asm_instruction * tim_FDIV  = "FDIV"; 
tim_asm_instruction * tim_FASR  = "FASR"; 
tim_asm_instruction * tim_NOP   = "NOP"; 
tim_asm_instruction * tim_SLEEP = "SLEEP"; 
tim_asm_instruction * tim_DATA  = "DATA"; 


/*!
@brief Checks if the supplied register is a general purpose register or not.
@param [in] reg - the register to test.
@returns TRUE if the register is a gp reg, otherwise false.
*/
BOOL tim_is_general_register(tim_register reg)
{
    if(reg >= R0 && reg <= R15)
        return TRUE;
    else
        return FALSE;
}


/*!
@brief Checks if the supplied register is a temporary register or not.
@param [in] reg - the register to test.
@returns TRUE if the register is a temporary reg, otherwise false.
*/
BOOL tim_is_temp_register(tim_register reg)
{
    if(reg >= T0 && reg <= T7)
        return TRUE;
    else
        return FALSE;
}


/*!
@brief Checks if the supplied register is a special register or not.
@param [in] reg - the register to test.
@returns TRUE if the register is a special reg, otherwise false.
*/
BOOL tim_is_special_register(tim_register reg)
{
    if(reg >= PC && reg <= IS)
        return TRUE;
    else
        return FALSE;
}

//! }@



