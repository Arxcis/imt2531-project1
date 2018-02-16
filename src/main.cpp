#include <stdio.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>  // EXIT_FAILURE, srand, rand
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "./loadShader.hpp"
#include "./loadLevel.hpp"
#include "./loadTexture.hpp"
#include "./Shader.hpp"
#include "./Entity.hpp"
#include "./spritesheet.hpp"
#include "./logger.h"
#include "./Level.hpp"
//#include "./Primitive.hpp"
#define _1337 57
#define LOG_NO_DEBUG 0
//DISCUSSION: using Color = float[4]; instead?
struct Color {
    float r,g,b,a;
};

namespace ost {

bool restart = false;
bool pause   = false;
bool running = true;
}


inline GLFWwindow* init_GLFW_GLEW_OPENGL(const int openglMajor, const int openglMinor, const int wwidth, const int wheight, const char* wname);
inline bool update(GLFWwindow* window, ost::Pacman& pacman, ost::Level& level, std::vector<ost::Ghost>& ghosts, std::vector<ost::Cheese>& cheeses);
inline void render(GLFWwindow* window, Shader::Shader& levelShader, Shader::Shader& spriteShader, Shader::Shader& cheeseShader, Shader::Shader& fontShader);
inline void renderPause(GLFWwindow* window, Shader::Shader& levelShader, Shader::Shader& spriteShader, Shader::Shader& cheeseShader, Shader::Shader& fontShader);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

ost::UserInterface userInterface;

