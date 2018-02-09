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
#include "./Entity.hpp"
#include "./spritesheet.hpp"


//DISCUSSION: using Color = float[4]; instead?
struct Color {
    float r,g,b,a;
};

namespace ost {
namespace color {
const Color BACKGROUND = {.3f, .9f, .3f, 1.0f};
const Color FLOOR  = {1.0f, .1f, .8f, 1.0f};
const Color SCORE  = {.3f, .9f, .3f, 1.0f};
const Color CHEESE = {.3f, .9f, .3f, 1.0f};
}

const int LEVEL_COUNT  = 1;
const int OPENGL_MAJOR = 4;
const int OPENGL_MINOR = 1;

const char WIN_NAME[] = "Overkill Studio - Assignment1";
const int WIN_WIDTH   = 500;
const int WIN_HEIGHT  = 500;
    
enum BufferNames {
    BUFFER_LEVEL,
    BUFFER_SPRITE
};
}

inline bool update(GLFWwindow* window, ost::Pacman& pacman);
void render(GLFWwindow* window, const GLuint vao[], const std::vector<glm::vec2>& levelVertices,const std::vector<float>& pacmanVertices, const GLuint levelShader, const GLuint spriteShader);


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


    //
    // GENERATE GPU BUFFERS
    //
    GLuint vao[2];
    glGenVertexArrays(2, vao);
    
    GLuint vbo[2];
    glGenBuffers(2, vbo);

    //
    // LOAD ASSETS (map, spritesheet)
    //
    const std::vector<glm::vec2> levelVertices = ost::loadLevel("./levels/level0");
    const GLuint pacmanTexture = ost::loadTexture("./textures/pacman.png");
    const GLuint levelShader = ost::loadShaderProgram("./shaders/level.vert", "./shaders/level.geo","./shaders/level.frag");
    const GLuint spriteShader = ost::loadShaderProgram("./shaders/sprite.vert", "./shaders/sprite.frag");

    // @todo - 08.02.17
    // GLint quadSize = glGetUniformLocation(levelShader, "quadSize");
    // glUniform1f(quadSize, 2f/levelWidth);


    //
    // LEVEL SHADER SETUP
    //
    glBindVertexArray(vao[ost::BUFFER_LEVEL]);
    const GLint levelColor = glGetUniformLocation(levelShader, "floor_color");

    float floorColor[] = {ost::color::FLOOR.r,
                          ost::color::FLOOR.g,
                          ost::color::FLOOR.b,
                          ost::color::FLOOR.a};

    glUniform4fv(levelColor, 1, floorColor);

    const GLint  positionAttribute = glGetAttribLocation(levelShader, "position");
    const GLint  colorAttribute    = glGetAttribLocation(levelShader, "color");

    glBindBuffer(GL_ARRAY_BUFFER, vbo[ost::BUFFER_LEVEL]);
    glBufferData(GL_ARRAY_BUFFER, levelVertices.size()*sizeof(glm::vec2), levelVertices.data(), GL_STREAM_DRAW);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttribute);

    glBindVertexArray(0);


    //
    // SPRITE SHADER SETUP
    //
    glBindVertexArray(vao[ost::BUFFER_SPRITE]);    
    ost::Pacman             pacman{};
    //@todo std::vector<ost::Ghost> ghost{};

    const GLint  pacmanPosition = glGetAttribLocation(spriteShader, "position");
    const GLint  pacmanTexcoord = glGetAttribLocation(spriteShader, "texcoord");
   
    const auto _buffer = pacman.getBuffer();
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[ost::BUFFER_SPRITE]);
    glBufferData(GL_ARRAY_BUFFER, _buffer.size()*sizeof(float), _buffer.data(), GL_STREAM_DRAW);

    glVertexAttribPointer(pacmanPosition, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);
    glVertexAttribPointer(pacmanTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2));        
    
    glEnableVertexAttribArray(pacmanPosition);
    glEnableVertexAttribArray(pacmanTexcoord);
         
    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);
    const GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);
    glBindVertexArray(0);


    //
    // GAMELOOP
    //
    {
        const Color clear = ost::color::BACKGROUND;
        glClearColor(clear.r,clear.g,clear.b,clear.a);
    }
    bool running = true;
    while (running) {
        running = update(window, pacman);
        render(window, vao, levelVertices, pacman.getBuffer(), levelShader, spriteShader);
    }

    //
    // CLEANUP
    //
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

bool update(GLFWwindow* window, ost::Pacman& pacman) {
    glfwPollEvents();
    
    // Configure delta time
    static double baseTime = glfwGetTime();
    static double lastKeyTime = glfwGetTime();
    const double  keyMinInterval = 0.2;
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - baseTime;
    baseTime = glfwGetTime();

    // UPDATE
    // 1. MOVE PACMAN -  W, A, S, D
    {
        pacman.move(deltaTime);
        pacman.animate(deltaTime);

        if (currentTime - lastKeyTime >  keyMinInterval) {
            lastKeyTime = glfwGetTime();
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)   { 
                pacman.towards(ost::Entity::Direction::UP);   
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  { 
                pacman.towards(ost::Entity::Direction::DOWN); 
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ) { 
                pacman.towards(ost::Entity::Direction::LEFT); 
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ){ 
                pacman.towards(ost::Entity::Direction::RIGHT); 
            }
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


void render(GLFWwindow* window, const GLuint vao[], const std::vector<glm::vec2>& levelBuffer, const std::vector<float>& pacmanBuffer, const GLuint levelShader, const GLuint spriteShader) {
      
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        { // LEVEL DRAWCALL
            glUseProgram(levelShader);
            glBindVertexArray(vao[ost::BUFFER_LEVEL]);
            glDrawArrays(GL_POINTS, 0, levelBuffer.size());
            glBindVertexArray(0);
        }
        
        { // PACMAN DRAWCALL
            glUseProgram(spriteShader);
            glBindVertexArray(vao[ost::BUFFER_SPRITE]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, pacmanBuffer.size() * sizeof(float), pacmanBuffer.data());
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);            
        }         
        glfwSwapBuffers(window);
}