#pragma once
#include <vector>
#include <cmath>
#include "glm/glm.hpp"
#include "./spritesheet.hpp"

namespace ost {

enum PacmanFrame : int {
    PACMAN_DOWN3,  PACMAN_DOWN2, PACMAN_DOWN1, PACMAN_DOWN0,
    PACMAN_UP3,    PACMAN_UP2, PACMAN_UP1, PACMAN_UP0,
    PACMAN_LEFT3,  PACMAN_LEFT2, PACMAN_LEFT1, PACMAN_LEFT0,
    PACMAN_RIGHT3, PACMAN_RIGHT2, PACMAN_RIGHT1, PACMAN_RIGHT0,
};

enum GhostFrame : int {
    GHOST_DOWN0,  GHOST_DOWN1,
    GHOST_UP0,    GHOST_UP1,
    GHOST_LEFT0,  GHOST_LEFT1,
    GHOST_RIGHT0, GHOST_RIGHT1,
};

struct Entity {
    enum Direction : int {
        UP,
        DOWN, 
        LEFT, 
        RIGHT,
    };

    const std::vector<ost::Rect> uv;

    glm::vec2 pos  = {-.9f, .1f};
    glm::vec2 size = { .1f, .1f};
    glm::vec2 velocity = {.1f, 0.0f};
    int       frame = PACMAN_RIGHT0;
    
    Entity(const std::vector<ost::Rect> _uv)
    :uv(_uv)
    {}

    void move(const float dt) {
        pos += glm::vec2{ velocity.x*dt, velocity.y*dt};
    }

    void towards(const Direction direction) {
        switch(direction) {
            case UP:
                velocity.y = std::abs(velocity.x) + std::abs(velocity.y);
                velocity.x = 0.0f;
            case DOWN:
                velocity.y = - (std::abs(velocity.x) + std::abs(velocity.y));
                velocity.x = 0.0f;
            case LEFT:
                velocity.x = - (std::abs(velocity.x) + std::abs(velocity.y));
                velocity.y = 0.0f;
            case RIGHT:
                velocity.x = (std::abs(velocity.x) + std::abs(velocity.y));
                velocity.y = 0.0f;
        }
    }

    std::vector<float> getBuffer() {
        return {
            pos.x,         pos.y,        uv[frame].topleft.x, uv[frame].topleft.y, 
            pos.x+size.x,  pos.y,        uv[frame].topright.x, uv[frame].topright.y,
            pos.x+size.x,  pos.y-size.y, uv[frame].botright.x, uv[frame].botright.y,
            pos.x,         pos.y-size.y, uv[frame].botleft.x, uv[frame].botleft.y
        };
    }
};

struct Pacman : public Entity {

    Pacman() : Entity(ost::makeSpriteUVCoordinates(4,4,16, {5.5f, 6.0f}, 
                                                       {278.0f, 278.0f},
                                                       {439.0f, 289.0f}))
    {}
};

struct Ghost : public Entity {

    Ghost() : Entity(ost::makeSpriteUVCoordinates(2,4,8, {295.0f, 6.0f},
                                                       {144.0f, 278.0f},
                                                       {439.0f, 289.0f})) 
    {} 
};

} 