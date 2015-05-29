/*!
@ingroup sw-asm
@{
@file asm.c
@brief Main source file for the assembler. Contains main function and argument parser.
*/

#include "asm.h"

/*!
@brief prints usage instructions for the program.
*/
void usage(int argc, char ** argv)
{
    tprintf("TIM Assembler                                                      \n");
    tprintf("-------------------------------------------------------------------\n");
    tprintf("                                                                   \n");
    tprintf("Usage: $> %s -i <input file> -o <output file> -f format\n", argv[0]);
    tprintf("\n");
}

/*!
@brief Parses the command line arguments passed to the program into a program_context object.
*/
void parse_cmd_args(int argc, char ** argv, asm_context * cxt)
{
    cxt -> format = ASCII;
    int arg;

    for(arg = 1; arg<argc; arg++)
    {
        if(strcmp(argv[arg], "-i") == 0)
        {
            if(arg+1 < argc)
            {
                cxt -> input_file = argv[arg+1];
                arg++;
            }
            else
            {
                usage(argc, argv);
                exit(1);
            }
        }
        else if(strcmp(argv[arg], "-o") == 0)
        {
            if(arg+1 < argc)
            {
                cxt -> output_file = argv[arg+1];
                arg++;
            }
            else
            {
                usage(argc, argv);
                exit(1);
            }
        }
        else if(strcmp(argv[arg], "-f") == 0)
        {
            if(arg+1 <= argc)
            {
                if(strcmp(argv[arg+1], "ascii") == 0)
                    cxt -> format = ASCII;
                else if(strcmp(argv[arg+1], "binary") == 0)
                    cxt -> format = BINARY;
                else
                {
                    fatal("Unknown output format: %s\n", argv[arg+1]);
                    usage(argc, argv);
                    exit(1);
                }
                arg++;
            }
            else
            {
                usage(argc, argv);
                exit(1);
            }
        }
        else
        {
            warning("Unknown argument: '%s'\n", argv[arg]);
            usage(argc, argv);
            exit(1);
        }
    }
}

/*!
@brief Main entry point for the application.
*/
int main(int argc, char ** argv)
{
    if(argc == 1)
    {
        usage(argc, argv);
        exit(1);
    }

    asm_context * cxt = calloc(1, sizeof(asm_context));
    parse_cmd_args(argc, argv, cxt);

    if(cxt -> input_file == NULL || cxt -> output_file == NULL)
    {
        usage(argc, argv);
        exit(1);
    }

    log("Source File:\t %s\n", cxt -> input_file);
    log("Output File:\t %s\n", cxt -> output_file);
    cxt -> source = fopen(cxt -> input_file, "r");
    
    if(ferror(cxt -> source))
    {
        error("Error code %d opening source file.\n", ferror(cxt -> source));
        fatal("Could not open input file: %s\n", cxt -> input_file);
    }

    if(cxt -> format == ASCII)
        cxt -> binary = fopen(cxt -> output_file, "w");
    else if(cxt -> format == BINARY)
        cxt -> binary = fopen(cxt -> output_file, "wb");

    if(ferror(cxt -> binary))
    {
        error("Error code %d opening output file.\n", ferror(cxt -> binary));
        fatal("Could not open output file: %s\n", cxt -> output_file);
    }

    cxt -> statements = NULL;
    cxt -> symbol_table = calloc(1, sizeof(asm_hash_table));
    asm_hash_table_new(25, cxt -> symbol_table);
    
    // Parse everything into a linked list of statments.
    int errors = asm_parse_input(cxt -> source, cxt -> statements, cxt -> symbol_table);
    if(errors > 0) fatal("Encountered %d parser errors.\n", errors);

    // Calculate memory addresses for each statment for jumps. Hard code a base address of zero
    // for now.
    errors = asm_calculate_addresses(cxt -> statements, 0);
    if(errors > 0) fatal("Encountered %d errors in address calculation.\n", errors);


    fclose(cxt -> source);
    fclose(cxt -> binary);
    free(cxt);

    return 0;
}

//! }@
