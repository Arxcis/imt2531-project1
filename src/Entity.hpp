#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "./spritesheet.hpp"

namespace ost {

enum PacmanFrame : int {
    PACMAN_RIGHT0, PACMAN_RIGHT1, PACMAN_RIGHT2, PACMAN_RIGHT3,
    PACMAN_LEFT0,  PACMAN_LEFT1, PACMAN_LEFT2, PACMAN_LEFT3,
    PACMAN_UP0,    PACMAN_UP1, PACMAN_UP2, PACMAN_UP3,
    PACMAN_DOWN0,  PACMAN_DOWN1, PACMAN_DOWN2, PACMAN_DOWN3,
};

enum GhostFrame : int {
    GHOST_RIGHT0, GHOST_RIGHT1,
    GHOST_LEFT0,  GHOST_LEFT1,
    GHOST_UP0,    GHOST_UP1,
    GHOST_DOWN0,  GHOST_DOWN1,
};

struct Entity {
    const std::vector<ost::Rect> uv;

    Entity(std::vector<ost::Rect> _uv)
    :uv(_uv)
    {}

    glm::vec2 pos  = {-.1f, .1f};
    glm::vec2 size = { .1f, .1f};
    int frame = 0;
    
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