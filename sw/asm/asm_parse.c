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
tim_register asm_parse_register(char str[4])
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
@param instruction - The instruction to be validated.
@returns true or false depending on whether the instructions operands are valid or not.
*/
BOOL asm_validate_load_store(asm_statement * instruction)
{
    BOOL tr = TRUE;

    if(instruction -> instruction.opcode == LOADR || instruction -> instruction.opcode == STORR)
    {
        if( tim_is_general_register(instruction -> reg_1) == FALSE ||
            tim_is_general_register(instruction -> reg_2) == FALSE ||
            tim_is_general_register(instruction -> reg_3) == FALSE)
        {
            error("Line %d: All Register Operands of LOAD/STORE should be general purpose register.\n", instruction->line);
            log  ("\t Arguments are: %d, %d, %d\n", instruction->reg_1, instruction->reg_2, instruction->reg_3);
            tr = FALSE;
        }
    }
    else if(instruction -> instruction.opcode == LOADI || instruction -> instruction.opcode == STORI)
    {
        if( tim_is_general_register(instruction -> reg_1) == FALSE)
        {
            error("Line %d: All Register Operands of LOAD/STORE should be general purpose register.\n", instruction->line);
            log  ("\t Arguments are: %d\n", instruction->reg_1)
            tr = FALSE;
        }
    }
    else
        error("Invalid instruction validated by %s on line %d\n", __FUNCTION__, instruction->line);

    return tr;
}


