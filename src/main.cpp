#include <stdio.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>  // EXIT_FAILURE
#include <vector>
#include <fstream>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "./loadShader.hpp"
#include "./loadTexture.hpp"

struct Color {
    float r,g,b,a;
};

namespace ost {
namespace color {
const Color BACKGROUND = {.3f, .9f, .3f, 1.0f};
const Color FLOOR  = {.3f, .9f, .3f, 1.0f};
const Color SCORE  = {.3f, .9f, .3f, 1.0f};
const Color CHEESE = {.3f, .9f, .3f, 1.0f};
}

const int LEVEL_COUNT  = 1;
const int OPENGL_MAJOR = 4;
const int OPENGL_MINOR = 1;

const char WIN_NAME[] = "Overkill Studio - Assignment1";
const int WIN_WIDTH   = 1024;
const int WIN_HEIGHT  = 768;
}

#define PANIC(msg) {\
            fprintf(stderr,\
                "----- PANIC -----| file: %s | line: %d | msg: %s\n", __FILE__, __LINE__,  msg);\
            getchar();\
            exit(EXIT_FAILURE);}\


void update(const GLFWwindow*);

int main(int argc, char* argv[]) {

    int _selectedLevel = 0;

    // PROCESS ARGUMENTS
    if (argc > 2) {
        PANIC("Too many arguments from command-line");
    }
    if (argc == 2) {
        _selectedLevel = atoi(argv[1]);
    }
    if (_selectedLevel < 0 || _selectedLevel >= ost::LEVEL_COUNT) {
        PANIC("Level not available");
    }
    const int selectedLevel = _selectedLevel;
    printf("Starting level %d\n", selectedLevel);

    // INITIALIZE G L F W
    if (!glfwInit()){
        glfwTerminate();
        PANIC("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ost::OPENGL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ost::OPENGL_MINOR);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // OPEN G L F W WINDOW AND HOOK UP THE OPEN GL CONTEXT
    GLFWwindow* window = glfwCreateWindow(ost::WIN_WIDTH, ost::WIN_HEIGHT, ost::WIN_NAME, NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL) {
        glfwTerminate();
        PANIC("Failed to open GLFW window");
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);
    
    // INITIALIZE G L E W
    glewExperimental = GL_TRUE;  // MACOS/intel cpu support
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        PANIC("Failed to initialize GLEW");
    }
    
    // LOAD ASSETS (map, spritesheet)

    // BUILD ASSET-STRUCTS

    // BUILD SHADER PROGRAMS
    
    // @TODO - add sprite shader and floor-shader. 

    const GLuint pacmanShader = ost::loadTexture("./assets/pacman.bmp");
    if (pacmanShader == 0) {
        PANIC("Did not load pacman shader");
    }

    const GLuint shaderProgram = ost::loadShaderProgram("./src/vertex.glsl", "./src/fragment.glsl");
    if (shaderProgram == 0) {
        PANIC("Did not load shader program");
    }

    const GLint  positionAttribute = glGetAttribLocation(shaderProgram, "position");
    const GLint  colorAttribute    = glGetAttribLocation(shaderProgram, "color");

    // GENERATE GPU BUFFERS
    GLuint vao;
    glGenVertexArrays(1, &vao);
    GLuint vbo[2];
    glGenBuffers(2, vbo);

    // BUFFER DATA TO GPU
    glBindVertexArray(vao);

    const GLfloat vertexPosition[6] = {-0.5,-0.5,   0.5, -0.5,   0.0, 0.5};
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttribute);

    const GLfloat vertexColor[9] = {0.0,0.0,0.0,  0.0,0.0,0.0,  0.0,0.0,0.0};
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorAttribute);


    // GENERATE MATRICES

    // GAMELOOP
    bool running = true;
    glClearColor(ost::color::BACKGROUND.r,
                 ost::color::BACKGROUND.g,
                 ost::color::BACKGROUND.b,
                 ost::color::BACKGROUND.a);

    while (running) {

        // INPUT
        glfwPollEvents();
        running = (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                   glfwWindowShouldClose(window) == 0);    

        update(window);

        // RENDER
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
    }

    // CLEANUP
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void update(const GLFWwindow* window) {
    // UPDATE 
    // 1. MOVE PACMAN -  W, A, S, D
    {

    }
    // 2. MOVE GHOSTS
    {

    }
    // 3. INCREMENT HIGH SCORE
    {

    }
    // 4. DELETE CHEESE -  
    {

    }
}
