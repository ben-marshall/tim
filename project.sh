#! /bin/bash

BUILD_FOLDER='build'

function usage {
    echo "$> source ./project"
}

function log {
 echo -e "\e[1;36mproject>\e[0m $1"
}

function setup_build_folder {
    if [ ! -d build ]; then
        log "Created build folder '$BUILD_FOLDER'"
        mkdir build
    fi
}

function run_cmake {
    setup_build_folder;

    log "Running CMake Configuration..."
    cd $BUILD_FOLDER
    cmake ..
    RESULT=$?
    cd -

    if [ "$RESULT" == "0" ] ; then
        log "CMake [ \e[1;32mSUCCESS\e[0m ]"
    else
        log "CMake [ \e[1;31mFAILURE\e[0m ]"
    fi

}


setup_build_folder;
run_cmake;