/*!
@brief Parses the arguments to a load instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
*/
asm_statement * asm_parse_load(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;

    char * operand1 = strtok(NULL, " ");
    char * operand2 = strtok(NULL, " ");
    char * operand3 = strtok(NULL, " \r\n");
    char * operand4 = strtok(NULL, " \r\n");

    to_return -> type = OPCODE;
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
*/
asm_statement * asm_parse_store(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;

    char * operand1 = strtok(NULL, " ");
    char * operand2 = strtok(NULL, " ");
    char * operand3 = strtok(NULL, " \r\n");
    char * operand4 = strtok(NULL, " \r\n");

    to_return -> type = OPCODE;
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
@brief Validates the arguments/operands to a MOV instruction.
@param instruction - The instruction to be validated.
@returns true or false depending on whether the instructions operands are valid or not.
*/
BOOL asm_validate_mov(asm_statement * instruction)
{
    BOOL to_return = TRUE;

    if(instruction -> reg_1 == PC)
    {
        error("Line %d : MOV instruction cannot modify the program counter\n", instruction -> line);
        to_return = FALSE;
    }

    instruction -> reg_3 = REG_NOT_USED;
    return to_return;
}

/*!
@brief Parses the arguments of a move instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
*/
asm_statement * asm_parse_mov(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;

    char * operand1 = strtok(NULL, " ");
    char * operand2 = strtok(NULL, " \r\n");

    to_return -> type = OPCODE;
    to_return -> reg_1 = asm_parse_register(operand1);
    tim_register reg2  = asm_parse_register(operand2);

    if(reg2 != REG_ERROR)
    {
        // Assume it is the 2 register version of mov.
        to_return -> reg_2 = reg2;
        to_return -> instruction.opcode = MOVR;
        to_return -> instruction.size   = 3;
    }
    else
    {
        // Assume it is the register-immediate version of mov
        to_return -> immediate = asm_parse_immediate(operand2, errors, line_num);
        to_return -> instruction.opcode = MOVI;
        to_return -> instruction.size   = 4;
    }

    BOOL valid = asm_validate_mov(to_return);
    if(valid)
        return to_return;
    else
    {
        free(to_return);
        return NULL;
    }
}


/*!
@brief Validates the arguments/operands to a PUSH instruction.
@param instruction - The instruction to be validated.
@returns true or false depending on whether the instructions operands are valid or not.
*/
BOOL asm_validate_push(asm_statement * instruction)
{
    instruction -> reg_2 = REG_NOT_USED;
    instruction -> reg_3 = REG_NOT_USED;
    return TRUE;
}

/*!
@brief Parses the arguments of a push instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
*/
asm_statement * asm_parse_push(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;

    char * operand1 = strtok(NULL, " ");
    
    to_return -> type = OPCODE;
    to_return -> reg_1 = asm_parse_register(operand1);
    to_return -> reg_2 = REG_NOT_USED;
    to_return -> reg_3 = REG_NOT_USED;

    to_return -> instruction.opcode = PUSH;
    to_return -> instruction.size   = 2;

    BOOL valid = asm_validate_push(to_return);
    if(valid)
        return to_return;
    else
    {
        free(to_return);
        return NULL;
    }
}


/*!
@brief Validates the arguments/operands to a POP instruction.
@param instruction - The instruction to be validated.
@returns true or false depending on whether the instructions operands are valid or not.
@todo implement this!
*/
BOOL asm_validate_pop(asm_statement * instruction)
{
    instruction -> reg_2 = REG_NOT_USED;
    instruction -> reg_3 = REG_NOT_USED;
    return TRUE;
}

/*!
@brief Parses the arguments of a pop instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
@todo Add operand validation.
*/
asm_statement * asm_parse_pop(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;

    char * operand1 = strtok(NULL, " ");
    
    to_return -> type = OPCODE;
    to_return -> reg_1 = asm_parse_register(operand1);
    to_return -> reg_2 = REG_NOT_USED;
    to_return -> reg_3 = REG_NOT_USED;

    to_return -> instruction.opcode = POP;
    to_return -> instruction.size   = 2;

    BOOL valid = asm_validate_pop(to_return);
    if(valid)
        return to_return;
    else
    {
        free(to_return);
        return NULL;
    }
}


/*!
@brief Validates the arguments/operands to a JUMP instruction.
@param instruction - The instruction to be validated.
@returns true or false depending on whether the instructions operands are valid or not.
@todo implement this!
*/
BOOL asm_validate_jump(asm_statement * instruction)
{
    if(instruction -> reg_1 == PC)
    {
        warning("Line %d : Jumping to the program counter can cause an infinite loop!\n", instruction -> line);
    }
    return TRUE;
}

/*!
@brief Parses the arguments of a JUMP instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
@todo Add operand validation.
*/
asm_statement * asm_parse_jump(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;

    char * operand1 = strtok(NULL, " ");
    
    to_return -> type = OPCODE;
    to_return -> reg_2 = REG_NOT_USED;
    to_return -> reg_3 = REG_NOT_USED;
    to_return -> reg_1 = asm_parse_register(operand1);

    if(to_return -> reg_1 != REG_ERROR)
    {
        // Assume we are jumping to the contents of a register.
        to_return -> instruction.opcode = JUMPR;
        to_return -> instruction.size   = 2;
    }
    else
    {
        // Immediate version of the instruction.
        to_return -> reg_1 = REG_NOT_USED;
        to_return -> immediate = asm_parse_immediate(operand1, errors, line_num);
        to_return -> instruction.opcode = JUMPI;
        to_return -> instruction.size   = 4;
    }

    BOOL valid = asm_validate_jump(to_return);
    if(valid)
        return to_return;
    else
    {
        free(to_return);
        return NULL;
    }
}


/*!
@brief Validates the arguments/operands to a CALL instruction.
@param instruction - The instruction to be validated.
@returns true or false depending on whether the instructions operands are valid or not.
@todo implement this!
*/
BOOL asm_validate_call(asm_statement * instruction)
{
    if(instruction -> reg_1 == PC)
    {
        warning("Line %d : Jumping to the program counter can cause an infinite loop!\n", instruction -> line);
    }
    return TRUE;
}

/*!
@brief Parses the arguments of a CALL instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
@todo Add operand validation.
*/
asm_statement * asm_parse_call(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;

    char * operand1 = strtok(NULL, " ");
    
    to_return -> type = OPCODE;
    to_return -> reg_2 = REG_NOT_USED;
    to_return -> reg_3 = REG_NOT_USED;
    to_return -> reg_1 = asm_parse_register(operand1);

    if(to_return -> reg_1 != REG_ERROR)
    {
        // Assume we are jumping to the contents of a register.
        to_return -> instruction.opcode = CALLR;
        to_return -> instruction.size   = 2;
    }
    else
    {
        // Immediate version of the instruction.
        to_return -> reg_1 = REG_NOT_USED;
        if(operand1[0] == '0')
        {
            to_return -> immediate = asm_parse_immediate(operand1, errors, line_num);
        }
        else
        {
            // The target address is a label! It will need calculating later.
            to_return -> target_label = operand1;
        }
        to_return -> instruction.opcode = CALLI;
        to_return -> instruction.size   = 4;
    }

    BOOL valid = asm_validate_call(to_return);
    if(valid)
        return to_return;
    else
    {
        free(to_return);
        return NULL;
    }
}

/*!
@brief Parses the arguments of a RETURN instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
@todo Add operand validation.
*/
asm_statement * asm_parse_return(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;
    
    to_return -> type = OPCODE;
    to_return -> reg_1 = REG_NOT_USED;
    to_return -> reg_2 = REG_NOT_USED;
    to_return -> reg_3 = REG_NOT_USED;
    to_return -> instruction.opcode = RETURN;
    to_return -> instruction.size   = 1;

    return to_return;
}

/*!
@brief Parses the arguments of a HALT instruction.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
@todo Add operand validation.
*/
asm_statement * asm_parse_halt(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    
    to_return -> type = OPCODE;
    to_return -> reg_1 = REG_NOT_USED;
    to_return -> reg_2 = REG_NOT_USED;
    to_return -> reg_3 = REG_NOT_USED;
    to_return -> instruction.opcode = HALT;
    to_return -> instruction.size   = 1;

    return to_return;
}


/*!
@brief Parses the arguments of all instructions that take three register operands.
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
*/
asm_statement * asm_parse_bool_alu_opcode(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;

    char * operand1 = strtok(NULL, " ");
    char * operand2 = strtok(NULL, " ");
    char * operand3 = strtok(NULL, " \r\n");

    to_return -> type = OPCODE;
    to_return -> reg_1 = asm_parse_register(operand1);
    to_return -> reg_2 = asm_parse_register(operand2);
    to_return -> reg_3 = asm_parse_register(operand3);
    to_return -> instruction.size = 3;

    if(to_return -> reg_3 == REG_ERROR)
    {
        to_return -> reg_3 = REG_NOT_USED;
        to_return -> instruction.size = 4;
        to_return -> immediate = asm_parse_immediate(operand3, errors, line_num);
    }


    return to_return;
}

/*!
@brief Parses the arguments of a NOT instruction
@param [in] arguments - the remainder of the string containing the arguments to the opcode, with the
instruction removed.
@param [inout] errors - pointer to an error counter for syntax errors.
@param [in] line_num - The line number of the instruction, used for error reporting.
@returns An asm_statement structure which has its fields fully populated.
*/
asm_statement * asm_parse_not(char * arguments, int * errors, int line_num)
{
    asm_statement * to_return = calloc(1, sizeof(asm_statement));
    to_return -> line = line_num;

    char * operand1 = strtok(NULL, " ");
    char * operand2 = strtok(NULL, " \r\n");

    to_return -> type = OPCODE;
    to_return -> reg_1 = asm_parse_register(operand1);
    to_return -> reg_2 = asm_parse_register(operand2);
    to_return -> reg_3 = REG_NOT_USED;

    to_return -> instruction.size   = 2;
    to_return -> instruction.opcode = NOTR;

    return to_return;
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
    else if(strcmp(opcode, tim_MOV) == 0)
        to_return = asm_parse_mov(arguments, errors, line_num);
    else if(strcmp(opcode, tim_PUSH) == 0)
        to_return = asm_parse_push(arguments, errors, line_num);
    else if(strcmp(opcode, tim_POP) == 0)
        to_return = asm_parse_pop(arguments, errors, line_num);
    else if(strcmp(opcode, tim_JUMP) == 0)
        to_return = asm_parse_jump(arguments, errors, line_num);
    else if(strcmp(opcode, tim_CALL) == 0)
        to_return = asm_parse_call(arguments, errors, line_num);
    else if(strcmp(opcode, tim_RETURN) == 0)
        to_return = asm_parse_return(arguments, errors, line_num);
    else if(strcmp(opcode, tim_HALT) == 0)
        to_return = asm_parse_halt(arguments, errors, line_num);
    else if(strcmp(opcode, tim_NOT) == 0)
        to_return = asm_parse_not(arguments, errors, line_num);

    else if(strcmp(opcode, tim_AND)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? ANDR : ANDI;
    }
    else if(strcmp(opcode, tim_NAND)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? NANDR : NANDI;
    }
    else if(strcmp(opcode, tim_OR)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? ORR : ORI;
    }
    else if(strcmp(opcode, tim_NOR)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? NORR : NORI;
    }
    else if(strcmp(opcode, tim_XOR)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? XORR : XORI;
    }
    else if(strcmp(opcode, tim_LSL)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? LSLR : LSLI;
    }
    else if(strcmp(opcode, tim_LSR)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? LSRR : LSRI;
    }
    else if(strcmp(opcode, tim_IADD)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? IADDR : IADDI;
    }
    else if(strcmp(opcode, tim_ISUB)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? ISUBR : ISUBI;
    }
    else if(strcmp(opcode, tim_IMUL)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? IMULR : IMULI;
    }
    else if(strcmp(opcode, tim_IDIV)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? IDIVR : IDIVI;
    }
    else if(strcmp(opcode, tim_IASR)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? IASRR : IASRI;
    }
    else if(strcmp(opcode, tim_FADD)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? FADDR : FADDI;
    }
    else if(strcmp(opcode, tim_FSUB)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? FSUBR : FSUBI;
    }
    else if(strcmp(opcode, tim_FMUL)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? FMULR : FMULI;
    }
    else if(strcmp(opcode, tim_FDIV)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? FDIVR : FDIVI;
    }
    else if(strcmp(opcode, tim_FASR)  == 0){
        to_return = asm_parse_bool_alu_opcode(arguments, errors, line_num);
        to_return -> instruction.opcode = (to_return -> reg_3 != REG_NOT_USED) ? FASRR : FASRI;
    }
    else
    {
        *errors ++;
        error("Encountered invalid instruction '%s' on line %d.\n", opcode, line_num);
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
    int lines_read = 0;
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
