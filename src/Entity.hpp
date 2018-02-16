#pragma once
#include <vector>
#include <cmath>
#include "./spritesheet.hpp"
#include "Entity.h"
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

    void bind() {
        Mesh::bindRect(mesh, pos, size, uv[animationFrame], 0);
    }

    void move(const float dt, const ost::Grid& grid)
    {
        if (Level::canWalkToward(grid, pos, size, direction))
            pos += glm::vec2{direction} * dt * speed;
    }

    bool tryAttack(const Pacman& pacman)
    {
        if(Level::isInSameTile(pacman.pos, pacman.size, pos, size)) {
            if(!onAttackCooldown) {
                onAttackCooldown = true;
                return true;
            }
        } else {
            onAttackCooldown = false;
        }
        return false;
    }

    void towards(const glm::ivec2 _wantedDirection, const ost::Grid& grid)
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
    Mesh::Mesh      mesh;
    glm::vec2       pos;
    bool            enabled = true;
    const glm::vec2 size{0.5f,0.5f};

    void bind() {
        Mesh::bindPoint(mesh, pos, vec4(1.0f));
    }

    bool tryGetEatenBy(const Pacman& pacman)
    {
        if(Level::isInSameTile(pos, size, pacman.pos, pacman.size)) {
            enabled = false;
            Mesh::updatePoint(mesh, 0.0f); //disable rendering
            return true;
        }
        return false;
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

    void bind() {
        Mesh::bindText(mesh, pos, size, uv, text, margin, color);
    }

    void print()
    {
       // LOG_DEBUG("printing text with color: %.2f, %.2f, %.2f, %.2f,", color.x, color.y, color.z, color.w);
        Mesh::updateTextColor(mesh, text, color);
        Mesh::updateTextUV(mesh, text, uv);
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

class UserInterface
{
public:
    std::vector<Text> UItext;

    glm::vec4 unselectColor = {0,0,1,1};
    glm::vec4 selectColor   = {1,0,0,1};

    size_t menuIndex = UI_MENU_ITEM_RESUME;

    UserInterface() = default;
    UserInterface& operator=(const UserInterface& other) = default;


    UserInterface(std::vector<Text> _uitext)
    :UItext(_uitext)
    {
        hideMenu();
        for (auto& txt: UItext) {
            txt.bind();
        }
    }

    void refreshText() {
        for(auto text : UItext) {
            text.print();
        }
    }

    void setScore(size_t score)
    {
        char buffer[20];
        snprintf(buffer,20, "Score: %03lu", score);
        UItext[UI_SCORE].text = buffer;
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

inline void Pacman::bind() {
    Mesh::bindRect(mesh, pos, size, uv[animationFrame], 0);
}

inline void Pacman::move(const float dt, const ost::Grid& grid) {
    if (Level::canWalkToward(grid, pos, size, direction))
        pos += glm::vec2{direction} * dt * speed;
}

inline void Pacman::towards(const glm::ivec2 _wantedDirection, const ost::Grid& grid) {

    if (_wantedDirection != direction) {
        if (Level::canChangeDirection(grid, pos, size, direction, _wantedDirection)){
            direction = _wantedDirection;

            pos = Level::getTileSnapPosition(pos, size);
        }
    }
}

inline void Pacman::animate(const double gameTime)
{
    const double  timeStep = .1;
    if (gameTime > nextUpdateTime) {
        nextUpdateTime += timeStep;
        animationFrame = getNextAnimationFrame(animationFrame, direction, PACMAN_UP0, PACMAN_DOWN0, PACMAN_LEFT0, PACMAN_RIGHT0, 4);
        Mesh::updateRect(mesh, pos, size, uv[animationFrame], 0);
    }
}

} // END NAMESPACE OST
