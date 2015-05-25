/*!
@ingroup sw-asm
@{
@file asm_parse.c
@brief Contains all functions relating to parsing the source code into IR data structure.
*/

#include "asm.h"


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

    return errors;
}
