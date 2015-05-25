/*!
@ingroup sw-asm
@{
@file asm_parse.c
@brief Contains all functions relating to parsing the source code into IR data structure.
*/

#include "asm.h"

/*
@Brief reads and returns the rest of the current line in the file from the current cursor
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
int asm_parse_input(FILE * source, asm_statement * statements, asm_hash_table * labels)
{
    int errors = 0;
    int lines_read = 1;
    char * line = NULL;

    while(feof(source) == 0)
    {
        line = asm_parse_readline(source);
        if(line == NULL) break;

        lines_read ++;

        // Parse the line content here.

        free(line);
    }

    return errors;
}
