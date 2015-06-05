/*!
@ingroup sw-asm
@{
@file asm_emit.h
@brief Code responsible for emitting code as binary or ascii codes.
*/

#include "asm.h"

int asm_ascii_counter = 0;

/*!
@brief Writes out ascii code instead of binary to the supplied file.
@param to_write - the entire instruction to write to file right aligned in the 32 bit unsigned int.
@param length - The number of BITS in the instruction. This number of upper bits from the to_write
field will be emitted.
*/
int asm_emit_ascii(unsigned int to_write, unsigned char length, FILE * file)
{
    int i;
    for(i = 31; i > 31- length; i--)
    {
        to_write & ((unsigned int)1)<<i ? fprintf(file,"1") : fprintf(file,"0");
        asm_ascii_counter += 1;
        if(asm_ascii_counter >=8)
        {
            asm_ascii_counter = 0;
            fprintf(file, "\n");
        }
    }
    
    return 0;
}

int asm_emit_opcode_LOADR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-5);
    to_write |= ((unsigned int)statement -> condition) << (31-6-1);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-3);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-3);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4-3);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 24, file); else
    fwrite(&to_write, 3, 1, file);
    return 0;
}

int asm_emit_opcode_LOADI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-5);
    to_write |= ((unsigned int)statement -> condition) << (31-6-1);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-3);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-3);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_STORI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-5);
    to_write |= ((unsigned int)statement -> condition) << (31-6-1);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-3);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-3);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_STORR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-5);
    to_write |= ((unsigned int)statement -> condition) << (31-6-1);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-3);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-3);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4-3);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 24, file); else
    fwrite(&to_write, 3, 1, file);
    return 0;
}

int asm_emit_opcode_PUSH  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg.reg_1) << (31-6-2-5);

    if(format == ASCII) asm_emit_ascii(to_write, 16, file); else
    fwrite(&to_write, 2, 1, file);
    return 0;
}

int asm_emit_opcode_POP   (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg.reg_1) << (31-6-2-5);

    if(format == ASCII) asm_emit_ascii(to_write, 16, file); else
    fwrite(&to_write, 2, 1, file);
    return 0;
}

int asm_emit_opcode_MOVR  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg.reg_1) << (31-6-2-5);
    to_write |= ((unsigned int)statement -> args.reg_reg.reg_2) << (31-6-2-5-5);

    if(format == ASCII) asm_emit_ascii(to_write, 24, file); else
    fwrite(&to_write, 3 , 1, file);
    return 0;
}

int asm_emit_opcode_MOVI  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_immediate.reg_1) << (31-6-2-5);
    to_write |= ((unsigned int)statement -> args.reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_JUMPR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg.reg_1) << (31-6-2-5);

    if(format == ASCII) asm_emit_ascii(to_write, 31, file); else
    fwrite(&to_write, 2, 1, file);
    return 0;
}

int asm_emit_opcode_JUMPI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_CALLR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg.reg_1) << (31-6-2-5);

    if(format == ASCII) asm_emit_ascii(to_write, 16, file); else
    fwrite(&to_write, 2, 1, file);
    return 0;
}

int asm_emit_opcode_CALLI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_RETURN(asm_statement * statement, FILE * file, asm_format format){
    unsigned char to_write  = ((unsigned char)statement -> opcode)    << (7-6);
    to_write |= ((unsigned char)statement -> condition);
     
    if(format == ASCII) asm_emit_ascii(to_write, 8, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_TEST  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg.reg_1) << (23-6-2-5);
    to_write |= ((unsigned int)statement -> args.reg_reg.reg_2) << (23-6-2-5-5);

    if(format == ASCII) asm_emit_ascii(to_write, 24, file); else
    fwrite(&to_write, 3 , 1, file);
    return 0;
}

int asm_emit_opcode_HALT  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-5);
    to_write |= ((unsigned int)statement -> condition) << (31-6-1);
     
    if(format == ASCII) asm_emit_ascii(to_write, 8, file); else
    fwrite(&to_write, 1, 1, file);
    return 0;
}

int asm_emit_opcode_ANDR  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_NANDR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_ORR   (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_NORR  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_XORR  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_LSLR  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_LSRR  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_NOTR  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (23-6);
    to_write |= ((unsigned int)statement -> condition) << (23-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg.reg_1) << (23-6-2-5);
    to_write |= ((unsigned int)statement -> args.reg_reg.reg_2) << (23-6-2-5-5);

    if(format == ASCII) asm_emit_ascii(to_write, 24, file); else
    fwrite(&to_write, 3 , 1, file);
    return 0;
}

int asm_emit_opcode_ANDI  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_NANDI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_ORI   (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_NORI  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_XORI  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_LSLI  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_LSRI  (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IADDI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_ISUBI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IMULI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IDIVI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IALSI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IASRI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IADDR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_ISUBR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IMULR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IDIVR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IASLR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_IASRR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FADDI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FSUBI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FMULI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FDIVI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FASLI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FASRI (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_immediate.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned short)statement -> args.reg_reg_immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FADDR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FSUBR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FMULR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FDIVR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FASLR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_FASRR (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_1) << (31-6-2-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_2) << (31-6-2-4-4);
    to_write |= ((unsigned int)statement -> args.reg_reg_reg.reg_3) << (31-6-2-4-4);
    to_write |= 0xF;

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}

int asm_emit_opcode_SLEEP (asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> opcode)    << (31-6);
    to_write |= ((unsigned int)statement -> condition) << (31-6-2);
    to_write |= ((unsigned int)statement -> args.reg.reg_1) << (31-6-2-5);

    if(format == ASCII) asm_emit_ascii(to_write, 16, file); else
    fwrite(&to_write, 2, 1, file);
    return 0;
}

