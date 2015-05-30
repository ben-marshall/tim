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
int asm_calculate_addresses(asm_statement * statements, unsigned int base_address, asm_hash_table * labels)
{
    int errors = 0;
    unsigned int current_address = base_address;

    // First walk over the program assigning addresses to the statements.
    asm_statement * walker = statements;
    while(walker != NULL)
    {
        walker -> address = current_address;
        current_address += walker -> size;
        walker = walker -> next;
    }
    
    // Now walk over the program replacing jump label targets with the proper immediate
    // values.
    walker = statements;
    while(walker != NULL)
    {
        if(walker -> label_to_resolve)
        {
            asm_statement * target = NULL;

            switch(walker -> opcode)
            {
                case(CALLI):
                case(JUMPI):
                case(NOT_EMITTED):
                    target = asm_hash_table_get(labels, walker -> args.immediate_label.label);
                    if(target == NULL)
                    {
                        error("Could not find label declaration for %s\n", walker -> args.immediate_label.label);
                        return errors + 1;
                    }
                    target = target -> next;
                    break;
                default:
                    error("Cannot resolve label for instruction opcode %d\n", walker -> opcode);
                    break;
            }

            unsigned int address_difference = target -> address;
            //log("Calculated jump to %d\n", address_difference);
            walker -> args.immediate.immediate = address_difference;
        }
        walker = walker -> next;
    }
    
    log("Program Size: %d Bytes\n", current_address - base_address);
    return errors;
}
