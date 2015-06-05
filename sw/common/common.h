/*!
@ingroup sw-common
@{
@file common.h
@brief Header file for all common code shared across the tim toolchain.
*/

#include "stdio.h"

#ifndef COMMON_H
#define COMMON_H


// --------------------------- IO Macros ----------------------------------------------

//! The prefix for all stdout/stderr messages.
#define TIM_PRINT_PROMPT "\e[1;36mtim>\e[0m "

//! Prints the prompt to stdout.
#define TIM_PROMPT  printf(TIM_PRINT_PROMPT)

//! Masking print macro that places the prompt in front of the message.
#define tprintf(...) {TIM_PROMPT; printf(__VA_ARGS__);}

//! Alias for the tprintf macro.
#define log(...) {TIM_PROMPT; printf(__VA_ARGS__);}

#ifdef DEBUG

    //! Debug macro that prints line number, function name and file.
    #define debug(...) {TIM_PROMPT; \
                        printf("line %d of %s in %s\n", __LINE__, __FUNCTION__, __FILE__); \
                        TIM_PROMPT; \
                        printf(__VA_ARGS__);}
#endif
#ifndef DEBUG

    //! Alternative debug macro that is evaluated to NOP when we are not in a debug build.
    #define debug(...) ; 

#endif

//! Warning macro that spits out line number and function.
#define warning(...) {TIM_PROMPT; \
                   printf("\e[1;33m[Warning] \e[0m"); \
                   printf("line %d of %s in %s\n", __LINE__, __FUNCTION__, __FILE__); \
                   TIM_PROMPT; \
                   printf("\e[1;33m[Warning] \e[0m"); \
                   printf(__VA_ARGS__); \
                   TIM_PROMPT; printf("\n"); }

//! Error macro that spits out the line number and function.
#define error(...) {TIM_PROMPT; \
                   printf("\e[1;31m[Error] \e[0m"); \
                   printf("line %d of %s in %s\n", __LINE__, __FUNCTION__, __FILE__); \
                   TIM_PROMPT; \
                   printf("\e[1;31m[Error] \e[0m"); \
                   printf(__VA_ARGS__); \
                   TIM_PROMPT; printf("\n"); }

//! Fatal error macro that behaves the same as error() but also exits the program.
#define fatal(...) {error(__VA_ARGS__); \
                   TIM_PROMPT; \
                   printf("\e[1;31m[ FATAL ERROR ] \e[0m\n"); \
                   exit(1);}

// --------------------------- TIM Instructions and opcodes ---------------------------

//! Boolean type.
typedef unsigned char BOOL;

#define TRUE 1
#define FALSE 0

//! typedef for tim immediate values as operands to instructions.
typedef int tim_immediate;

//! Encodes the address of the registers.
typedef enum tim_register_e{
    R0 = 0,
    R1 = 1, 
    R2 = 2, 
    R3 = 3, 
    R4 = 4, 
    R5 = 5, 
    R6 = 6, 
    R7 = 7, 
    R8 = 8, 
    R9 = 9, 
    R10= 10, 
    R11= 11, 
    R12= 12,
    R13= 13, 
    R14= 14, 
    R15= 15, 
    PC = 16,
    SP = 17,
    LR = 18,
    TR = 19,
    SR = 20,
    IR = 21,
    IS = 22,
    REG_RESERVED_1 = 23,
    T0 = 24,
    T1 = 25,
    T2 = 26,
    T3 = 27,
    T4 = 28,
    T5 = 29,
    T6 = 30,
    T7 = 31,
    REG_ERROR=32,
    REG_NOT_USED = 33
} tim_register;

//! Encodes the size in bytes of an instruction in memory.
typedef unsigned char tim_instruction_size;

