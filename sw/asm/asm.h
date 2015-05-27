/*!
@ingroup sw-asm
@{
@file asm.h
@brief Header file for data types and functions used by the assembler.
*/


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

#include "common.h"

#ifndef ASM_H
#define ASM_H

#ifdef TIM_PRINT_PROMPT
    #undef TIM_PRINT_PROMPT
#endif
#define TIM_PRINT_PROMPT "\e[1;36masm>\e[0m "

//! Stores what sort of statement a parsed instruction is.
typedef enum asm_statement_type_e {NOP, DATA, OPCODE, LABEL} asm_statement_type;

/*!
@brief Stores all information on a single ASM instruction.
@details Stores the opcode and arguments of an ASM instruction. This includes instructions that
are not actually emitted such as DATA instructions. It also contains the memory address of the
instruction for any label pointers too it.
*/
typedef struct asm_statement_t asm_statement;
struct asm_statement_t
{
    //! The instruction the statment refers too.
    tim_instruction instruction;

    //! Register Argument 1
    tim_register reg_1;
    //! Register Argument 2
    tim_register reg_2;
    //! Register Argument 3
    tim_register reg_3;

    //! Immediate operand to the instruction.
    tim_immediate immediate;

    //! Is this an opcode, data/NOP or label?
    asm_statement_type type;

    //! The address of the instruction in byte-aligned memory.
    unsigned int address;

    //! The next statement to be executed in the program.
    asm_statement * next;
    //! The previously executed statement in the program.
    asm_statement * prev;
};


//! Typedef masking an integer to be the asm hash table key type.
typedef int asm_hash_key;

//! Typedef for as asm hash table.
typedef struct asm_hash_table_bin_t asm_hash_table_bin;

//! Describes whether to output the parsed asm code as binary or ascii code.
typedef enum asm_format_e {BINARY, ASCII} asm_format;


/*!
@brief Contains a single bin in the hashtable.
*/
struct asm_hash_table_bin_t
{
    char used;
    //! The key of the data.
    char * key;
    //! Pointer to the data item inserted.
    void        * data;
    //! Pointer to the next data item in a linked list.
    asm_hash_table_bin * next;
} ;

/*!
@brief Keeps a key,value pairing of elements. It's a hash table folks.
*/
typedef struct asm_hash_table_t
{
    //! A running count of the number of elements in the hashtable.
    int         element_count;

    //! The current size of the hash tables internal data structure.
    int         current_size;

    //! Holds the internal data.
    asm_hash_table_bin * buckets;

} asm_hash_table;

/*!
@brief Contains all information for the program in a format that can be easily passed around.
*/
typedef struct asm_context_t
{
    //! The path of the input source file.
    char * input_file;
    //! The path of the output binary file.
    char * output_file;

    //! How should we output to the binary file? ASCII or bytes?
    asm_format format;
    
    //! The opened source file stream.
    FILE * source;
    //! THe opened output file stream.
    FILE * binary;

    //! The asm program in linked list form.
    asm_statement * statements;

    //! Stores all of the (label, asm_statement) pairs for the program where the labels are
    //! all of the jump target labels.
    asm_hash_table * symbol_table;

} asm_context;



/*!
@brief Top function to trigger the parsing of an input source file.
@details Takes an opened for reading text file and parses it into a series of asm statements,
filling out their arguments and parameters as it goes. It also populates the hash-table of
labels used for calculating jump target addresses.
@param [in] source - Opened source file pointer. Open in "r" mode.
@param [out] statements - Returned list of statements parsed from the input file.
@param [inout] labels - Hashtable which is populated with any encountered labels.
@returns an interger represeting the number of errors encountered in parsing. zero means
all went well. Otherwise the program contained syntax errors.
*/
int asm_parse_input(FILE * source, asm_statement * statements, asm_hash_table * labels);


/*!
@brief Inserts an element into the hash table associated with the provided key.
@param table - Pointer to the hash table to insert into.
@param key - The key to the data.
@param data - Pointer to the data the table will contain.
@returns a status code. Zero if it worked, otherwise some integer.
@todo Check that the key will fit!!
*/
int asm_hash_table_insert(asm_hash_table * table, char * key, void * data);

/*!
@brief Creates and returns a new pointer to a hash table.
@param initial_size - The initial size of the hash table's internal data structure.
@param tr - The newly initialised and returned hashtable. Memory space should already be declared.
*/
void asm_hash_table_new(int initial_size, asm_hash_table * tr);

#endif


//! }@
