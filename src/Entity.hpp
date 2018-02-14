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
};

struct Pacman {

    enum PacmanFrame : int {
        PACMAN_DOWN0,  PACMAN_DOWN1, PACMAN_DOWN2, PACMAN_DOWN3,
        PACMAN_UP0,    PACMAN_UP1,  PACMAN_UP2, PACMAN_UP3,
        PACMAN_LEFT0,  PACMAN_LEFT1, PACMAN_LEFT2, PACMAN_LEFT3,
        PACMAN_RIGHT0, PACMAN_RIGHT1, PACMAN_RIGHT2, PACMAN_RIGHT3,
    };
    
    std::vector<ost::Rect> uv;
    glm::vec2              size = { 1.0f, 1.0f };
    float                  speed = 3.0f;
    
    Mesh::Mesh   mesh{};
    glm::vec2    pos{};
    int          animationFrame = PACMAN_RIGHT3;
    glm::ivec2   direction      = { 1, 0 };
    double nextUpdateTime = 0.0;
    
    Pacman() = default;
    Pacman(const Pacman& pacman) :mesh(pacman.mesh), pos(pacman.pos),uv(pacman.uv){}
    Pacman& operator=(const Pacman& other) = default;

    Pacman(Mesh::Mesh _mesh, const glm::vec2 _pos, std::vector<ost::Rect> _uv) 
    :mesh(_mesh)
    ,pos(_pos)
    ,uv(_uv)
    {
        Mesh::bindRect(mesh, pos, size, uv[animationFrame]);
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
            Mesh::updateRect(mesh, pos, size, uv[animationFrame]);
        }
    }
};

struct Ghost {
    enum GhostFrame : int {
        GHOST_DOWN0,  GHOST_DOWN1,
        GHOST_UP0,    GHOST_UP1,
        GHOST_LEFT0,  GHOST_LEFT1,
        GHOST_RIGHT0, GHOST_RIGHT1
    };

    std::vector<ost::Rect> uv;
    glm::vec2              size  = { 1.0f, 1.0f };
    float                  speed = 3.0f;
    Mesh::Mesh      mesh{};
    glm::vec2       pos{};
    int             animationFrame = GHOST_DOWN0;
    glm::ivec2      direction      = { 1, 0};
    double          nextUpdateTime = 0.0;
    

    Ghost() = default;    
    Ghost(const Ghost& ghost):mesh(ghost.mesh),pos(ghost.pos),uv(ghost.uv){}
    Ghost& operator=(const Ghost& other) = default;

    Ghost(Mesh::Mesh _mesh, glm::vec2 _pos, std::vector<ost::Rect> _uv) 
    :mesh(_mesh)
    ,pos(_pos)
    ,uv(_uv)
    {
        Mesh::bindRect(mesh, pos, size, uv[animationFrame]);
    }

    void move(const float dt, const Grid& grid) 
    {
        if (Level::canWalkToward(grid, pos, size, direction))
            pos += glm::vec2{direction} * dt * speed;
            
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
            
            Mesh::updateRect(mesh, pos, size, uv[animationFrame]);            
        }
    }
};


struct Cheese {
    Mesh::Mesh mesh;
    glm::vec2 pos;

    Cheese(Mesh::Mesh _mesh, glm::vec2 _pos) 
    :mesh(_mesh)
    ,pos(_pos)
    {
        (*mesh.VBO)[mesh.VBOindex].position = pos;
    }
};

struct Font {

};


} // END NAMESPACE OST
