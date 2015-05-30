
cmake_minimum_required(VERSION 2.8)

project(tim-sw-asm)
MESSAGE( STATUS "PROJECT NAME:            " ${PROJECT_NAME} )

SET(SRC_FILES   "asm.c"
                "asm_lex.h"
                "asm_lex.c"
                "asm_hash_table.c"
                "asm_parse.c"
                "asm_control_flow.c"
                "asm_emit.c")
SET(HEADER_FILES "asm.h")

include_directories("../common")

add_executable(tim-asm ${HEADER_FILES} ${SRC_FILES})
target_link_libraries(tim-asm tim-common)
