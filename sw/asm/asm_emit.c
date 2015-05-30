/*!
@ingroup sw-asm
@{
@file asm_emit.h
@brief Code responsible for emitting code as binary or ascii codes.
*/

#include "asm.h"

/*!
@brief Responsible for writing all statements to the supplied file.
@param statements - Linked list of statements to emit binary code for.
@param file - The file to write the code too.
@param format - Whether to emit the code as raw bytes or ascii binary strings. This is used to
feed the VHDL testbenches.
@returns An integer representing the number of errors encountered, if any.
*/
int asm_emit_instructions(asm_statement * statements, FILE * file, asm_format format)
{
    int errors = 0;
    asm_statement * walker = statements;

    while(walker != NULL)
    {
        switch(walker -> opcode)
        {
            case (LOADR ):
            case (LOADI ):
            case (STORI ):
            case (STORR ):
            case (PUSH  ):
            case (POP   ):
            case (MOVR  ):
            case (MOVI  ):
            case (MOVSR ):
            case (MOVRS ):
            case (JUMPR ):
            case (JUMPI ):
            case (CALLR ):
            case (CALLI ):
            case (RETURN):
            case (TEST  ):
            case (HALT  ):
            case (ANDR  ):
            case (NANDR ):
            case (ORR   ):
            case (NORR  ):
            case (XORR  ):
            case (LSLR  ):
            case (LSRR  ):
            case (NOTR  ):
            case (ANDI  ):
            case (NANDI ):
            case (ORI   ):
            case (NORI  ):
            case (XORI  ):
            case (LSLI  ):
            case (LSRI  ):
            case (IADDI ):
            case (ISUBI ):
            case (IMULI ):
            case (IDIVI ):
            case (IALSI ):
            case (IASRI ):
            case (IADDR ):
            case (ISUBR ):
            case (IMULR ):
            case (IDIVR ):
            case (IASLR ):
            case (IASRR ):
            case (FADDI ):
            case (FSUBI ):
            case (FMULI ):
            case (FDIVI ):
            case (FASLI ):
            case (FASRI ):
            case (FADDR ):
            case (FSUBR ):
            case (FMULR ):
            case (FDIVR ):
            case (FASLR ):
            case (FASRR ):
            case (SLEEP ):
            case (NOT_EMITTED):

            default:
                error("Cannot emit opcode type: %d\n", walker -> opcode);
                errors += 1;
                break;
        }
        walker = walker -> next;
    }

    return errors;
}
