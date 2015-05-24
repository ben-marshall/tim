/*!
@file asm.h
@brief Header file for data types and functions used by the assembler.
*/


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

#ifndef COMMON_H
#define COMMON_H

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

    //! Stores all of the (label, asm_statement) pairs for the program where the labels are
    //! all of the jump target labels.
    asm_hash_table * symbol_table;

} asm_context;


#endif
