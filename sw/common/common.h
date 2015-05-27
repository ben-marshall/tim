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
                   printf(__VA_ARGS__);}

//! Error macro that spits out the line number and function.
#define error(...) {TIM_PROMPT; \
                   printf("\e[1;31m[Error] \e[0m"); \
                   printf("line %d of %s in %s\n", __LINE__, __FUNCTION__, __FILE__); \
                   TIM_PROMPT; \
                   printf("\e[1;31m[Error] \e[0m"); \
                   printf(__VA_ARGS__);}

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
    PC = 0,
    SP = 1,
    LR = 2,
    TR = 3,
    SR = 4,
    IR = 5,
    IS = 6,
    RESERVED_1 = 7,
    T0 = 8,
    T1 = 9,
    T2 = 10,
    T3 = 11,
    T4 = 12,
    T5 = 13,
    T6 = 14,
    T7 = 15,
    REG_ERROR=32,
    REG_NOT_USED = 33
} tim_register;

//! Encodes the size in bytes of an instruction in memory.
typedef unsigned char tim_instruction_size;

//! Encodes all of the valid instruction opcodes and their bit values.
typedef enum tim_instruction_opcode_e {
    LOADR , //!< Load to register X from address in register Y with offset in register Z.
    LOADI , //!< Load to register X from address in register Y with immediate offset.
    STORI , //!< Store register X to address in register Y with offset in register Z.
    STORR , //!< Store register X to address in register Y with immediate offset.
    PUSH  , //!< Push register X onto the top of the stack and decrement the stack pointer.
    POP   , //!< Pop element at top of stack into register X and increment the stack pointer.
    MOVR  , //!< Move the content of register X into register Y
    MOVI  , //!< Move immediate I into register X
    MOVSR , //!< Move content of special register X into general register Y
    MOVRS , //!< Move content of general register X into special register Y
    JUMPR , //!< Jump to address contained within register X
    JUMPI , //!< Jump to address contained within instruction immediate.
    CALLR , //!< Call to function who's address is contained within register X
    CALLI , //!< Call to function who's address is contained within instruction immediate.
    RETURN, //!< Return from the last function call.
    TEST  , //!< Test two general or special registers and set comparison bits.
    HALT  , //!< Stop processing and wait to be reset.
    ANDR  , //!< Bitwise AND two registers together.
    NANDR , //!< Bitwise NAND two registers together.
    ORR   , //!< Bitwise OR two registers together.
    NORR  , //!< Bitwise NOR two registers together.
    XORR  , //!< Bitwise XOR two registers together.
    LSLR  , //!< Logical shift left the bits in register X by the value in register Y.
    LSRR  , //!< Logical shift right the bits in register X by the value in register Y.
    NOTR  , //!< Bitwise invert the specificed register.
    ANDI  , //!< Bitwise AND two registers together.
    NANDI , //!< Bitwise NAND two registers together.
    ORI   , //!< Bitwise OR two registers together.
    NORI  , //!< Bitwise NOR two registers together.
    XORI  , //!< Bitwise XOR two registers together.
    LSLI  , //!< Logical shift left the bits in register X by the immediate value
    LSRI  , //!< Logical shift right the bits in register X by the immediate value
    IADDI , //!< Integer Add register X to immediate value.
    ISUBI , //!< Integer Subtract immediate value from register X.
    IMULI , //!< Integer Multiply register X by immediate value.
    IDIVI , //!< Integer Divide register X by immediate value.
    IALSI , //!< Integer Arithmetic shift register X left by immediate value.
    IASRI , //!< Integer Arithmetic shift register X right immediate value.
    IADDR , //!< Integer Add register X to register Y.
    ISUBR , //!< Integer Subtract register X from register Y.
    IMULR , //!< Integer Multiply register X by register Y.
    IDIVR , //!< Integer Divide register X by register Y.
    IASLR , //!< Integer Arithmetic shift register X left by value in register Y.
    IASRR , //!< Integer Arithmetic shift register X right value in register Y.
    FADDI , //!< Floating point Add register X to immediate value.
    FSUBI , //!< Floating point Subtract immediate value from register X.
    FMULI , //!< Floating point Multiply register X by immediate value.
    FDIVI , //!< Floating point Divide register X by immediate value.
    FASLI , //!< Floating point Arithmetic shift register X left by immediate value.
    FASRI , //!< Floating point Arithmetic shift register X right immediate value.
    FADDR , //!< Floating point Add register X to register Y.
    FSUBR , //!< Floating point Subtract register X from register Y.
    FMULR , //!< Floating point Multiply register X by register Y.
    FDIVR , //!< Floating point Divide register X by register Y.
    FASLR , //!< Floating point Arithmetic shift register X left by value in register Y.
    FASRR   //!< Floating point Arithmetic shift register X right value in register Y.
} tim_instruction_opcode;

/*!
@brief Encodes all information on a single tim instruction.
*/
typedef struct tim_instruction_t {
    
    //! The opcode of the instruction.
    tim_instruction_opcode opcode;

    //! The number of bytes the instruction occupies in memory.
    tim_instruction_size   size;

} tim_instruction;

//! Typdef for storing the string forms of instructions encountered in assembly programs.
typedef char tim_asm_instruction;

extern tim_asm_instruction * tim_LOAD  ;
extern tim_asm_instruction * tim_STORE ; 
extern tim_asm_instruction * tim_PUSH  ; 
extern tim_asm_instruction * tim_POP   ; 
extern tim_asm_instruction * tim_MOV   ; 
extern tim_asm_instruction * tim_JUMP  ; 
extern tim_asm_instruction * tim_CALL  ; 
extern tim_asm_instruction * tim_RETURN; 
extern tim_asm_instruction * tim_TEST  ; 
extern tim_asm_instruction * tim_HALT  ; 
extern tim_asm_instruction * tim_AND   ; 
extern tim_asm_instruction * tim_NAND  ; 
extern tim_asm_instruction * tim_OR    ; 
extern tim_asm_instruction * tim_NOR   ; 
extern tim_asm_instruction * tim_XOR   ; 
extern tim_asm_instruction * tim_LSL   ; 
extern tim_asm_instruction * tim_LSR   ; 
extern tim_asm_instruction * tim_NOT   ; 
extern tim_asm_instruction * tim_IADD  ; 
extern tim_asm_instruction * tim_ISUB  ; 
extern tim_asm_instruction * tim_IMUL  ; 
extern tim_asm_instruction * tim_IDIV  ; 
extern tim_asm_instruction * tim_IASR  ; 
extern tim_asm_instruction * tim_FADD  ; 
extern tim_asm_instruction * tim_FSUB  ; 
extern tim_asm_instruction * tim_FMUL  ; 
extern tim_asm_instruction * tim_FDIV  ; 
extern tim_asm_instruction * tim_FASR  ; 
extern tim_asm_instruction * tim_NOP   ; 
extern tim_asm_instruction * tim_SLEEP ; 
extern tim_asm_instruction * tim_DATA  ; 

#endif

//! }@
