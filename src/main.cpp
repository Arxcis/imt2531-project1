#include <stdio.h>
#include <errno.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string.h>
#include <assert.h>
#include <stdlib.h>  // EXIT_FAILURE

const int ARGUMENT_LEVEL_INDEX = 2;
const int LEVEL_COUNT = 1;

#define PANIC(msg) { printf("----- PANIC -----| file: %s | line: %d | msg: %s",  __FILE__, __LINE__, msg); exit(EXIT_FAILURE); }

int main(int argc, char* argv[]) {

    int selectedLevel = 0;

    // PROCESS ARGUMENTS
    { 

    if (argc > ARGUMENT_LEVEL_INDEX) {
        PANIC("Too many arguments from command-line");
    }
    if (argc == ARGUMENT_LEVEL_INDEX) {
        selectedLevel = atoi(argv[1]);
    }
    if (selectedLevel < 0 || selectedLevel >= LEVEL_COUNT) {
        PANIC("Level not available");
    }
    printf("Starting level %d\n", selectedLevel);

    }

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