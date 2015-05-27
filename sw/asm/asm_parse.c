/*!
@ingroup sw-asm
@{
@file asm_parse.c
@brief Contains all functions relating to parsing the source code into IR data structure.
*/

#include "asm.h"

/*!
@brief reads and returns the rest of the current line in the file from the current cursor
position.
@param [in] source - The opened source code file.
@returns The read line or NULL if the end of the file is reached.
*/
char * asm_parse_readline(FILE * source)
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
@param str - A two letter code representing a register.
@returns an asm_register representing the passed two letter code. If the input code is invalid then
REG__ERROR is returned.
*/
tim_register asm_parse_register(char str[3])
{

    if (str == NULL) return REG_ERROR;
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
    else return REG_ERROR;
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
tim_immediate asm_parse_immediate(char * immediate, int * errors, int line_num)
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
@brief Validates the arguments/operands to a load or store instruction.
@returns true or false depending on whether the instructions operands are valid or not.
@todo implement this!
*/
BOOL asm_validate_load_store(asm_statement * instruction)
{
    return TRUE;
}


/*!
@brief Parses the arguments to a load instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
@todo Add operand validation.
*/
asm_statement * asm_parse_load(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));

    char * operand1 = strtok(NULL, " ");
    char * operand2 = strtok(NULL, " ");
    char * operand3 = strtok(NULL, " ");
    char * operand4 = strtok(NULL, " ");

    to_return -> reg_1 = asm_parse_register(operand1);
    tim_register reg2 = asm_parse_register(operand2);

    if(reg2 != REG_ERROR)
    {
        // Assume it is the 3 register version of load.
        to_return -> reg_2 = reg2;
        to_return -> reg_3 = asm_parse_register(operand3);
        to_return -> instruction.opcode = LOADR;
        to_return -> instruction.size = 3;

        if(operand4 != NULL)
        {
            warning("Bit masks for STORE instruction not yet implemented.");
        }
    }
    else
    {
        // Assume it is the register-immediate version of load.
        to_return -> reg_2 = REG_NOT_USED;
        to_return -> reg_3 = REG_NOT_USED;
        to_return -> immediate = asm_parse_immediate(operand2, errors, line_num);
        to_return -> instruction.opcode = LOADI;
        to_return -> instruction.size = 4;
    }

    BOOL valid = asm_validate_load_store(to_return);
    if(valid)
        return to_return;
    else
    {
        free(to_return);
        return NULL;
    }
}

/*!
@brief Parses the arguments to a store instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
@todo Add operand validation.
*/
asm_statement * asm_parse_store(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));

    char * operand1 = strtok(NULL, " ");
    char * operand2 = strtok(NULL, " ");
    char * operand3 = strtok(NULL, " ");
    char * operand4 = strtok(NULL, " ");

    to_return -> reg_1 = asm_parse_register(operand1);
    tim_register reg2 = asm_parse_register(operand2);

    if(reg2 != REG_ERROR)
    {
        // Assume it is the 3 register version of load.
        to_return -> reg_2 = reg2;
        to_return -> reg_3 = asm_parse_register(operand3);
        to_return -> instruction.opcode = STORR;
        to_return -> instruction.size = 3;

        if(operand4 != NULL)
        {
            warning("Bit masks for STORE instruction not yet implemented.");
        }
    }
    else
    {
        // Assume it is the register-immediate version of load.
        to_return -> reg_2 = REG_NOT_USED;
        to_return -> reg_3 = REG_NOT_USED;
        to_return -> immediate = asm_parse_immediate(operand2, errors, line_num);
        to_return -> instruction.opcode = STORI;
        to_return -> instruction.size = 4;
    }

    BOOL valid = asm_validate_load_store(to_return);
    if(valid)
        return to_return;
    else
    {
        free(to_return);
        return NULL;
    }
}


/*!
@brief Decodes the opcode string and calls the appropriate function to decode the arguments.
@param [in] opcode - The opcode as a character string.
@param [in] arguments - The string containing the arguments to the opcode. This may be empty for
some instructions.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns The parsed asm statement with all arguments and fields filled out correctly.
@note An asm_statement will be returned even if errors are encountered. If by the time parsing
is complete the errors counter is greater than zero then the program should be considered invalid.
*/
asm_statement * asm_parse_instruction(char * opcode, char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = NULL;

    if(strcmp(opcode, tim_LOAD) == 0)
        to_return = asm_parse_load(arguments, errors, line_num);
    else if(strcmp(opcode, tim_STORE) == 0)
        to_return = asm_parse_store(arguments, errors, line_num);
    else
    {
        *errors ++;
        warning("Encountered invalid instruction '%s' on line %d.\n", opcode, line_num);
    }

    return to_return;
}

/*!
@brief Responsible for parsing a single line from the asm source code file.
@param [in] line - pointer to the start of the line to parse.
@param [inout] labels - The symbol table of labels so label declarations can be added.
@param [inout] errors - Pointer to an error counter that is incremented when incorrect syntax
is observed.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns a pointer to a new asm_statement if the line contains an instruction, or
NULL if it is a comment or just a label declaration.
@note A single line can contain a label declaration followed by an instruction, hence
this function may recurse *once*.
@bug Cannot place label declaration and instruction on same line because of strtok wierdness.
*/
asm_statement * asm_parse_line(char * line, asm_hash_table * labels, int * errors, int line_num)
{
        // Parse the line content here.
        char * token1 = strtok(line, " \r\n");

        asm_statement * to_return = NULL;

        if(token1 == NULL)
            return to_return;

        switch(token1[0])
        {
            case '\r':
            case '\n':
            case ';':
                // We found a comment line! Skip it.
                break;
            
            case '.':
                // We found a label declaration. Add it to the symbol table.
                log("Found Label: '%s' on line %d\n", token1, line_num);
                to_return = calloc(1, sizeof(asm_statement));
                to_return -> type = LABEL;
                asm_hash_table_insert(labels, token1, to_return);
                // Recurse once to check for a remaining instruction.
                //to_return -> next = asm_parse_line(line, labels, errors, line_num);
                break;

            default:
                // Asssume it is an instruction and parse it.
                to_return = asm_parse_instruction(token1, line, errors, line_num);
                break;
        }

        return to_return;
}


/*!
@brief Top function to trigger the parsing of an input source file.
@details Takes an opened for reading text file and parses it into a series of asm statements,
filling out their arguments and parameters as it goes. It also populates the hash-table of
labels used for calculating jump target addresses.
@see The ISA Specification contains more information on the grammar of the assembly language.
@param [in] source - Opened source file pointer. Open in "r" mode.
@param [out] statements - Returned list of statements parsed from the input file.
@param [inout] labels - Hashtable which is populated with any encountered labels.
@returns an interger represeting the number of errors encountered in parsing. zero means
all went well. Otherwise the program contained syntax errors.
*/
int asm_parse_input(FILE * source, asm_statement * statements, asm_hash_table * labels)
{
    int errors = 0;
    int lines_read = 1;
    char * line = NULL;

    asm_statement * walker = statements;

    while(feof(source) == 0)
    {
        line = asm_parse_readline(source);
        if(line == NULL) break;

        lines_read ++;
        fflush(stdout);
        asm_statement * s = asm_parse_line(line, labels, &errors, lines_read);

        if(s != NULL)
        {
            if(walker == NULL)
                walker = s;
            else
            {
                walker -> next = s;
                s      -> prev = walker;
                walker  = s;
            }
        }

        free(line);
    }

    return errors;
}
