
cmake_minimum_required(VERSION 2.8)

project(tim-sw-asm)
MESSAGE( STATUS "PROJECT NAME:            " ${PROJECT_NAME} )

SET(SRC_FILES    "asm.c")
SET(HEADER_FILES "asm.h")

add_executable(tim-asm ${HEADER_FILES} ${SRC_FILES})
target_link_libraries(tim-asm tim-common)
