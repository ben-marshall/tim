/*!
@ingroup sw-asm
@{
@file asm_lex.c
@brief Contains all functions for turning an input text file into a token stream.
*/

#include "asm.h"
#include "asm_lex.h"

/*!
@brief reads and returns the rest of the current line in the file from the current cursor
position.
@param [in] source - The opened source code file.
@returns The read line or NULL if the end of the file is reached.
*/
char * asm_lex_file_readline(FILE * source)
{
    long int line_start = ftell(source);
    int character = fgetc(source);
    char breakout = 0;
    char * line = NULL;

    while(character != '\n' && feof(source) == 0 && character != '\r')
        character = fgetc(source);

    long int line_end = ftell(source);
    fgetc(source);

    if(feof(source))
        breakout = 1;

    int line_length = line_end-line_start;
    fseek(source, line_start, SEEK_SET);
    line = calloc(line_length + 1, sizeof(char));

    int i = 0;
    for(i =0; i < line_length; i++)
        line[i] = (char)fgetc(source);

    line[line_length] = '\0';
    
    while(character == '\n' && feof(source) != 0 && character == '\r')
        fgetc(source);

    if(breakout)
        return NULL;
    else
        return line;
}

/*!
@brief Takes a three letter string and returns either an assembly register code or NO_REG if the
character code is invalid.
@param str - Pointer to the head of a string to check.
@returns an asm_register representing the passed two letter code. If the input code is invalid then
REG__ERROR is returned.
*/
tim_register asm_lex_register(char * str, int * errors, unsigned int line_number)
{

    if (str == NULL)
    {
        *errors +=1;
        error("Line %d: NULL string passed to register lexer.\n", line_number);
        return REG_ERROR;
    }
    else if(strcmp(str, "$R0")==0) return R0;
    else if(strcmp(str, "$R1")==0) return R1;
    else if(strcmp(str, "$R2")==0) return R2;
    else if(strcmp(str, "$R3")==0) return R3;
    else if(strcmp(str, "$R4")==0) return R4;
    else if(strcmp(str, "$R5")==0) return R5;
    else if(strcmp(str, "$R6")==0) return R6;
    else if(strcmp(str, "$R7")==0) return R7;
    else if(strcmp(str, "$R8")==0) return R8;
    else if(strcmp(str, "$R9")==0) return R9;
    else if(strcmp(str, "$R10")==0) return R10;
    else if(strcmp(str, "$R11")==0) return R11;
    else if(strcmp(str, "$R12")==0) return R12;
    else if(strcmp(str, "$R13")==0) return R13;
    else if(strcmp(str, "$R14")==0) return R14;
    else if(strcmp(str, "$R15")==0) return R15;
    else if(strcmp(str, "$PC")==0) return PC;
    else if(strcmp(str, "$SP")==0) return SP;
    else if(strcmp(str, "$LR")==0) return LR;
    else if(strcmp(str, "$TR")==0) return TR;
    else if(strcmp(str, "$SR")==0) return SR;
    else if(strcmp(str, "$IR")==0) return IR;
    else if(strcmp(str, "$IS")==0) return IS;
    else if(strcmp(str, "$T0")==0) return T0;
    else if(strcmp(str, "$T1")==0) return T1;
    else if(strcmp(str, "$T2")==0) return T2;
    else if(strcmp(str, "$T3")==0) return T3;
    else if(strcmp(str, "$T4")==0) return T4;
    else if(strcmp(str, "$T5")==0) return T5;
    else if(strcmp(str, "$T6")==0) return T6;
    else if(strcmp(str, "$T7")==0) return T7;
    else 
    {
        *errors +=1;
        error("Line %d: Could not parse register '%s'\n", line_number, str);
        return REG_ERROR;
    }
}


/*!
@brief Parses a character array into an asm_immediate and returns it as a 32 bit integer.
@note The value returned is not nessecerily correct. It simply has the right
bits set to represent the immediate in memory.
@param [in] immediate - The character representation of the immediate value.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns A tim_immediate representing the passed char array.
*/
tim_immediate asm_lex_immediate(char * immediate, int * errors, int line_num)
{
    switch(immediate[1])
    {
        case 'b':
            return (int)strtol(&immediate[1], NULL,  2);
        case 'd':
            return (int)strtol(&immediate[1], NULL, 10);
        case 'x':
            return (int)strtol(&immediate[1], NULL, 16);
        default:
            error("Could not parse immediate '%s' on line %d\n", immediate, line_num);
            *errors += 1;
            break;
    }

    return 0;
}