//! Encodes all of the valid instruction opcodes and their bit values.
typedef enum tim_instruction_opcode_e {
    LOADR = 1 , //!< Load to register X from address in register Y with offset in register Z.
    LOADI = 2 , //!< Load to register X from address in register Y with immediate offset.
    STORI = 3 , //!< Store register X to address in register Y with offset in register Z.
    STORR = 4 , //!< Store register X to address in register Y with immediate offset.
    PUSH  = 5 , //!< Push register X onto the top of the stack and decrement the stack pointer.
    POP   = 6 , //!< Pop element at top of stack into register X and increment the stack pointer.
    MOVR  = 7 , //!< Move the content of register X into register Y
    MOVI  = 8 , //!< Move immediate I into register X
    JUMPR = 9 , //!< Jump to address contained within register X
    JUMPI = 10, //!< Jump to address contained within instruction immediate.
    CALLR = 11, //!< Call to function who's address is contained within register X
    CALLI = 12, //!< Call to function who's address is contained within instruction immediate.
    RETURN= 13, //!< Return from the last function call.
    TEST  = 14, //!< Test two general or special registers and set comparison bits.
    HALT  = 15, //!< Stop processing and wait to be reset.
    ANDR  = 0 , //!< Bitwise AND two registers together.
    NANDR = 16, //!< Bitwise NAND two registers together.
    ORR   = 17, //!< Bitwise OR two registers together.
    NORR  = 18, //!< Bitwise NOR two registers together.
    XORR  = 19, //!< Bitwise XOR two registers together.
    LSLR  = 20, //!< Logical shift left the bits in register X by the value in register Y.
    LSRR  = 21, //!< Logical shift right the bits in register X by the value in register Y.
    NOTR  = 22, //!< Bitwise invert the specificed register.
    ANDI  = 23, //!< Bitwise AND two registers together.
    NANDI = 24, //!< Bitwise NAND two registers together.
    ORI   = 25, //!< Bitwise OR two registers together.
    NORI  = 26, //!< Bitwise NOR two registers together.
    XORI  = 27, //!< Bitwise XOR two registers together.
    LSLI  = 28, //!< Logical shift left the bits in register X by the immediate value
    LSRI  = 29, //!< Logical shift right the bits in register X by the immediate value
    IADDI = 30, //!< Integer Add register X to immediate value.
    ISUBI = 31, //!< Integer Subtract immediate value from register X.
    IMULI = 32, //!< Integer Multiply register X by immediate value.
    IDIVI = 33, //!< Integer Divide register X by immediate value.
    IASRI = 34, //!< Integer Arithmetic shift register X right immediate value.
    IADDR = 35, //!< Integer Add register X to register Y.
    ISUBR = 36, //!< Integer Subtract register X from register Y.
    IMULR = 37, //!< Integer Multiply register X by register Y.
    IDIVR = 38, //!< Integer Divide register X by register Y.
    IASRR = 39, //!< Integer Arithmetic shift register X right value in register Y.
    FADDI = 40, //!< Floating point Add register X to immediate value.
    FSUBI = 41, //!< Floating point Subtract immediate value from register X.
    FMULI = 42, //!< Floating point Multiply register X by immediate value.
    FDIVI = 43, //!< Floating point Divide register X by immediate value.
    FASRI = 44, //!< Floating point Arithmetic shift register X right immediate value.
    FADDR = 45, //!< Floating point Add register X to register Y.
    FSUBR = 46, //!< Floating point Subtract register X from register Y.
    FMULR = 47, //!< Floating point Multiply register X by register Y.
    FDIVR = 48, //!< Floating point Divide register X by register Y.
    FASRR = 49, //!< Floating point Arithmetic shift register X right value in register Y.
    SLEEP = 50, //!< Sleeps the core for a certain number of cycles.
    NOT_EMITTED=51 //!< Used in the parse tree for instruction like DATA that are not emitted.
} tim_instruction_opcode;
    

//! The opcode of the instruction.
tim_instruction_opcode opcode;

//! The number of bytes the instruction occupies in memory.
tim_instruction_size   size;

//! A condition code for conditional execution.
typedef enum tim_condition_e{
    ALWAYS = 0,
    IFTRUE   = 1,
    IFFALSE  = 2,
    IFZERO   = 3
} tim_condition;

/*!
@brief Checks if the supplied register is a general purpose register or not.
@param [in] reg - the register to test.
@returns TRUE if the register is a gp reg, otherwise false.
*/
BOOL tim_is_general_register(tim_register reg);

/*!
@brief Checks if the supplied register is a temporary register or not.
@param [in] reg - the register to test.
@returns TRUE if the register is a temporary reg, otherwise false.
*/
BOOL tim_is_temp_register(tim_register reg);

/*!
@brief Checks if the supplied register is a special register or not.
@param [in] reg - the register to test.
@returns TRUE if the register is a special reg, otherwise false.
*/
BOOL tim_is_special_register(tim_register reg);

#endif

//! }@
