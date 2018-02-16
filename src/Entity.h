#pragma once
struct Portal {
    glm::ivec2 entryDirection;
    glm::ivec2 tileIndex;

    Portal* destination = nullptr;
    glm::ivec2 exitDirection() {
        return -entryDirection;
    }
    bool tryTeleport(const Pacman& pacman);
    void teleport(Pacman& pacman);
};
