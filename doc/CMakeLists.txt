cmake_minimum_required(VERSION 2.8)

project(tim-doc)
MESSAGE( STATUS "PROJECT NAME:            " ${PROJECT_NAME} )


find_package(Doxygen)
if(DOXYGEN_FOUND)

    add_custom_target(docs-isa
    ${DOXYGEN_EXECUTABLE} doxygen-isa.config 2>&1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating ISA Documentation" VERBATIM
    )

    add_custom_target(docs-sw
    ${DOXYGEN_EXECUTABLE} doxygen-sw.config 2>&1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating Software Documentation" VERBATIM
    )
    
    add_custom_target(docs-hw
    ${DOXYGEN_EXECUTABLE} doxygen-hw.config 2>&1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating Hardware Documentation" VERBATIM
    )

    add_custom_target(docs
    echo "Built All Documentation."
    DEPENDS docs-isa docs-sw docs-hw
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating All Documentation" VERBATIM
    )


else(DOXYGEN_FOUND)

message(WARNING "Doxygen package not found. Unable to build documentation")

add_custom_target(docs
    echo "Please run: 'sudo apt-get install doxygen', Then re-run CMake"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Cannot Build Documentation" VERBATIM
)

endif(DOXYGEN_FOUND)
