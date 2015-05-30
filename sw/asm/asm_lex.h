
/*!
@ingroup sw-asm
@{
@file asm_lex.h
@brief Header file for data types and functions used by the asm code lexer.
*/


#ifndef ASM_LEX_H
#define ASM_LEX_H

#define lex_tok_LOAD    "LOAD"
#define lex_tok_STORE   "STORE" 
#define lex_tok_PUSH    "PUSH" 
#define lex_tok_POP     "POP" 
#define lex_tok_MOV     "MOV" 
#define lex_tok_JUMP    "JUMP" 
#define lex_tok_CALL    "CALL" 
#define lex_tok_RETURN  "RETURN" 
#define lex_tok_TEST    "TEST"
#define lex_tok_HALT    "HALT" 
#define lex_tok_AND     "AND"
#define lex_tok_NAND    "NAND" 
#define lex_tok_OR      "OR" 
#define lex_tok_NOR     "NOR" 
#define lex_tok_XOR     "XOR" 
#define lex_tok_LSL     "LSL" 
#define lex_tok_LSR     "LSR" 
#define lex_tok_NOT     "NOT" 
#define lex_tok_IADD    "IADD" 
#define lex_tok_ISUB    "ISUB" 
#define lex_tok_IMUL    "IMUL" 
#define lex_tok_IDIV    "IDIV" 
#define lex_tok_IASR    "IASR" 
#define lex_tok_FADD    "FADD" 
#define lex_tok_FSUB    "FSUB" 
#define lex_tok_FMUL    "FMUL" 
#define lex_tok_FDIV    "FDIV" 
#define lex_tok_FASR    "FASR" 
#define lex_tok_NOP     "NOP"
#define lex_tok_SLEEP   "SLEEP" 
#define lex_tok_DATA    "DATA" 


typedef enum asm_lex_opcode_e{
    LEX_LOAD ,   
    LEX_STORE, 
    LEX_PUSH , 
    LEX_POP  , 
    LEX_MOV  , 
    LEX_JUMP , 
    LEX_CALL , 
    LEX_RETURN,
    LEX_TEST , 
    LEX_HALT , 
    LEX_AND  , 
    LEX_NAND , 
    LEX_OR   , 
    LEX_NOR  , 
    LEX_XOR  , 
    LEX_LSL  , 
    LEX_LSR  , 
    LEX_NOT  , 
    LEX_IADD , 
    LEX_ISUB , 
    LEX_IMUL , 
    LEX_IDIV , 
    LEX_IASR , 
    LEX_FADD , 
    LEX_FSUB , 
    LEX_FMUL , 
    LEX_FDIV , 
    LEX_FASR , 
    LEX_NOP  , 
    LEX_SLEEP, 
    LEX_DATA , 
    LEX_ERROR
} asm_lex_opcode;

//! Type mask for a character array.
typedef char asm_lex_label;

//! Union to hold the correct value of an instruction.
typedef union asm_lex_token_value_e{
    tim_register     reg;
    tim_immediate    immediate;
    asm_lex_opcode   opcode;
    asm_lex_label  * label;
} asm_lex_token_value;

//! The type of token that a lexer token can be.
typedef enum asm_lex_token_type_e{
    OPCODE, 
    LABEL,
    REGISTER,
    IMMEDIATE
} asm_lex_token_type;


//! Typedef for the asm lexer token structure
typedef struct asm_lex_token_t asm_lex_token;
struct asm_lex_token_t{
    asm_lex_token_type    type;
    asm_lex_token_value   value;
    asm_lex_token       * next;
    unsigned int          line_number;
};

/*!
@brief Parses an entire input file into a single lexical token stream.
@param input - The input file with the seeker at the beginning of the file.
@param errors - pointer to an error counter.
@returns The head of a linked list of lexer tokens representing the parsed file.
*/
asm_lex_token *  asm_lex_input_file(FILE * input, int * errors);

#endif
