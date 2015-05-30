/*!
@ingroup sw-asm
@{
@file asm_parse.c
@brief Contains all functions relating to parsing the source code into IR data structure.
*/

#include "asm.h"

/*!
@brief Responsible for parsing Jump and call instructions.
@param [inout] statement - Resulting statment to set members of.
@param [in] token - The token which to parse into a statement. Several subsequent tokens may also
be eaten.
@param errors - Error counter pointer.
@returns The next token that should be parsed, i.e. the one following the last token eaten by this
function.
*/
asm_lex_token * asm_parse_call_jump(asm_statement * statement, asm_lex_token * token, int * errors)
{
    asm_lex_token * opcode  = token;
    asm_lex_token * operand = opcode -> next;

    log("Parsing opcode immediate\n");

    if(opcode -> value.opcode == LEX_JUMP)
    {
        if(operand -> type == REGISTER)
        {
            statement -> opcode = JUMPR;
            statement -> args.reg.reg_1 = operand -> value.reg;
            statement -> size = 2;
        }
        else if(operand -> type == IMMEDIATE)
        {
            statement -> opcode = JUMPI;
            statement -> args.immediate.immidiate = operand -> value.immediate;
            statement -> size = 4;
        }
        else if(operand -> type == LABEL)
        {
            statement -> opcode = JUMPI;
            statement -> args.immediate_label.label = operand -> value.label;
            statement -> size = 4;
        }
        else
        {
            error("Invalid operand to JUMP instruction of type %d\n", operand -> type);
            *errors+=1;
        }
    }
    else if(opcode -> value.opcode == LEX_CALL)
    {
        if(operand -> type == REGISTER)
        {
            statement -> opcode = CALLR;
            statement -> args.reg.reg_1 = operand -> value.reg;
            statement -> size = 2;
        }
        else if(operand -> type == IMMEDIATE)
        {
            statement -> opcode = CALLI;
            statement -> args.immediate.immidiate = operand -> value.immediate;
            statement -> size = 4;
        }
        else if(operand -> type == LABEL)
        {
            statement -> opcode = CALLI;
            statement -> args.immediate_label.label = operand -> value.label;
            statement -> size = 4;
        }
        else
        {
            error("Invalid operand to CALL instruction of type %d\n", operand -> type);
            *errors+=1;
        }
    }
    else
    {
        error("This function can only parse CALL and JUMP instructions!\n");
        *errors+=1;
    }

    return operand -> next;
}

/*!
@brief Responsible for selecting which function should parse the next few tokens.:w
@param token - The token containing the  label value
@param statement - The statement to parse the tokens into.
@param errors - Pointer to an error counter.
@returns the next token to be parsed.
*/
asm_lex_token * asm_parse_opcode(asm_statement * statement, asm_lex_token * token, int * errors)
{
    assert(token -> type == OPCODE);

    switch(token -> value.opcode)
    {
        case(LEX_JUMP):
        case(LEX_CALL):
            return asm_parse_call_jump(statement, token, errors);

        case(LEX_ERROR):
            error("Bad Token!\n");
            return token -> next;

        default:
            error("Unknown asm opcode: %d\n", token -> value.opcode);
            return token -> next;
    }
}


/*!
@brief Responsible for adding a label declaration and associated statement to the symbol table.
@param token - The token containing the  label value
@param labels - The symbol table.
@param errors - Pointer to an error counter.
@returns the next token to be parsed.
*/
asm_lex_token * asm_parse_label_declaration(asm_lex_token * token, asm_hash_table * labels, int * errors)
{
    assert(token -> type == LABEL);
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
@param tokens - Linked list of tokens to parse into a program IR.
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
                current_token = current_token -> next;
                break;
        }
    }

    return to_return;
}
