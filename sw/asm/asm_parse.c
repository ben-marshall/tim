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
@see The ISA Specification contains more information on the grammar of the assembly language.
@param [in] source - Opened source file pointer. Open in "r" mode.
@param [inout] labels - Hashtable which is apopulated with any encountered labels.
@param [inout] errors - Pointer to a error counter. If the counter has the same value before
and after being called, all of the parsing was a success.
@returns The parsed statements as a doublely linked list.
*/
asm_statement * asm_parse_input(FILE * source, asm_hash_table * labels, int * errors)
{
    int lines_read = 0;
    char * line = NULL;

    asm_statement * walker = NULL;
    asm_statement * head   = NULL;

    while(feof(source) == 0)
    {
        line = NULL;
        if(line == NULL) break;

        lines_read ++;
        fflush(stdout);

        if(walker == NULL)
        {
            walker = calloc(1, sizeof(asm_statement));
            head = walker;
        }
        else
        {
            walker -> next = calloc(1, sizeof(asm_statement));
            if(walker -> next != NULL)
            {
                walker -> next -> prev = walker;
                walker  = walker -> next;
            }
        }

        free(line);
    }

    if(head == NULL)
        error("Returning NULL walker in parser.\n");

    return head;
}
