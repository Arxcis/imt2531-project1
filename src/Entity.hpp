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


struct Pacman {

    enum PacmanFrame : int {
        PACMAN_DOWN0,  PACMAN_DOWN1, PACMAN_DOWN2, PACMAN_DOWN3,
        PACMAN_UP0,    PACMAN_UP1,  PACMAN_UP2, PACMAN_UP3,
        PACMAN_LEFT0,  PACMAN_LEFT1, PACMAN_LEFT2, PACMAN_LEFT3,
        PACMAN_RIGHT0, PACMAN_RIGHT1, PACMAN_RIGHT2, PACMAN_RIGHT3,
    };
    
    const std::vector<ost::Rect> uv = ost::makeSpriteUVCoordinates(4,4,16, {5.5f, 6.0f},{278.0f, 278.0f},{439.0f, 289.0f});
    const glm::vec2              size = { 1.0f, 1.0f };
    const float                  speed = 3.0f;
    
    Mesh::Mesh mesh;
    glm::vec2    pos;
    int          animationFrame = PACMAN_RIGHT3;
    glm::ivec2   direction       = { 1, 0 };
  
    Pacman(Mesh::Mesh _mesh,
           const glm::vec2 _pos) 
    :mesh(_mesh)
    ,pos(_pos)
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


    void animate(const float dt) {
        const float frameTimeLimit = .017f*4;
        static float deltaFrameTime = 0.0f;

        deltaFrameTime += dt;

        if (deltaFrameTime > frameTimeLimit) {
            deltaFrameTime = 0.0f;

            const int framesPerDirection = 4;
            int offset = 0;
            offset = animationFrame % framesPerDirection;
            
            if (offset == 0) {
                offset = 3;
            } else {
                offset -= 1;
            }

            if(direction == ost::vecUp) {
                    animationFrame = PACMAN_UP0 + offset;
            }
            else if(direction == ost::vecDown) {
                    animationFrame = PACMAN_DOWN0 + offset;
            }
            else if (direction == ost::vecLeft) {
                    animationFrame = PACMAN_LEFT0 + offset;
            }
            else if (direction == ost::vecRight){
                    animationFrame = PACMAN_RIGHT0 + offset;
            }
            Mesh::updateRect(mesh, pos, size, uv[animationFrame]);
        }
    }
};

struct Ghost {
    enum GhostFrame : int {
        GHOST_DOWN0,  GHOST_DOWN1,
        GHOST_UP0,    GHOST_UP1,
        GHOST_LEFT0,  GHOST_LEFT1,
        GHOST_RIGHT0, GHOST_RIGHT1,
    };

    const std::vector<ost::Rect> uv = ost::makeSpriteUVCoordinates(2,4,8, {295.0f, 6.0f},{144.0f, 278.0f}, {439.0f, 289.0f});
    const glm::vec2              size = { 1.0f, 1.0f };
    const float                  speed = 3.0f;
    Mesh::Mesh    mesh;
    glm::vec2       pos;
    int             animationFrame = GHOST_DOWN0;
    glm::ivec2      direction      = { 1, 0};

    Ghost(Mesh::Mesh  _mesh,
          const glm::vec2 _pos) 
    :mesh(_mesh)
    ,pos(_pos)
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

    void animate(const float dt) {
        const float frameTimeLimit = .017f*4;
        static float deltaFrameTime = 0.0f;

        deltaFrameTime += dt;

        if (deltaFrameTime > frameTimeLimit) {
            deltaFrameTime = 0.0f;
            const int offset = (animationFrame%2 == 1)?0:1;

            if(direction == ost::vecUp) {
                    animationFrame = GHOST_UP0 + offset;
            }
            else if(direction == ost::vecDown) {
                    animationFrame = GHOST_DOWN0 + offset;
            }
            else if (direction == ost::vecLeft) {
                    animationFrame = GHOST_LEFT0 + offset;
            }
            else if (direction == ost::vecRight){
                    animationFrame = GHOST_RIGHT0 + offset;
            }
            Mesh::updateRect(mesh, pos, size, uv[animationFrame]);            
        }
    }

};

struct Cheese {
    Mesh::Mesh mesh;
    glm::vec2 pos;

    Cheese(Mesh::Mesh _mesh,
           const glm::vec2 _pos
          ) 
    :mesh(_mesh)
    ,pos(_pos)
    {
        mesh.VBO[0].position = pos;
    }
};

} // END NAMESPACE OST
