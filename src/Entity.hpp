#pragma once
#include <vector>
#include <cmath>
#include "glm/glm.hpp"
#include "./spritesheet.hpp"
#include "./Shader.hpp"
#include "./Level.hpp"


namespace ost {

const auto vecUp    = glm::ivec2(0, 1);
const auto vecDown  = glm::ivec2(0,-1);
const auto vecRight = glm::ivec2(1, 0);
const auto vecLeft  = glm::ivec2(-1,0);

inline int getNextAnimationFrame(const int currentFrame, const glm::ivec2 direction, const int FRAME_UP,const int FRAME_DOWN, const int FRAME_LEFT, const int FRAME_RIGHT, const int framesPerDirection)
{
    int offset = currentFrame % framesPerDirection;

    if (offset == 0) {
        offset = framesPerDirection-1;
    } else {
        offset -= 1;
    }

    if (direction == vecUp) {
            return FRAME_UP + offset;
    }
    else if (direction == vecDown) {
            return FRAME_DOWN + offset;
    }
    else if (direction == vecLeft) {
            return FRAME_LEFT + offset;
    }
    else if (direction == vecRight){
            return FRAME_RIGHT + offset;
    }
    return FRAME_UP;
};

struct Pacman
{
    enum PacmanFrame : int {
        PACMAN_DOWN0,  PACMAN_DOWN1, PACMAN_DOWN2, PACMAN_DOWN3,
        PACMAN_UP0,    PACMAN_UP1,  PACMAN_UP2, PACMAN_UP3,
        PACMAN_LEFT0,  PACMAN_LEFT1, PACMAN_LEFT2, PACMAN_LEFT3,
        PACMAN_RIGHT0, PACMAN_RIGHT1, PACMAN_RIGHT2, PACMAN_RIGHT3,
    };

    Mesh::Mesh             mesh;
    glm::vec2              pos;
    std::vector<ost::Rect> uv;

    glm::vec2              size = { 1.0f, 1.0f };
    float                 speed = 3.0f;
    int          animationFrame = PACMAN_RIGHT3;
    glm::ivec2        direction = { 1, 0 };
    double       nextUpdateTime = 0.0;
    double       invincibleTime = 0.0;
    int                   lives = 3;

    Pacman() = default;
    Pacman& operator=(const Pacman& other) = default;

    Pacman(Mesh::Mesh _mesh, const glm::vec2 _pos, std::vector<ost::Rect> _uv)
    :mesh(_mesh)
    ,pos(_pos)
    ,uv(_uv)
    {
        Mesh::bindRect(mesh, pos, size, uv[animationFrame], 0);
    }

    void damage() {
        if(invincibleTime > 0)
            return;
        lives--;
        LOG_DEBUG("Pacman.health--");
        invincibleTime = 2.0; //MAGIC NUMBER
    }

    void tickInvincibility(const double dt) {
        invincibleTime -= dt;
    }

    void move(const float dt, const Grid& grid) {
        if (Level::canWalkToward(grid, pos, size, direction))
            pos += glm::vec2{direction} * dt * speed;
    }

    void towards(const glm::ivec2 _wantedDirection, const Grid& grid) {

        if (_wantedDirection != direction) {
            if (Level::canChangeDirection(grid, pos, size, direction, _wantedDirection)){
                direction = _wantedDirection;

                pos = Level::getTileSnapPosition(pos, size);
            }
        }
    }

    void animate(const double gameTime)
    {
        const double  timeStep = .1;
        if (gameTime > nextUpdateTime) {
            nextUpdateTime += timeStep;
            animationFrame = getNextAnimationFrame(animationFrame, direction, PACMAN_UP0, PACMAN_DOWN0, PACMAN_LEFT0, PACMAN_RIGHT0, 4);
            Mesh::updateRect(mesh, pos, size, uv[animationFrame], 0);
        }
    }
};

struct Ghost
{
    enum GhostFrame : int {
        GHOST_DOWN0,  GHOST_DOWN1,
        GHOST_UP0,    GHOST_UP1,
        GHOST_LEFT0,  GHOST_LEFT1,
        GHOST_RIGHT0, GHOST_RIGHT1
    };

    Mesh::Mesh             mesh;
    glm::vec2              pos;
    std::vector<ost::Rect> uv;

    glm::vec2              size  = { 1.0f, 1.0f };
    float                  speed = 3.0f;
    int           animationFrame = GHOST_DOWN0;
    glm::ivec2         direction = { 1, 0};
    double        nextUpdateTime = 0.0;
    bool        onAttackCooldown = false;

    Ghost() = default;
    Ghost& operator=(const Ghost& other) = default;
    Ghost(Mesh::Mesh _mesh, glm::vec2 _pos, std::vector<ost::Rect> _uv)
    :mesh(_mesh)
    ,pos(_pos)
    ,uv(_uv)
    { Mesh::bindRect(mesh, pos, size, uv[animationFrame], 0); }

    void move(const float dt, const Grid& grid)
    {
        if (Level::canWalkToward(grid, pos, size, direction))
            pos += glm::vec2{direction} * dt * speed;
    }

    void attackCheck(Pacman& pacman) {
        if(Level::isInSameTile(pacman.pos, pacman.size, pos, size)) {
            if(!onAttackCooldown) {
                onAttackCooldown = true;
                pacman.damage();
            }
        } else {
            onAttackCooldown = false;
        }
    }

