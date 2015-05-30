/*!
@ingroup sw-asm
@{
@file asm_parse.c
@brief Contains all functions relating to parsing the source code into IR data structure.
*/

#include "asm.h"


asm_lex_token * asm_parse_opcode(asm_statement * statment, asm_lex_token * token, int * errors)
{
    assert(token -> type == OPCODE);

    switch(token -> value.opcode)
    {
        default:
            error("Unknown asm opcode: %d\n", token -> value.opcode);
            break;
    }

    return token -> next;
}

asm_lex_token * asm_parse_label_declaration(asm_lex_token * token, asm_hash_table * labels, int * errors)
{
    return token -> next;
}


/*!
@brief Top function to trigger the parsing of an input source file.
@details Takes an opened for reading text file and parses it into a series of asm statements,
filling out their arguments and parameters as it goes. It also populates the hash-table of
labels used for calculating jump target addresses.
@see The ISA Specification contains more information on the grammar of the assembly language.
@param [inout] labels - Hashtable which is apopulated with any encountered labels.
@param [inout] errors - Pointer to a error counter. If the counter has the same value before
and after being called, all of the parsing was a success.
@returns The parsed statements as a doublely linked list.
*/
asm_statement * asm_parse_token_stream(asm_lex_token * tokens, asm_hash_table * labels, int * errors)
{
    asm_statement * to_return = NULL;
    asm_lex_token * current_token = tokens;

    // Iterate over all of the tokens in the stream.
    while(current_token != NULL)
    {
        asm_statement * to_add = calloc(1, sizeof(asm_statement));

        switch(current_token -> type)
        {
            case (OPCODE):
                current_token = asm_parse_opcode(to_add, current_token, errors);
                break;

            case (LABEL):
                current_token = asm_parse_label_declaration(current_token, labels, errors);
                break;

            default:
                error("Unexpected token type: %d\n", current_token -> type);
                *errors += 1;
                break;
        }

        current_token = current_token -> next;
    }

    return to_return;
}