int main() {

    const int OPENGL_MAJOR = 4;
    const int OPENGL_MINOR = 1;
    const char WIN_NAME[] = "Overkill Studio - Assignment1";
    const int  WIN_WIDTH   = 500;
    const int  WIN_HEIGHT  = 500;

    auto window = init_GLFW_GLEW_OPENGL(OPENGL_MAJOR, OPENGL_MINOR, WIN_WIDTH, WIN_HEIGHT, WIN_NAME);


    LOG_INFO("LOADING FILES");
    ost::Level level     = ost::loadLevel("./levels/level0");
    GLuint pacmanDiffuse = ost::loadTexture("./textures/pacman.png");
    GLuint fontDiffuse   = ost::loadTexture("./textures/font512.png");

    const GLuint levelShaderProgram  = ost::loadShaderProgram("./shaders/general.vert", "./shaders/level.geo","./shaders/level.frag");
    const GLuint spriteShaderProgram = ost::loadShaderProgram("./shaders/general.vert", "./shaders/sprite.frag");
    const GLuint cheeseShaderProgram = ost::loadShaderProgram("./shaders/cheese.vert", "./shaders/cheese.frag");
    const GLuint fontShaderProgram   = ost::loadShaderProgram("./shaders/general.vert", "./shaders/font.frag");


    LOG_INFO("INIT LEVEL SHADER");
    Shader::Shader levelShader;
    {
        levelShader = Shader::makeShader_VBO(levelShaderProgram, GL_STATIC_DRAW, GL_POINTS);
        level.bindBufferVertices(  newMesh(levelShader, level.vertices.size())   );
    }


    LOG_INFO("INIT SPRITE SHADER");
    Shader::Shader          spriteShader;
    ost::Pacman             pacman;
    std::vector<ost::Ghost> ghosts;
    {
        auto   pacmanUV      = ost::makeSpriteUVCoordinates(4,4,16, {5.5f, 6.0f},{278.0f, 278.0f},{439.0f, 289.0f});
        auto   ghostUV       = ost::makeSpriteUVCoordinates(2,4,8, {295.0f, 6.0f},{144.0f, 278.0f}, {439.0f, 289.0f});

        glm::ivec2 pacmanStart  = level.pacmanSpawnTile;
        glm::ivec2* ghostStarts = level.ghostSpawnTiles.data();

        size_t rectVertexCount  = 4;
        size_t rectElementCount = 6;
        size_t ghostCount       = level.ghostSpawnTiles.size();


        spriteShader = Shader::makeShader_VBO_EBO_TEX(spriteShaderProgram, pacmanDiffuse, GL_STREAM_DRAW, GL_TRIANGLES);

        auto pacmanMesh = newMesh(spriteShader, rectVertexCount, rectElementCount);
        pacman = ost::Pacman{
            pacmanMesh,
            pacmanStart,
            pacmanUV
        };

        for (size_t i = 0; i < ghostCount; ++i) {

            auto ghostMesh = newMesh(spriteShader, rectVertexCount, rectElementCount);
            ghosts.push_back(ost::Ghost{
                ghostMesh,
                ghostStarts[i],
                ghostUV
            });
        }
    }





    LOG_INFO("INIT CHEESE SHADER");
    Shader::Shader           cheeseShader;
    std::vector<ost::Cheese> cheeses;
    {
        const glm::vec2 cheeseOffset = glm::vec2(0.5f,-0.5f);
        const size_t    cheeseVertexCount = 1;

        cheeseShader = Shader::makeShader_VBO(cheeseShaderProgram, GL_STATIC_DRAW, GL_POINTS);

        for (const auto v : level.vertices) {
            cheeses.push_back(ost::Cheese{
                newMesh(cheeseShader, cheeseVertexCount),
                v + cheeseOffset
            });
        }
    }



    LOG_INFO("INIT FONT SHADER");
    Shader::Shader fontShader;
    {
        fontShader = Shader::makeShader_VBO_EBO_TEX(fontShaderProgram, fontDiffuse, GL_STREAM_DRAW, GL_TRIANGLES);

        auto makeText = [&](std::string text, glm::vec2 textPos){

            const auto   textUV = ost::makeSpriteUVCoordinates(16,16, 128, {0, 0}, {511, 511}, {511, 511});
            const size_t letterVertexCount  = 4;
            const size_t letterElementCount = 6;
            const auto   textSize    = glm::vec2{ 1.2 , 1.2};

            return ost::Text{
                Shader::newMesh(fontShader, letterVertexCount * text.size(), letterElementCount * text.size()),
                textPos,
                textSize,
                textUV,
                text
            };
        };

        std::vector<ost::Text> textElements;
        textElements.resize(6);

        textElements[ost::UI_SCORE]             =  makeText( "Score: 000",                  glm::vec2{ 14,32}  );
        textElements[ost::UI_LIVES]             =  makeText( "Lives: 03",                   glm::vec2{ 1, 32}  );
        textElements[ost::UI_MENU_ITEM_PAUSE]   =  makeText( "----    PAUSE     ----",      glm::vec2{ 0, 19}  );
        textElements[ost::UI_MENU_ITEM_RESUME]  =  makeText( "        RESUME        " ,     glm::vec2{ 0, 17}  );
        textElements[ost::UI_MENU_ITEM_RESTART] =  makeText( "        RESTART       " ,     glm::vec2{ 0, 16}  );
        textElements[ost::UI_MENU_ITEM_QUIT]    =  makeText( "        QUIT          ",      glm::vec2{ 0, 13}  );

        userInterface = ost::UserInterface{ textElements };
    }

    //
    // INIT BUFFERS - based on the meshes that where allocated(newed) above
    //
    {

        Shader::initBuffers_VBO( levelShader );
        Shader::setUniformMat4(levelShader, "scale", level.scaleMatrix);
        Shader::setUniformMat4(levelShader, "move", level.moveMatrix);

        const Color FLOOR  = {1.0f, .7f, .8f, 1.0f};

        Shader::setUniformFloat(levelShader, "quadSize",     2.0f/level.biggestSize);
        Shader::setUniformVec4(levelShader,  "floor_color", {FLOOR.r, FLOOR.g,FLOOR.b,FLOOR.a});

        Shader::initBuffers_VBO( cheeseShader );
        Shader::setUniformMat4(cheeseShader, "scale", level.scaleMatrix);
        Shader::setUniformMat4(cheeseShader, "move", level.moveMatrix);
        Shader::setUniformFloat(cheeseShader, "pointSize", 5.0f);

        Shader::initBuffers_VBO_EBO_TEX( spriteShader );
        Shader::setUniformMat4(spriteShader, "scale", level.scaleMatrix);
        Shader::setUniformMat4(spriteShader, "move", level.moveMatrix);

        Shader::initBuffers_VBO_EBO_TEX( fontShader );
        Shader::setUniformMat4(fontShader, "scale", level.scaleMatrix);
        Shader::setUniformMat4(fontShader, "move", level.moveMatrix);
    }

    //
    // PAUSE LOOP
    //
    auto startPause = [&](){
        double pausetime = glfwGetTime();

        userInterface.showMenu();

        while (ost::pause) {
            glfwPollEvents();
            render(window, levelShader, spriteShader, cheeseShader, fontShader);
            if (( glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)) break;
        }

        userInterface.hideMenu();
        glfwSetTime(pausetime);
    };
    //
    // GAMELOOP
    //
    while (ost::running) {


        ost::running = update(window, pacman, level, ghosts, cheeses);
        render(window, levelShader, spriteShader, cheeseShader, fontShader);

        if (ost::pause) startPause();
    }

    //
    // CLEANUP
    //
    glfwDestroyWindow(window);
    glfwTerminate();

    return ost::restart ? _1337 : 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    key = (action==GLFW_PRESS)?key:0;
    //LOG_DEBUG("key: %d  scancode: %d  action: %d   mods: %d", key, scancode, action, mods);

    auto actionQuit = [](){
        LOG_INFO("QUITTING GAME");

        ost::restart = false;
        ost::running = false;
        ost::pause   = false;
    };

    auto actionRestart = [](){
        LOG_INFO("RESTARTING GAME");

        ost::restart = true;
        ost::running = false;
        ost::pause   = false;
    };

    auto actionResume = [](){
        LOG_INFO("RESUMING GAME");
        ost::running = true;
        ost::pause   = false;
    };

    switch(key)
    {
        case GLFW_KEY_SPACE:{
            ost::pause = (ost::pause)? 0:1;
            break;
        }
        case GLFW_KEY_ESCAPE:
            actionQuit();
            break;

        case GLFW_KEY_UP:
            userInterface.menuUp();
            break;

        case GLFW_KEY_DOWN:
            userInterface.menuDown();
            break;

        case GLFW_KEY_R:
            actionRestart();
            break;

        case GLFW_KEY_ENTER: {
            if (userInterface.menuIndex == ost::UI_MENU_ITEM_RESUME) {
                actionResume();
            } else if (userInterface.menuIndex == ost::UI_MENU_ITEM_QUIT) {
                actionQuit();
            } else if (userInterface.menuIndex == ost::UI_MENU_ITEM_RESTART) {
                actionRestart();
            }
            break;
        }

    }

}

