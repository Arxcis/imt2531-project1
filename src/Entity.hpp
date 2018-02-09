#pragma once
#include <vector>
#include <cmath>
#include "glm/glm.hpp"
#include "./spritesheet.hpp"

namespace ost {

enum PacmanFrame : int {
    PACMAN_DOWN0,  PACMAN_DOWN1, PACMAN_DOWN2, PACMAN_DOWN3,
    PACMAN_UP0,    PACMAN_UP1,  PACMAN_UP2, PACMAN_UP3,
    PACMAN_LEFT0,  PACMAN_LEFT1, PACMAN_LEFT2, PACMAN_LEFT3,
    PACMAN_RIGHT0, PACMAN_RIGHT1, PACMAN_RIGHT2, PACMAN_RIGHT3,
};

enum GhostFrame : int {
    GHOST_DOWN0,  GHOST_DOWN1,
    GHOST_UP0,    GHOST_UP1,
    GHOST_LEFT0,  GHOST_LEFT1,
    GHOST_RIGHT0, GHOST_RIGHT1,
};

struct Entity {
    Entity(const std::vector<ost::Rect> _uv)
    :uv(_uv)
    {}
    //
    // BUFFER COMPONENT
    //
    const std::vector<ost::Rect> uv;
    glm::vec2 pos  = {-.9f, .1f};
    glm::vec2 size = { .1f, .1f};
    int animationFrame = PACMAN_RIGHT0;
   
    std::vector<float> getBuffer() const {
        ost::Rect _uv = uv[animationFrame];
        return {
            pos.x,         pos.y,        _uv.topleft.x,  _uv.topleft.y, 
            pos.x+size.x,  pos.y,        _uv.topright.x, _uv.topright.y,
            pos.x+size.x,  pos.y-size.y, _uv.botright.x, _uv.botright.y,
            pos.x,         pos.y-size.y, _uv.botleft.x,  _uv.botleft.y
        };
    }

    enum Direction : int {
        UP,
        DOWN, 
        LEFT, 
        RIGHT,
    };

    //
    // GAME COMPONENT
    //
    glm::vec2 velocity = {.2f, 0.0f};
    Direction direction = RIGHT;
    
    void move(const float dt) {
        pos += glm::vec2{ velocity.x*dt, velocity.y*dt};
    }


    void towards(const Direction _direction) {

        if (_direction == UP && direction != UP) {
            velocity.y = std::abs(velocity.x) + std::abs(velocity.y);
            velocity.x = 0.0f;
            direction = UP;
        } else if (_direction == DOWN && direction != DOWN) {
            velocity.y = - (std::abs(velocity.x) + std::abs(velocity.y));
            velocity.x = 0.0f;
            direction = DOWN;
        } else if (_direction == LEFT && direction != LEFT) {
            velocity.x = - (std::abs(velocity.x) + std::abs(velocity.y));
            velocity.y = 0.0f;
            direction = LEFT;
        } else if (_direction == RIGHT && direction != RIGHT) {
            velocity.x = (std::abs(velocity.x) + std::abs(velocity.y));
            velocity.y = 0.0f;
            direction = RIGHT;
        }        

        //std::cout << "Direction: " << direction << " velocity: " << velocity.x << ", " << velocity.y << '\n'; //@debug
     }
    virtual void animate(const float dt) {};    
};

struct Pacman : public Entity {

    Pacman() : Entity(ost::makeSpriteUVCoordinates(4,4,16, {5.5f, 6.0f}, 
                                                       {278.0f, 278.0f},
                                                       {439.0f, 289.0f}))
    {
        animationFrame = PACMAN_RIGHT0;
    }

    void animate(const float dt) override {
        const float frameTimeLimit = .1f;
        static float deltaFrameTime = 0.0f;
        
        deltaFrameTime += dt;

        if (deltaFrameTime > frameTimeLimit) {
            deltaFrameTime = 0.0f;

            const int framesPerDirection = 4;
            int offset = (animationFrame+1) % framesPerDirection;            
            offset = (framesPerDirection-1) - offset;   // Reverse animation 
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
                    //std::cout << "RIGHT! dir:" << direction << " frame: " << animationFrame << " offset: " << offset <<'\n'; // @debug
                    animationFrame = PACMAN_RIGHT0 + offset;   
                    break;
                }
            } 
        }
    }
};

struct Ghost : public Entity {

    Ghost() : Entity(ost::makeSpriteUVCoordinates(2,4,8, {295.0f, 6.0f},
                                                       {144.0f, 278.0f},
                                                       {439.0f, 289.0f})) 
    {
        animationFrame = GHOST_DOWN0;
    } 

    void animate(const float dt) override {
        const float frameTimeLimit = .05f;
        static float deltaFrameTime = 0.0f;
        
        deltaFrameTime += dt;

        if (deltaFrameTime > frameTimeLimit) {
            deltaFrameTime = 0.0f;

            const int framesPerDirection = 2;
            const int offset = ((animationFrame+1)%framesPerDirection);

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
                }
            } 
        }
    }

};


struct Dot : public Entity {

    Dot(glm::vec2 _position): Entity(
        ost::makeSpriteUVCoordinates(1,1,1,{219.0f, 9.0f},
                                           {61.0f, 61.0f},
                                           {439.0f, 289.0f}))
    {
        pos = _position;
    }
    bool active = true;
};

std::vector<ost::Dot> makeDots(const std::vector<glm::vec2> levelVertices) {

    std::vector<ost::Dot> dotvector{};
    for (const auto& v : levelVertices) {
        dotvector.push_back(ost::Dot{ v });
    }
    return dotvector;
}
} 