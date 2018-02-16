#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "./logger.h"

namespace Shader {
    using  Element = int;

    struct Vertex
    {
        glm::vec2 position{};
        glm::vec4 color{};
        glm::vec2 texCoord{};
    };
}

namespace Mesh {
    struct Mesh;

    struct Mesh
    {
        std::vector<Shader::Vertex>::iterator  VBObegin;
        std::vector<Shader::Vertex>::iterator  VBOend;
        size_t VBOindex;
        size_t VBOcount;

        std::vector<Shader::Element>::iterator EBObegin;
        std::vector<Shader::Element>::iterator EBOend;
        size_t EBOindex;
        size_t EBOcount;
    };
}

namespace ost {

enum TileType : int;
using Grid = const std::vector<std::vector<TileType>>;
struct Rect;
struct Portal;
struct Pacman;



struct Rect {
    glm::vec2 topleft;
    glm::vec2 topright;
    glm::vec2 botright;
    glm::vec2 botleft;
};

struct Portal {
    glm::ivec2 entryDirection;
    glm::ivec2 tileIndex;

    Portal* destination = nullptr;
    glm::ivec2 exitDirection() {
        auto exitDir = -glm::normalize(glm::vec2{entryDirection});
        return exitDir;
    }
    bool tryTeleport(const Pacman& pacman);
    void teleport(Pacman& pacman);
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
    int                   score = 0;

    void addScore(int points) {
        score += points;
    }

    void damage()
    {
        if(invincibleTime > 0)
            return;
        lives--;
        LOG_INFO("Pacman.lives: %d", lives);
        invincibleTime = 2.0; //MAGIC NUMBER
    }


    void tickInvincibility(const double dt) {
        invincibleTime -= dt;
    }

    void bind();
    void move(const float dt, const ost::Grid& grid);
    void towards(const glm::ivec2 _wantedDirection, const ost::Grid& grid);
    void animate(const double gameTime);
};

}
