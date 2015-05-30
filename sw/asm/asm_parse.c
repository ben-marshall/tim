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
@brief Responsible for parsing instructions with three register operands.
@param [inout] statement - Resulting statment to set members of.
@param [in] token - The token which to parse into a statement. Several subsequent tokens may also
be eaten.
@param errors - Error counter pointer.
@returns The next token that should be parsed, i.e. the one following the last token eaten by this
function.
*/
asm_lex_token * asm_parse_three_register_operands(asm_statement * statement, asm_lex_token * token, int * errors)
{
    asm_lex_token * opcode    = token;
    asm_lex_token * operand_1 = opcode    -> next;
    asm_lex_token * operand_2 = operand_1 -> next;
    asm_lex_token * operand_3 = operand_2 -> next;
    
    assert(opcode -> type == OPCODE);
    assert(operand_1 -> type == REGISTER);
    assert(operand_2 -> type == REGISTER);
    assert(operand_3 -> type == REGISTER);

    statement -> args.reg_reg_reg.reg_1 = operand_1 -> value.reg;
    statement -> args.reg_reg_reg.reg_2 = operand_2 -> value.reg;
    statement -> args.reg_reg_reg.reg_3 = operand_3 -> value.reg;

    statement -> size = 3;
    
    switch(opcode -> value.opcode)
    {
        case(LEX_LOAD):  statement -> opcode = LOADR; break;
        case(LEX_STORE): statement -> opcode = STORR; break;
        case(LEX_AND ):  statement -> opcode = ANDR ; break;
        case(LEX_NAND):  statement -> opcode = NANDR; break;
        case(LEX_OR  ):  statement -> opcode = ORR  ; break;
        case(LEX_NOR ):  statement -> opcode = NORR ; break;
        case(LEX_XOR ):  statement -> opcode = XORR ; break;
        case(LEX_LSL ):  statement -> opcode = LSLR ; break;
        case(LEX_LSR ):  statement -> opcode = LSRR ; break;
        case(LEX_IADD):  statement -> opcode = IADDR; break;
        case(LEX_ISUB):  statement -> opcode = ISUBR; break;
        case(LEX_IMUL):  statement -> opcode = IMULR; break;
        case(LEX_IDIV):  statement -> opcode = IDIVR; break;
        case(LEX_IASR):  statement -> opcode = IASRR; break;
        case(LEX_FADD):  statement -> opcode = FADDR; break;
        case(LEX_FSUB):  statement -> opcode = FSUBR; break;
        case(LEX_FMUL):  statement -> opcode = FMULR; break;
        case(LEX_FDIV):  statement -> opcode = FDIVR; break;
        case(LEX_FASR):  statement -> opcode = FASRR; break;

        case(LEX_ERROR):
            error("Bad Token!\n");
            return operand_3 -> next;

        default:
            error("This function doesnt support parsing of asm opcode %d \n", opcode -> value.opcode);
            return operand_3 -> next;

    }

    return operand_3 -> next;
}

/*!
@brief Responsible for parsing instructions with two register operands and one immediate operand.
@param [inout] statement - Resulting statment to set members of.
@param [in] token - The token which to parse into a statement. Several subsequent tokens may also
be eaten.
@param errors - Error counter pointer.
@returns The next token that should be parsed, i.e. the one following the last token eaten by this
function.
*/
asm_lex_token * asm_parse_two_register_one_immediate(asm_statement * statement, asm_lex_token * token, int * errors)
{
    asm_lex_token * opcode    = token;
    asm_lex_token * operand_1 = opcode    -> next;
    asm_lex_token * operand_2 = operand_1 -> next;
    asm_lex_token * operand_3 = operand_2 -> next;
    
    assert(opcode -> type == OPCODE);
    assert(operand_1 -> type == REGISTER);
    assert(operand_2 -> type == REGISTER);
    assert(operand_3 -> type == IMMEDIATE);

    statement -> args.reg_reg_immediate.reg_1 = operand_1 -> value.reg;
    statement -> args.reg_reg_immediate.reg_2 = operand_2 -> value.reg;
    statement -> args.reg_reg_immediate.immidiate = operand_3 -> value.immediate;

    statement -> size = 3;
    
    switch(opcode -> value.opcode)
    {
        case(LEX_LOAD):  statement -> opcode = LOADI; break;
        case(LEX_STORE): statement -> opcode = STORI; break;
        case(LEX_AND ):  statement -> opcode = ANDI ; break;
        case(LEX_NAND):  statement -> opcode = NANDI; break;
        case(LEX_OR  ):  statement -> opcode = ORI  ; break;
        case(LEX_NOR ):  statement -> opcode = NORI ; break;
        case(LEX_XOR ):  statement -> opcode = XORI ; break;
        case(LEX_LSL ):  statement -> opcode = LSLI ; break;
        case(LEX_LSR ):  statement -> opcode = LSRI ; break;
        case(LEX_IADD):  statement -> opcode = IADDI; break;
        case(LEX_ISUB):  statement -> opcode = ISUBI; break;
        case(LEX_IMUL):  statement -> opcode = IMULI; break;
        case(LEX_IDIV):  statement -> opcode = IDIVI; break;
        case(LEX_IASR):  statement -> opcode = IASRI; break;
        case(LEX_FADD):  statement -> opcode = FADDI; break;
        case(LEX_FSUB):  statement -> opcode = FSUBI; break;
        case(LEX_FMUL):  statement -> opcode = FMULI; break;
        case(LEX_FDIV):  statement -> opcode = FDIVI; break;
        case(LEX_FASR):  statement -> opcode = FASRI; break;

        case(LEX_ERROR):
            error("Bad Token!\n");
            return operand_3 -> next;

        default:
            error("This function doesnt support parsing of asm opcode %d \n", opcode -> value.opcode);
            return operand_3 -> next;

    }

    return operand_3 -> next;
}


/*!
@brief Responsible for parsing instructions with three operands..
@param [inout] statement - Resulting statment to set members of.
@param [in] token - The token which to parse into a statement. Several subsequent tokens may also
be eaten.
@param errors - Error counter pointer.
@returns The next token that should be parsed, i.e. the one following the last token eaten by this
function.
*/
asm_lex_token * asm_parse_three_operand(asm_statement * statement, asm_lex_token * token, int * errors)
{
    asm_lex_token * opcode    = token;
    asm_lex_token * operand_1 = opcode    -> next;
    asm_lex_token * operand_2 = operand_1 -> next;
    asm_lex_token * operand_3 = operand_2 -> next;

    if(operand_3->type == IMMEDIATE)
    {
        return asm_parse_two_register_one_immediate(statement, opcode, errors);
    }
    else if(operand_3->type == REGISTER)
    {
        return asm_parse_three_register_operands(statement, opcode, errors);
    }
    else
    {
        error("Line %d: Expected immediate or register, but got token type %d\n", operand_3->line_number, operand_3->type);
        return operand_3 -> next;
    }

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
        
        case(LEX_LOAD):
        case(LEX_STORE):
        case(LEX_AND ): 
        case(LEX_NAND): 
        case(LEX_OR  ): 
        case(LEX_NOR ): 
        case(LEX_XOR ): 
        case(LEX_LSL ): 
        case(LEX_LSR ): 
        case(LEX_IADD): 
        case(LEX_ISUB): 
        case(LEX_IMUL): 
        case(LEX_IDIV): 
        case(LEX_IASR): 
        case(LEX_FADD): 
        case(LEX_FSUB): 
        case(LEX_FMUL): 
        case(LEX_FDIV): 
        case(LEX_FASR): 
            return asm_parse_three_operand(statement, token,errors);

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
