/*!
@ingroup sw-common
@{
@file common.h
@brief Header file for all common code shared across the tim toolchain.
*/

#include "stdio.h"

#ifndef COMMON_H
#define COMMON_H

//! The prefix for all stdout/stderr messages.
#define TIM_PRINT_PROMPT "\e[1;36mtim>\e[0m "

//! Prints the prompt to stdout.
#define TIM_PROMPT  printf(TIM_PRINT_PROMPT)

//! Masking print macro that places the prompt in front of the message.
#define tprintf(...) TIM_PROMPT; printf(__VA_ARGS__)

//! Alias for the tprintf macro.
#define log(...) TIM_PROMPT; printf(__VA_ARGS__)

#ifdef DEBUG

    //! Debug macro that prints line number, function name and file.
    #define tdebug(...) TIM_PROMPT; \
                        printf("line %d of %s in %s\n", __LINE__, __FUNCTION__, __FILE__); \
                        TIM_PROMPT; \
                        printf(__VA_ARGS__)
#endif
#ifndef DEBUG

    //! Alternative debug macro that is evaluated to NOP when we are not in a debug build.
    #define tdebug(...) ; 

#endif

//! Warning macro that spits out line number and function.
#define warning(...) TIM_PROMPT; \
                   printf("\e[1;33m[Warning] \e[0m"); \
                   printf("line %d of %s in %s\n", __LINE__, __FUNCTION__, __FILE__); \
                   TIM_PROMPT; \
                   printf("\e[1;33m[Warning] \e[0m"); \
                   printf(__VA_ARGS__)

//! Error macro that spits out the line number and function.
#define error(...) TIM_PROMPT; \
                   printf("\e[1;31m[Error] \e[0m"); \
                   printf("line %d of %s in %s\n", __LINE__, __FUNCTION__, __FILE__); \
                   TIM_PROMPT; \
                   printf("\e[1;31m[Error] \e[0m"); \
                   printf(__VA_ARGS__)

//! Fatal error macro that behaves the same as error() but also exits the program.
#define fatal(...) error(__VA_ARGS__); \
                   TIM_PROMPT; \
                   printf("[ FATAL ERROR ]");\
                   exit(1)

#endif

//! }@
