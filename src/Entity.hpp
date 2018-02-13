#pragma once
#include <vector>
#include <cmath>
#include "glm/glm.hpp"
#include "./spritesheet.hpp"
#include "./Shader.hpp"

namespace ost {

enum Direction : int {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

const auto vecUp    = glm::vec2(0.0f, 1.0f);
const auto vecDown  = glm::vec2(0.0f, -1.0f);
const auto vecRight = glm::vec2(-1.0f, 0.0f);
const auto vecLeft  = glm::vec2( 1.0f,  0.0f);

void bufferBindRect(const std::vector<ost::Vertex>::iterator vertIt, 
                    const std::vector<int>::iterator eleIt,
                    const int elementOffset,
                    const glm::vec2 pos,
                    const glm::vec2 size,
                    const ost::Rect uv) 
{
    vertIt[0].position = pos;
   
    vertIt[1].position = pos + glm::vec2{ size.x, 0.0f};
    vertIt[2].position = pos + glm::vec2{ size.x, -size.y };
    vertIt[3].position = pos + glm::vec2{ 0.0f, -size.y };
    
    vertIt[0].texCoord = uv.topleft;
    vertIt[1].texCoord = uv.topright;
    vertIt[2].texCoord = uv.botright;    
    vertIt[3].texCoord = uv.botleft;

    eleIt[0] = elementOffset+0;   
    eleIt[1] = elementOffset+1;
    eleIt[2] = elementOffset+2;
    eleIt[3] = elementOffset+2;
    eleIt[4] = elementOffset+3;
    eleIt[5] = elementOffset+0;
}


void bufferUpdateRect(const std::vector<ost::Vertex>::iterator vertIt,
                      const glm::vec2 pos,
                      const glm::vec2 size,
                      const ost::Rect uv) 
{
    vertIt[0].position = pos;
    vertIt[1].position = pos + glm::vec2{ size.x, 0.0f};
    vertIt[2].position = pos + glm::vec2{ size.x, -size.y };
    vertIt[3].position = pos + glm::vec2{ 0.0f, -size.y };
    
    vertIt[0].texCoord = uv.topleft;
    vertIt[1].texCoord = uv.topright;
    vertIt[2].texCoord = uv.botright;    
    vertIt[3].texCoord = uv.botleft;
}

struct Pacman {

    enum PacmanFrame : int {
        PACMAN_DOWN0,  PACMAN_DOWN1, PACMAN_DOWN2, PACMAN_DOWN3,
        PACMAN_UP0,    PACMAN_UP1,  PACMAN_UP2, PACMAN_UP3,
        PACMAN_LEFT0,  PACMAN_LEFT1, PACMAN_LEFT2, PACMAN_LEFT3,
        PACMAN_RIGHT0, PACMAN_RIGHT1, PACMAN_RIGHT2, PACMAN_RIGHT3,
    };
    
    const std::vector<ost::Rect> uv;
    const glm::vec2             size = { 1.0f, 1.0f };
    const std::vector<ost::Vertex>::iterator vertexBufferIt;
    const std::vector<int>::iterator         elementBufferIt;
    const int bufferOffset;
    

    glm::vec2                    pos;
    int                          animationFrame;
    glm::vec2                    velocity       = { 2.0f, 0.0f};
    Direction                    direction      = RIGHT;
  
    Pacman(const std::vector<ost::Vertex>::iterator _vertexBuffer,
           const std::vector<int>::iterator         _elementBuffer,
           const int _bufferOffset,
           const glm::vec2 _pos
           ) 

    :vertexBufferIt(_vertexBuffer)
    ,elementBufferIt(_elementBuffer)
    ,bufferOffset(_bufferOffset)
    ,pos(_pos)
    ,uv(ost::makeSpriteUVCoordinates(4,4,16, {5.5f, 6.0f},{278.0f, 278.0f},{439.0f, 289.0f}))
    ,animationFrame(PACMAN_RIGHT3)
    {
        bufferBindRect(vertexBufferIt, elementBufferIt, bufferOffset, pos, size, uv[animationFrame]);
    }
  
    void move(const float dt) {

    }

    void towards(const Direction wantedDirection, const Level& level) {

    }

    void animate(const float dt) {
        const float frameTimeLimit = .1f;
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

            switch(direction) {
                case UP: {
                    animationFrame = PACMAN_UP0 + offset;
                    break;
                }
                case DOWN: {
                    animationFrame = PACMAN_DOWN0 + offset;
                    break;
                }
                case LEFT: {
                    animationFrame = PACMAN_LEFT0 + offset;
                    break;
                }
                case RIGHT: {
                    animationFrame = PACMAN_RIGHT0 + offset;
                    break;
                }
            }
            bufferUpdateRect(vertexBufferIt, pos, size, uv[animationFrame]);
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

    const std::vector<ost::Rect> uv;
    const glm::vec2              size = { 1.0f, 1.0f };
    const std::vector<ost::Vertex>::iterator vertexBufferIt;
    const std::vector<int>::iterator         elementBufferIt;
    const int bufferOffset;

    glm::vec2                    pos;
    int                          animationFrame = 0;
    glm::vec2                    velocity       = { 0.0f, 0.0f};
    Direction                    direction      = RIGHT;

    Ghost(const std::vector<ost::Vertex>::iterator _vertexBuffer,
          const std::vector<int>::iterator         _elementBuffer,
          const int _bufferOffset,
          const glm::vec2 _pos
          ) 

    :vertexBufferIt(_vertexBuffer)
    ,elementBufferIt(_elementBuffer)
    ,bufferOffset(_bufferOffset)
    ,uv(ost::makeSpriteUVCoordinates(2,4,8, {295.0f, 6.0f},{144.0f, 278.0f}, {439.0f, 289.0f}))
    ,pos(_pos)
    ,animationFrame(GHOST_DOWN0)
    {
        bufferBindRect(vertexBufferIt, elementBufferIt, bufferOffset, pos, size, uv[animationFrame]);
    }

    void animate(const float dt) {
        const float frameTimeLimit = .05f;
        static float deltaFrameTime = 0.0f;

        deltaFrameTime += dt;

        if (deltaFrameTime > frameTimeLimit) {
            deltaFrameTime = 0.0f;
            const int offset = (animationFrame%2 == 1)?0:1;

            switch(direction) {
                case UP: {
                    animationFrame = GHOST_UP0 + offset;
                    break;
                }
                case DOWN: {
                    animationFrame = GHOST_DOWN0 + offset;
                    break;
                }
                case LEFT: {
                    animationFrame = GHOST_LEFT0 + offset;
                    break;
                }
                case RIGHT: {
                    animationFrame = GHOST_RIGHT0 + offset;
                    break;
                }
            }
            bufferUpdateRect(vertexBufferIt, pos, size, uv[animationFrame]);            
        }
    }

};


struct Cheese {

    const std::vector<ost::Vertex>::iterator vertexBufferIt;
    glm::vec2 pos;

    Cheese(const std::vector<ost::Vertex>::iterator _vertexBuffer,
           const glm::vec2 _pos
          ) 
    :vertexBufferIt(_vertexBuffer)
    ,pos(_pos)
    {
        vertexBufferIt->position = pos;
    }
};

} // END NAMESPACE OST