int asm_emit_opcode_NOT_EMITTED(asm_statement * statement, FILE * file, asm_format format){
    unsigned int to_write  = ((unsigned int)statement -> args.immediate.immediate);

    if(format == ASCII) asm_emit_ascii(to_write, 32, file); else
    fwrite(&to_write, sizeof(to_write), 1, file);
    return 0;
}


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

    asm_ascii_counter = 0;

    while(walker != NULL)
    {
        switch(walker -> opcode)
        {
            case (LOADR ):   asm_emit_opcode_LOADR(walker, file, format); break; 
            case (LOADI ):   asm_emit_opcode_LOADI(walker, file, format); break;  
            case (STORI ):   asm_emit_opcode_STORI(walker, file, format); break;  
            case (STORR ):   asm_emit_opcode_STORR(walker, file, format); break;  
            case (PUSH  ):   asm_emit_opcode_PUSH (walker, file, format); break;  
            case (POP   ):   asm_emit_opcode_POP  (walker, file, format); break;  
            case (MOVR  ):   asm_emit_opcode_MOVR (walker, file, format); break;  
            case (MOVI  ):   asm_emit_opcode_MOVI (walker, file, format); break;  
            case (JUMPR ):   asm_emit_opcode_JUMPR(walker, file, format); break;  
            case (JUMPI ):   asm_emit_opcode_JUMPI(walker, file, format); break;  
            case (CALLR ):   asm_emit_opcode_CALLR(walker, file, format); break;  
            case (CALLI ):   asm_emit_opcode_CALLI(walker, file, format); break;  
            case (RETURN):   asm_emit_opcode_RETURN(walker, file, format); break;
            case (TEST  ):   asm_emit_opcode_TEST (walker, file, format); break;  
            case (HALT  ):   asm_emit_opcode_HALT (walker, file, format); break;  
            case (ANDR  ):   asm_emit_opcode_ANDR (walker, file, format); break;  
            case (NANDR ):   asm_emit_opcode_NANDR(walker, file, format); break;  
            case (ORR   ):   asm_emit_opcode_ORR  (walker, file, format); break;  
            case (NORR  ):   asm_emit_opcode_NORR (walker, file, format); break;  
            case (XORR  ):   asm_emit_opcode_XORR (walker, file, format); break;  
            case (LSLR  ):   asm_emit_opcode_LSLR (walker, file, format); break;  
            case (LSRR  ):   asm_emit_opcode_LSRR (walker, file, format); break;  
            case (NOTR  ):   asm_emit_opcode_NOTR (walker, file, format); break;  
            case (ANDI  ):   asm_emit_opcode_ANDI (walker, file, format); break;  
            case (NANDI ):   asm_emit_opcode_NANDI(walker, file, format); break;  
            case (ORI   ):   asm_emit_opcode_ORI  (walker, file, format); break;  
            case (NORI  ):   asm_emit_opcode_NORI (walker, file, format); break;  
            case (XORI  ):   asm_emit_opcode_XORI (walker, file, format); break;  
            case (LSLI  ):   asm_emit_opcode_LSLI (walker, file, format); break;  
            case (LSRI  ):   asm_emit_opcode_LSRI (walker, file, format); break;  
            case (IADDI ):   asm_emit_opcode_IADDI(walker, file, format); break;  
            case (ISUBI ):   asm_emit_opcode_ISUBI(walker, file, format); break;  
            case (IMULI ):   asm_emit_opcode_IMULI(walker, file, format); break;  
            case (IDIVI ):   asm_emit_opcode_IDIVI(walker, file, format); break;  
            case (IASRI ):   asm_emit_opcode_IASRI(walker, file, format); break;  
            case (IADDR ):   asm_emit_opcode_IADDR(walker, file, format); break;  
            case (ISUBR ):   asm_emit_opcode_ISUBR(walker, file, format); break;  
            case (IMULR ):   asm_emit_opcode_IMULR(walker, file, format); break;  
            case (IDIVR ):   asm_emit_opcode_IDIVR(walker, file, format); break;  
            case (IASRR ):   asm_emit_opcode_IASRR(walker, file, format); break;  
            case (FADDI ):   asm_emit_opcode_FADDI(walker, file, format); break;  
            case (FSUBI ):   asm_emit_opcode_FSUBI(walker, file, format); break;  
            case (FMULI ):   asm_emit_opcode_FMULI(walker, file, format); break;  
            case (FDIVI ):   asm_emit_opcode_FDIVI(walker, file, format); break;  
            case (FASRI ):   asm_emit_opcode_FASRI(walker, file, format); break;  
            case (FADDR ):   asm_emit_opcode_FADDR(walker, file, format); break;  
            case (FSUBR ):   asm_emit_opcode_FSUBR(walker, file, format); break;  
            case (FMULR ):   asm_emit_opcode_FMULR(walker, file, format); break;  
            case (FDIVR ):   asm_emit_opcode_FDIVR(walker, file, format); break;  
            case (FASRR ):   asm_emit_opcode_FASRR(walker, file, format); break;  
            case (SLEEP ):   asm_emit_opcode_SLEEP(walker, file, format); break;  
            case (NOT_EMITTED): asm_emit_opcode_NOT_EMITTED(walker, file,format); break;

            default:
                error("Cannot emit opcode type: %d\n", walker -> opcode);
                errors += 1;
                break;
        }
        walker = walker -> next;
    }

    return errors;
}