/*!
@brief parses a string instruction into its token typed declaration.
@param instruction - head of the string containing the token
@param errors - Pointer to an error counter.
@param line_num - The line number of the token being parsed. Helps with error reporting.
@returns A parsed asm_lex_opcode or LEX_ERROR if no such opcode exists.
*/
asm_lex_opcode asm_lex_instruction(char * instruction, int * errors, int line_num)
{
         if(strcmp(lex_tok_LOAD  , "LOAD"  ) == 0) return LEX_LOAD ;   
    else if(strcmp(lex_tok_STORE , "STORE" ) == 0) return LEX_STORE; 
    else if(strcmp(lex_tok_PUSH  , "PUSH"  ) == 0) return LEX_PUSH ; 
    else if(strcmp(lex_tok_POP   , "POP"   ) == 0) return LEX_POP  ; 
    else if(strcmp(lex_tok_MOV   , "MOV"   ) == 0) return LEX_MOV  ; 
    else if(strcmp(lex_tok_JUMP  , "JUMP"  ) == 0) return LEX_JUMP ; 
    else if(strcmp(lex_tok_CALL  , "CALL"  ) == 0) return LEX_CALL ; 
    else if(strcmp(lex_tok_RETURN, "RETURN") == 0) return LEX_RETURN; 
    else if(strcmp(lex_tok_TEST  , "TEST"  ) == 0) return LEX_TEST ;  
    else if(strcmp(lex_tok_HALT  , "HALT"  ) == 0) return LEX_HALT ; 
    else if(strcmp(lex_tok_AND   , "AND"   ) == 0) return LEX_AND  ; 
    else if(strcmp(lex_tok_NAND  , "NAND"  ) == 0) return LEX_NAND ; 
    else if(strcmp(lex_tok_OR    , "OR"    ) == 0) return LEX_OR   ; 
    else if(strcmp(lex_tok_NOR   , "NOR"   ) == 0) return LEX_NOR  ;  
    else if(strcmp(lex_tok_XOR   , "XOR"   ) == 0) return LEX_XOR  ;  
    else if(strcmp(lex_tok_LSL   , "LSL"   ) == 0) return LEX_LSL  ; 
    else if(strcmp(lex_tok_LSR   , "LSR"   ) == 0) return LEX_LSR  ;  
    else if(strcmp(lex_tok_NOT   , "NOT"   ) == 0) return LEX_NOT  ;  
    else if(strcmp(lex_tok_IADD  , "IADD"  ) == 0) return LEX_IADD ;  
    else if(strcmp(lex_tok_ISUB  , "ISUB"  ) == 0) return LEX_ISUB ; 
    else if(strcmp(lex_tok_IMUL  , "IMUL"  ) == 0) return LEX_IMUL ; 
    else if(strcmp(lex_tok_IDIV  , "IDIV"  ) == 0) return LEX_IDIV ;
    else if(strcmp(lex_tok_IASR  , "IASR"  ) == 0) return LEX_IASR ; 
    else if(strcmp(lex_tok_FADD  , "FADD"  ) == 0) return LEX_FADD ;  
    else if(strcmp(lex_tok_FSUB  , "FSUB"  ) == 0) return LEX_FSUB ; 
    else if(strcmp(lex_tok_FMUL  , "FMUL"  ) == 0) return LEX_FMUL ; 
    else if(strcmp(lex_tok_FDIV  , "FDIV"  ) == 0) return LEX_FDIV ;
    else if(strcmp(lex_tok_FASR  , "FASR"  ) == 0) return LEX_FASR ;  
    else if(strcmp(lex_tok_NOP   , "NOP"   ) == 0) return LEX_NOP  ; 
    else if(strcmp(lex_tok_SLEEP , "SLEEP" ) == 0) return LEX_SLEEP; 
    else if(strcmp(lex_tok_DATA  , "DATA"  ) == 0) return LEX_DATA ; 
    else return LEX_ERROR;
}

/*!
@brief Parses an entire input file into a single lexical token stream.
@param input - The input file with the seeker at the beginning of the file.
@param errors - pointer to an error counter.
@returns The head of a linked list of lexer tokens representing the parsed file.
*/
asm_lex_token *  asm_lex_input_file(FILE * input, int * errors)
{
    asm_lex_token * to_return = NULL;
    asm_lex_token * walker    = NULL;

    char * current_line = asm_lex_file_readline(input);
    unsigned int line_number = 0;

    while(current_line != NULL && feof(input) == 0)
    {
        char * token = strtok(current_line, "\r\n ");

        while (token != NULL)
        {
            char skip = 0;
            asm_lex_token * to_add = calloc(1, sizeof(asm_lex_token));
            to_add -> line_number = line_number;

            if(token[0] == ';')
            {
                // It is a comment, so skip the rest of this line.
                free(to_add);
                skip = 1;
                break;
            }
            else if(token[0] == '$')
            {
                // It is a register!
                to_add -> type = REGISTER;
                to_add -> value.reg = asm_lex_register(token, errors, line_number);
            }
            else if(token[0] == '0')
            {
                // It is an immediate.
                to_add -> type = IMMEDIATE;
                to_add -> value.immediate = asm_lex_immediate(token, errors, line_number);
            }
            else if(token[0] == '.')
            {
                // It is a label
                to_add -> type = LABEL;
                to_add -> value.label = token;
            }
            else
            {
                // Assume it is an instruction!
                to_add -> type = OPCODE;
                to_add -> value.opcode = asm_lex_instruction(token, errors, line_number);

                if(to_add -> value.opcode == LEX_ERROR)
                {
                    // we don't know what it is so output an error.
                    error("Line %d: Could not determine token type of '%s'\n", line_number, token);
                    *errors += 1;
                    free(to_add);
                    skip = 1;
                }
            }
        
            // If it was a valid token then add it to the list.
            if(skip == 0)
            {
                if(to_return == NULL)
                {
                    to_return = to_add;
                    walker    = to_add;
                }
                else
                {
                    walker -> next = to_add;
                    walker         = to_add;
                }
            }


            token = strtok(NULL, "\r\n ");
        }

        line_number ++;
        free(current_line);
        current_line = asm_lex_file_readline(input);
    }

    return to_return;
}
