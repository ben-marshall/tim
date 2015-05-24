/*!
@file asm.c
@brief Main source file for the assembler. Contains main function and argument parser.
*/

#include "asm.h"


/*!
@brief prints usage instructions for the program.
*/
void usage(int argc, char ** argv)
{
    printf("TIM Assembler                                                      \n");
    printf("-------------------------------------------------------------------\n");
    printf("                                                                   \n");
    printf("Usage: $> %s -i <input file> -o <output file> -f format\n", argv[0]);
    printf("\n");
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
                    printf("asm: unknown output format: %s\n", argv[arg+1]);
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
            printf("asm: Unknown argument: '%s'\n", argv[arg]);
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

    cxt -> source = fopen(cxt -> input_file, "r");

    if(cxt -> format == ASCII)
        cxt -> binary = fopen(cxt -> output_file, "w");
    else if(cxt -> format == BINARY)
        cxt -> binary = fopen(cxt -> output_file, "wb");

    cxt -> symbol_table = calloc(1, sizeof(asm_hash_table));
    // asm_hash_table_new(25, cxt -> symbol_table);


    //
    // Run everything about here...
    //


    fclose(cxt -> source);
    fclose(cxt -> binary);
    free(cxt);

    return 0;
}