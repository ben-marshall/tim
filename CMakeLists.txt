
cmake_minimum_required(VERSION 2.8)

project(tim)

set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT}/build )
set (LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH} )


add_custom_target(gitclean
    git clean -dfx
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Cleaning all non-git files..." VERBATIM
)

add_custom_target(gitlog
    git log --decorate --pretty=oneline --graph
)

add_subdirectory(sw)
add_subdirectory(hw)
add_subdirectory(doc)

MESSAGE( STATUS "------------------------[ PROJECT INFORMATION ]-------------------------------" )
MESSAGE( STATUS "PROJECT NAME:           " ${PROJECT_NAME} )
MESSAGE( STATUS "PROJECT ROOT FOLDER:    " ${PROJECT_ROOT} )
MESSAGE( STATUS "EXECUTABLE OUTPUT PATH: " ${EXECUTABLE_OUTPUT_PATH} )
MESSAGE( STATUS "LIBRARY OUTPUT PATH:    " ${LIBRARY_OUTPUT_PATH} )
MESSAGE( STATUS "BUILD TYPE:             " ${CMAKE_BUILD_TYPE} )
MESSAGE( STATUS "------------------------------------------------------------------------------" )
