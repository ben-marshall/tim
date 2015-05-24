
cmake_minimum_required(VERSION 2.8)

project(tim-sw-common)
MESSAGE( STATUS "PROJECT NAME:            " ${PROJECT_NAME} )

SET(SRC_FILES    "common.c")
SET(HEADER_FILES "common.h")

add_library(tim-common ${HEADER_FILES} ${SRC_FILES})
