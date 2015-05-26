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
    {
        warning("Instruction '%s' not yet implemented on line %d\n", opcode, line_num);
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
