/*!
@ingroup sw-common
@{
@file common.c
@brief code source file for all common code shared across the tim toolchain.
*/

#include "common.h"

char * tim_LOAD  = "LOAD";
char * tim_STORE = "STORE"; 
char * tim_PUSH  = "PUSH"; 
char * tim_POP   = "POP"; 
char * tim_MOV   = "MOV"; 
char * tim_JUMP  = "JUMP"; 
char * tim_CALL  = "CALL"; 
char * tim_RETURN= "RETURN"; 
char * tim_TEST  = "TEST"; 
char * tim_HALT  = "HALT"; 
char * tim_AND   = "AND"; 
char * tim_NAND  = "NAND"; 
char * tim_OR    = "OR"; 
char * tim_NOR   = "NOR"; 
char * tim_XOR   = "XOR"; 
char * tim_LSL   = "LSL"; 
char * tim_LSR   = "LSR"; 
char * tim_NOT   = "NOT"; 
char * tim_IADD  = "IADD"; 
char * tim_ISUB  = "ISUB"; 
char * tim_IMUL  = "IMUL"; 
char * tim_IDIV  = "IDIV"; 
char * tim_IASR  = "IASR"; 
char * tim_FADD  = "FADD"; 
char * tim_FSUB  = "FSUB"; 
char * tim_FMUL  = "FMUL"; 
char * tim_FDIV  = "FDIV"; 
char * tim_FASR  = "FASR"; 
char * tim_NOP   = "NOP"; 
char * tim_SLEEP = "SLEEP"; 
char * tim_DATA  = "DATA"; 


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



