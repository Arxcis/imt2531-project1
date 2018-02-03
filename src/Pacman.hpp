#pragma once
#include <vector>
#include "glm/glm.hpp"

namespace ost {

enum PacmanFrame {
    PF_RIGHT0, 
    PF_RIGHT1, 
    PF_RIGHT2, 
    PF_RIGHT3,
    PF_LEFT0, 
    PF_LEFT1, 
    PF_LEFT2, 
    PF_LEFT3,
    PF_UP0, 
    PF_UP1, 
    PF_UP2, 
    PF_UP3,
    PF_DOWN0, 
    PF_DOWN1, 
    PF_DOWN2, 
    PF_DOWN3,
};

enum GhostFrame {
    GF_RIGHT1, 
    GF_RIGHT2,
    GF_LEFT1, 
    GF_LEFT2,
    GF_UP1, 
    GF_UP2,
    GF_DOWN1, 
    GF_DOWN2,
};


const float SPRITESHEET_HEIGHT = 289;
const float SPRITESHEET_WIDTH  = 439;

const float FRAME_HEIGHT = 59;
const float FRAME_WIDTH  = 59;

const std::vector<float> PACMAN_UVS =  {
    10.0f,10.0f,   80.0f,10.0f,    150.0f,10.0f,    220.0f,10.0f,
    10.0f,80.0f,   80.0f, 80.0f,   150.0f,80.0f,    220.0f,80.0f,
    7.0f,152.0f,   79.0f,150.0f,   150.0f,150.0f,   220.0f,150.0f,
    10.f,222.0f,   81.0f,220.0f,   151.0f,220.0f,   220.0f,220.0f,
};

const std::vector<float> GHOST_UVS = {
    300.0f,10.0f,   370.0f,10.0f,
    300.0f,80.0f,   370.0f,80.0f,
    300.0f,150.0f,  370.0f,149.0f,
    300.0f,220.0f,  370.0f,220.0f,
};    

struct Pacman {
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    ost::PacmanFrame frame = ost::PacmanFrame::PF_RIGHT1;
};
} 