    void towards(const glm::ivec2 _wantedDirection, const Grid& grid)
    {
        if (_wantedDirection != direction) {
            if (Level::canChangeDirection(grid, pos, size, direction, _wantedDirection)){
                direction = _wantedDirection;
                pos = Level::getTileSnapPosition(pos, size);
            }
        }
    }

    void animate(const double gameTime)
    {
        const double  timeStep = .1;
        if (gameTime > nextUpdateTime) {
            nextUpdateTime += timeStep;
            animationFrame = getNextAnimationFrame(animationFrame, direction, GHOST_UP0, GHOST_DOWN0, GHOST_LEFT0, GHOST_RIGHT0, 2);

            Mesh::updateRect(mesh, pos, size, uv[animationFrame], 0);
        }
    }
};

struct Cheese
{
    Mesh::Mesh mesh;
    glm::vec2 pos;

    Cheese(Mesh::Mesh _mesh, glm::vec2 _pos)
    :mesh(_mesh)
    ,pos(_pos)
    {
        Mesh::bindPoint(mesh, pos);
    }
};

struct Text
{
    Mesh::Mesh             mesh;
    glm::vec2              pos;
    glm::vec2              size  = { 1.0f, 1.0f };
    std::vector<ost::Rect> uv;
    std::string            text;
    glm::vec4              color = {0,0,1,1};
    float                  margin = 0.1f;

    Text() = default;
    Text& operator=(const Text& other) = default;

    Text(Mesh::Mesh _mesh, glm::vec2 _pos, glm::vec2 _size, std::vector<ost::Rect> _uv, std::string _text)
    :mesh(_mesh)
    ,pos(_pos)
    ,size(_size)
    ,uv(_uv)
    ,text(_text)
    { 
        Mesh::bindText(mesh, pos, size, uv, text, margin, color);
    }

    void print()
    {
       // LOG_DEBUG("printing text with color: %.2f, %.2f, %.2f, %.2f,", color.x, color.y, color.z, color.w);
        Mesh::updateTextColor(mesh, text, color);
    }

    void setColor(glm::vec4 incolor) {
        color = glm::vec4{incolor.x, incolor.y, incolor.z, color.w};
    } 

    void hide() { 
        color = glm::vec4{color.x,color.y,color.z,0}; 
    }
    void show() { 
        color = glm::vec4{color.x,color.y,color.z,1}; 
    }
};

enum UIElement : int 
{
    UI_SCORE=0,
    UI_LIVES=1,
    UI_MENU_ITEM_PAUSE  = 2,

    UI_MENU_ITEM_MIN    = 3,
    UI_MENU_ITEM_RESUME = 3,
    UI_MENU_ITEM_RESTART= 4,
    UI_MENU_ITEM_QUIT   = 5,
    UI_MENU_ITEM_MAX    = 5,
};

struct UserInterface 
{
    std::vector<Text> UItext;
    size_t menuIndex = UI_MENU_ITEM_RESUME;

    glm::vec4 unselectColor = {0,0,1,1};
    glm::vec4 selectColor   = {1,0,0,1};


    UserInterface() = default;
    UserInterface& operator=(const UserInterface& other) = default;


    UserInterface(std::vector<Text> _uitext)
    :UItext(_uitext)
    {
        // Set the correct positions of all the UIElements
        hideMenu();
    }

    void refreshText() {
        for(auto text : UItext) {
            text.print();
        }
    }

    void setScore(size_t score) 
    {   
        UItext[UI_SCORE].text = "Score: 0" + std::to_string(score);
        UItext[UI_SCORE].print();
    }

    void setLives(size_t liveCount)
    {
        UItext[UI_LIVES].text = "Lives: 0" + std::to_string(liveCount);
        UItext[UI_LIVES].print();
    }

    void menuUp() 
    {   
        if (menuIndex == UI_MENU_ITEM_MIN)
            return;

        UItext[menuIndex].setColor(unselectColor);
        UItext[menuIndex].print();

        menuIndex -= 1;
        LOG_INFO("menuindex: %zu", menuIndex);

        UItext[menuIndex].setColor(selectColor);
        UItext[menuIndex].print();
    }

    void menuDown() 
    {
        if(menuIndex == UI_MENU_ITEM_MAX)
            return;

        UItext[menuIndex].setColor(unselectColor);
        UItext[menuIndex].print();

        menuIndex += 1;
        LOG_INFO("menuindex: %zu", menuIndex);

        UItext[menuIndex].setColor(selectColor);        
        UItext[menuIndex].print();

    }

    void showMenu() 
    {   
        UItext[menuIndex].setColor(unselectColor);
        UItext[menuIndex].setColor(selectColor);

        UItext[UI_MENU_ITEM_PAUSE].show();
        UItext[UI_MENU_ITEM_RESTART].show();
        UItext[UI_MENU_ITEM_RESUME].show();
        UItext[UI_MENU_ITEM_QUIT].show();

        refreshText();
    }

    void hideMenu() 
    {    
        UItext[UI_MENU_ITEM_PAUSE].hide();
        UItext[UI_MENU_ITEM_RESTART].hide();
        UItext[UI_MENU_ITEM_RESUME].hide();
        UItext[UI_MENU_ITEM_QUIT].hide();
        refreshText();
    }
};

} // END NAMESPACE OST
