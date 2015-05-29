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

    // First walk over the program assigning addresses to the statements.
    asm_statement * walker = statements;
    while(walker != NULL)
    {
        walker -> address = current_address;
        if(walker -> type == DATA ||
           walker -> type == NOP  ||
           walker -> type == OPCODE)
        {
            current_address += walker -> instruction.size;
        }
        walker = walker -> next;
    }
    
    // Now walk over the program replacing jump label targets with the proper immediate
    // values.
    walker = statements;
    while(walker != NULL)
    {
        if(walker -> target_label != NULL)
        {
            log("Calculating jump to '%s' for instruction on line %d\n", walker -> target_label, walker -> line_number);
        }
        walker = walker -> next;
    }
    
    log("Program Size: %d Bytes\n", current_address - base_address);
    return errors;
}
