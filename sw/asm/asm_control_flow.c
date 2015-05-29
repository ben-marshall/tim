/*!
@ingroup sw-asm
@{
@file asm_control_flow.c
@brief Code for calculating jump distances and assigning memory addresses to jump statements.
*/

#include "asm.h"

/*!
@brief Assigns addresses to each statement so that jumps and calls can be calculated.
@param statements - head of a linked list of asm statements.
@param base_address - Where the addresses of the program should start.
@returns The number of errors encountered such as missing labels. 0 means everything was okay.
*/
int asm_calculate_addresses(asm_statement * statements, unsigned int base_address)
{
    int errors = 0;
    unsigned int current_address = base_address;

    asm_statement * walker = statements;

    while(walker != NULL)
    {
        walker -> address = current_address;
        current_address += walker -> instruction.size;
        walker = walker -> next;
    }
    
    log("Program Size: %d Bytes\n", current_address);
    return errors;
}
