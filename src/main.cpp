#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

int main(int argc, char* argv[]) {

    // PROCESS ARGUMENTS
    const int SUPPORTED_NUMBER_OF_ARGUMENTS = 2;
    assert(argc <= SUPPORTED_NUMBER_OF_ARGUMENTS);
    printf("Argv 1: %s\n", argv[1]);

    // INIT OPENGL 

    // INIT WINDOW1

    // LOAD ASSETS (map, spritesheet)

    // BUILD ASSETS STRUCTS

    // GENERATE GPU BUFFERS

    // GENERATE MATRICES

    // GAMELOOP

        // INPUT

        // UPDATE 

        // RENDER


    // CLEANUP

    return 0;
}