inline GLFWwindow* init_GLFW_GLEW_OPENGL(const int openglMajor, const int openglMinor, const int wwidth, const int wheight, const char* wname)
{
    // init G L F W
    if (!glfwInit()){
        glfwTerminate();
        LOG_ERROR("Failed to init GLFW");
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
        LOG_ERROR("Failed to open GLFW window");
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);

    // init G L E W
    glewExperimental = GL_TRUE;  // MACOS/intel cpu support
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        LOG_ERROR("Failed to init GLEW");
    }
    glfwSetKeyCallback(window, key_callback);

    // INIT OPENGL
    {
        const Color BACKGROUND = {.3f, .9f, .3f, 1.0f};

        glEnable(GL_PROGRAM_POINT_SIZE);
        glClearColor(BACKGROUND.r,BACKGROUND.g,BACKGROUND.b,BACKGROUND.a);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    return window;
}


inline bool update(GLFWwindow* window, ost::Pacman& pacman, ost::Level& level, std::vector<ost::Ghost>& ghosts, std::vector<ost::Cheese>& cheeses)
{
    glfwPollEvents();

    // Configure delta time
    static double baseTime = glfwGetTime();
    double deltaTime = glfwGetTime() - baseTime;
    baseTime = glfwGetTime();



      // UPDATE
    // 1. MOVE ANIMATE PACMAN -  W, A, S, D
    {
        pacman.move(deltaTime, level.grid);
        pacman.animate(baseTime);
        pacman.tickInvincibility(deltaTime);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)   {
            pacman.towards(ost::vecUp, level.grid);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  {
            pacman.towards(ost::vecDown, level.grid);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ) {
            pacman.towards(ost::vecLeft, level.grid);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ){
            pacman.towards(ost::vecRight, level.grid);
        }

        for(auto& portal : level.portals) {
            if(portal.tryTeleport(pacman)) {
                portal.teleport(pacman);
            }
        }
    }
    // 2. MOVE GHOSTS
    {
        for(auto& ghost : ghosts) {
            ghost.move(deltaTime, level.grid);
            ghost.animate(baseTime);
            if(ghost.tryAttack(pacman)) {
                pacman.damage();
            }
        }

        const  double step = 0.08;
        static double randomEventLimit = step;

        if (glfwGetTime() > randomEventLimit) {
            int x = (rand() % 2) ? 0 : 1;  // Random x is 0 or 1
            int y = 0;
            if (x)
                x = (rand() % 2)? x*-1: x;  // If x is 1, random 1 or -1
            else {
                y = 1;                     // if x is 0, y = 1
                y = (rand() %2)? y*-1:y;   // random y is 1 or -1
            }

            ghosts[rand() % ghosts.size()].towards(glm::ivec2{x, y}, level.grid);
            randomEventLimit += step;
        }
    }
    // 3. INCREMENT HIGH SCORE
    {
        userInterface.setScore(pacman.score);
        userInterface.setLives(pacman.lives);
        if(pacman.lives <= 0) {
            ost::pause = true; //@TODO open the restart/quit menu instead!
        }
    }
    // 4. DELETE CHEESE -
    {
        for(auto& cheese : cheeses) {
            if (cheese.enabled && cheese.tryGetEatenBy(pacman)) {
                pacman.addScore(1);
            }
        }
    }

    return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}


inline void render(GLFWwindow* window, Shader::Shader& levelShader, Shader::Shader& spriteShader, Shader::Shader& cheeseShader, Shader::Shader& fontShader)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Shader::setUniformFloat(levelShader,  "time", glfwGetTime());
    Shader::setUniformFloat(cheeseShader, "time", glfwGetTime());
    // Shader::setUniformFloat(spriteShader, "time", glfwGetTime());
    // Shader::setUniformFloat(fontShader,   "time", glfwGetTime());

    Shader::drawVBO(levelShader);
    Shader::drawVBO(cheeseShader);
    Shader::drawVBO_EBO_TEX(spriteShader);
    Shader::drawVBO_EBO_TEX(fontShader);

    glfwSwapBuffers(window);
}
