#!/bin/bash

BUILD_DIR="./build"
SOURCE_DIR="./src"

build() {
    INCLUDE_PATHS="-I/usr/local/include"
    LIBRARY_PATHS="-L/usr/local/lib"
    LINKER_FLAGS=

    # @doc https://stackoverflow.com/a/8597411/6292603
    if [[ "$OSTYPE" == "linux-gnu" ]]; then   #if LINUX
        LINKER_FLAGS="-lopengl -lglfw -lglew"
        echo "Building Linux($OSTYPE): "

    elif [[ "$OSTYPE" == "darwin"* ]]; then   #if MacOS
        LINKER_FLAGS="-framework OpenGL -lglfw -lglew"
        echo "Building MacOS($OSTYPE): "
    else
        echo "ERROR: OS NOT SUPPORTED"
        exit 1;
    fi

    g++ $INCLUDE_PATHS\
        $LIBRARY_PATHS\
        $LINKER_FLAGS\
        -std=c++14\
        -g\
        -Wall\
        -o $BUILD_DIR/main\
        $SOURCE_DIR/main.cpp
}

run() {
    $BUILD_DIR/main $1   
}

key="$1"
case $key in
    build)
        build
    ;;
    run)
        run
    ;;
    *)
        build
        run
    ;;
esac
