#pragma once
#include <vector>
#include "glm/glm.hpp"

namespace ost {

const float SPRITESHEET_HEIGHT = 289.0f;
const float SPRITESHEET_WIDTH  = 439.0f; 
const float SW = SPRITESHEET_WIDTH;
const float SH = SPRITESHEET_HEIGHT;


const float FRAME_HEIGHT = 59.0f/SH;
const float FRAME_WIDTH  = 59.0f/SW;

enum PacmanFrame {
    PF_RIGHT0 = 0, 
    PF_RIGHT1 = 1,
    PF_RIGHT2 = 2, 
    PF_RIGHT3 = 3,
    PF_LEFT0, PF_LEFT1, PF_LEFT2, PF_LEFT3,
    PF_UP0, PF_UP1, PF_UP2, PF_UP3,
    PF_DOWN0, PF_DOWN1, PF_DOWN2, PF_DOWN3,
};

const std::vector<glm::vec2> PACMAN_TEX_COORD =  {
    {10.0f/SW,  10.0f/SH },   {80.0f/SW,10.0f/SH },    {150.0f/SW,10.0f/SH},    {220.0f/SW,10.0f/SH},
    {10.0f/SW,  80.0f/SH },   {80.0f/SW, 80.0f/SH },   {150.0f/SW,80.0f/SH},    {220.0f/SW,80.0f/SH},
    { 7.0f/SW, 152.0f/SH},  {79.0f/SW,150.0f/SH },   {150.0f/SW,150.0f/SH},   {220.0f/SW,150.0f/SH},
    {10.0f/SW, 222.0f/SH},  {81.0f/SW,220.0f/SH },   {151.0f/SW,220.0f/SH},   {220.0f/SW,220.0f/SH},
};



enum GhostFrame {
    GF_RIGHT1, GF_RIGHT2,
    GF_LEFT1, GF_LEFT2,
    GF_UP1, GF_UP2,
    GF_DOWN1, GF_DOWN2,
};
const std::vector<glm::vec2> GHOST_TEX_COORD = {
    {300.0f/SW,10.0f/SH},   {370.0f/SW,10.0f/SH},
    {300.0f/SW,80.0f/SH},   {370.0f/SW,80.0f/SH},
    {300.0f/SW,150.0f/SH},  {370.0f/SW,149.0f/SH},
    {300.0f/SW,220.0f/SH},  {370.0f/SW,220.0f/SH},
};    

struct Pacman {
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    int frame = (int)ost::PacmanFrame::PF_RIGHT0;
};

} 