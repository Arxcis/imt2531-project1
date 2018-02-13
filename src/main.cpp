#include <stdio.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>  // EXIT_FAILURE
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "./macro.hpp"
#include "./loadShader.hpp"
#include "./loadLevel.hpp"
#include "./loadTexture.hpp"
#include "./Shader.hpp"
#include "./Entity.hpp"
#include "./spritesheet.hpp"
#include "./logger.h"


//DISCUSSION: using Color = float[4]; instead?
struct Color {
    float r,g,b,a;
};

namespace ost {
namespace color {
const Color BACKGROUND = {.3f, .9f, .3f, 1.0f};
const Color FLOOR  = {1.0f, .7f, .8f, 1.0f};
const Color SCORE  = {.3f, .9f, .3f, 1.0f};
const Color CHEESE = {.3f, .9f, .3f, 1.0f};
}
}

inline GLFWwindow* init_GLFW_GLEW(const int openglMajor, const int openglMinor, const int wwidth, const int wheight, const char* wname);
inline bool update(GLFWwindow* window, ost::Pacman& pacman, ost::Level& level);
inline void render(GLFWwindow* window, const ost::Shader& levelShader, const ost::Shader& spriteShader, const ost::Shader& cheeseShader);


int main(int argc, char* argv[]) {

    const int OPENGL_MAJOR = 4;
    const int OPENGL_MINOR = 1;
    const char WIN_NAME[] = "Overkill Studio - Assignment1";
    const int  WIN_WIDTH   = 500;
    const int  WIN_HEIGHT  = 500;

    auto window = init_GLFW_GLEW(OPENGL_MAJOR, OPENGL_MINOR, WIN_WIDTH, WIN_HEIGHT, WIN_NAME);

    using namespace ost::color;
    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(BACKGROUND.r,BACKGROUND.g,BACKGROUND.b,BACKGROUND.a);



    LOG_INFO("LOADING FILES");    
    ost::Level level                 = ost::loadLevel("./levels/level0");
    const GLuint levelShaderProgram  = ost::loadShaderProgram("./shaders/general.vert", "./shaders/level.geo","./shaders/level.frag");
    const GLuint pacmanTexture       = ost::loadTexture("./textures/pacman.png");
    const GLuint spriteShaderProgram = ost::loadShaderProgram("./shaders/general.vert", "./shaders/sprite.frag");
    const GLuint cheeseShaderProgram = ost::loadShaderProgram("./shaders/general.vert", "./shaders/cheese.frag");


    LOG_INFO("INIT LEVEL SHADER");
    ost::Shader levelShader = ost::makeShader_VBO(levelShaderProgram, level.vertices.size(), GL_STATIC_DRAW, GL_POINTS);
    level.bindBufferVertices( getVertexBufferIt(levelShader, level.vertices.size()) );
    ost::setUniformFloat(levelShader, "quadSize",     2.0f/level.biggestSize);
    ost::setUniformVec4(levelShader,  "floor_color", {ost::color::FLOOR.r, ost::color::FLOOR.g,ost::color::FLOOR.b,ost::color::FLOOR.a});

    ost::setUniformMat4(levelShader, "scale", level.scaleMatrix);
    ost::setUniformMat4(levelShader, "move", level.moveMatrix);


    LOG_INFO("INIT SPRITE SHADER");
    ost::Shader spriteShader = ost::makeShader_VBO_EBO(spriteShaderProgram, 24, 36, GL_STREAM_DRAW, GL_TRIANGLES);
    ost::Pacman pacman       = ost::Pacman{ getVertexBufferIt(spriteShader, 4), getElementBufferIt(spriteShader, 6),  0, {0.0f, 16.0f}, level};
   // ost::Ghost ghost1        = ost::Ghost{ getVertexBufferIt(spriteShader, 4), elementBufferIt(spriteShader, 6),   4, {-9.0f, 1.5f}};
   // ost::Ghost ghost2        = ost::Ghost{ getVertexBufferIt(spriteShader, 4), elementBufferIt(spriteShader, 6),   8, {-5.0f, 1.5f}};
    ost::setUniformMat4(spriteShader, "scale", level.scaleMatrix);
    ost::setUniformMat4(spriteShader, "move", level.moveMatrix);



    LOG_INFO("INIT CHEESE SHADER");
    ost::Shader cheeseShader = ost::makeShader_VBO(cheeseShaderProgram, level.vertices.size(), GL_STATIC_DRAW, GL_POINTS);
    for (auto v : level.vertices) {
        ost::Cheese cheese = ost::Cheese{ getVertexBufferIt(cheeseShader, 1), v + glm::vec2(0.5f,-0.5f)};
    }
    ost::setUniformFloat(cheeseShader, "pointSize", 5.0f);
    ost::setUniformMat4(cheeseShader, "scale", level.scaleMatrix);
    ost::setUniformMat4(cheeseShader, "move", level.moveMatrix);


    //
    // GAMELOOP
    //
    bool running = true;
    while (running) {
        running = update(window, pacman, level);
        render(window, levelShader, spriteShader, cheeseShader);
    }

    //
    // CLEANUP
    //
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


inline GLFWwindow* init_GLFW_GLEW(const int openglMajor, const int openglMinor, const int wwidth, const int wheight, const char* wname) {
    // INITIALIZE G L F W
    if (!glfwInit()){
        glfwTerminate();
        PANIC("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // OPEN G L F W WINDOW AND HOOK UP THE OPEN GL CONTEXT
    GLFWwindow* window = glfwCreateWindow(wwidth, wheight, wname, NULL, NULL);
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
    return window;
}


inline bool update(GLFWwindow* window, ost::Pacman& pacman, ost::Level& level) {
    glfwPollEvents();

    // Configure delta time
    static double baseTime = glfwGetTime();
    static double lastKeyTime = glfwGetTime();
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - baseTime;
    baseTime = glfwGetTime();

    // UPDATE
    // 1. MOVE PACMAN -  W, A, S, D
    {
        pacman.move(deltaTime);
        pacman.animate(deltaTime);
     //   ghost.animate(deltaTime);

        lastKeyTime = glfwGetTime();
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)   {
            pacman.towards(ost::vecUp, level);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  {
            pacman.towards(ost::vecDown, level);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ) {
            pacman.towards(ost::vecLeft, level);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ){
            pacman.towards(ost::vecRight, level);
        }
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

    return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}


inline void render(GLFWwindow* window,
                const   ost::Shader& levelShader,
                const   ost::Shader& spriteShader,
                const   ost::Shader& cheeseShader) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_VBO(levelShader);
    draw_VBO(cheeseShader);
    draw_VBO_EBO(spriteShader);

    glfwSwapBuffers(window);
}
