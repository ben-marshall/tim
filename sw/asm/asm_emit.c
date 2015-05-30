/*!
@ingroup sw-asm
@{
@file asm_emit.h
@brief Code responsible for emitting code as binary or ascii codes.
*/

#include "asm.h"

/*!
@brief Responsible for writing all statements to the supplied file.
@param statements - Linked list of statements to emit binary code for.
@param file - The file to write the code too.
@param format - Whether to emit the code as raw bytes or ascii binary strings. This is used to
feed the VHDL testbenches.
@returns An integer representing the number of errors encountered, if any.
*/
int asm_emit_instructions(asm_statement * statements, FILE * file, asm_format format)
{
    int errors = 0;
    asm_statement * walker = statements;

    while(walker != NULL)
    {
        walker = walker -> next;
    }

    return errors;
}
