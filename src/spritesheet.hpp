// name spritesheet.hpp
// author Jonas J. Solsvik
// date 06.02.2018
// license MIT

#pragma once
#include <vector>
#include <array>
#include <iomanip>
#include <iostream>
#include "glm/glm.hpp"

namespace ost{

struct Rect {
    glm::vec2 topleft;
    glm::vec2 topright;
    glm::vec2 botright;
    glm::vec2 botleft;
};

inline std::vector<ost::Rect>
makeSpriteUVCoordinates(const size_t columns,
                        const size_t rows,
                        const size_t count,
                              glm::vec2 start,
                        const glm::vec2 spritesheetSize,
                        const glm::vec2 textureSize) 
{
    std::vector<ost::Rect> result{};

    for (size_t y = 0; y < rows; ++y) {
        for (size_t x = 0; x < columns; ++x) {            

            ost::Rect rect{};

            rect.topleft = glm::vec2{
                (start.x + (x * (spritesheetSize.x / columns))) / textureSize.x,
                (start.y + (y * (spritesheetSize.y / rows   ))) / textureSize.y
            };

            const float normalWidth  = spritesheetSize.x  / columns / textureSize.x;
            const float normalHeight = spritesheetSize.y  / rows   / textureSize.y;

            rect.topright = glm::vec2{
                rect.topleft.x + normalWidth, 
                rect.topleft.y
            };

            rect.botleft = glm::vec2{
                rect.topleft.x, 
                rect.topleft.y + normalHeight
            };

            rect.botright = glm::vec2{
                rect.topleft.x + normalWidth, 
                rect.topleft.y + normalHeight
            };

            result.push_back(rect);
            
            if ((y * columns + x) >= count-1) {
                return result;
            }
        }
    }
    assert(false && "SHOULD NEVER HAPPEN!!!!!!");
    return result;
};

inline void 
test_makeSpriteUVCoordinates(){
    const std::vector<ost::Rect> pacmanSprites = makeSpriteUVCoordinates(4,4,16, {5.5f, 6.0f}, 
                                                                                 {278.0f, 278.0f},
                                                                                 {439.0f, 289.0f}); 
    const std::vector<ost::Rect> ghostSprites = makeSpriteUVCoordinates(2,4,8, {295.0f, 6.0f},
                                                                               {144.0f, 278.0f},
                                                                               {439.0f, 289.0f});

    auto printSprites = [](const std::vector<ost::Rect> sprites) {
        
        for (const auto& rect : sprites) {
            std::cout << std::setprecision(5) 
                      << "top left:  "   << std::setw(8)  <<rect.topleft.x  << ", " << std::setw(8) << rect.topleft.y << ")    " 
                      << "top right: "   << std::setw(8)  <<rect.topright.x << ", " << std::setw(8) << rect.topright.y << ")    " 
                      << "bot right:  "  << std::setw(8)  <<rect.botright.x << ", " << std::setw(8) << rect.botright.y << ")    " 
                      << "bot left: "    << std::setw(8)  <<rect.botleft.x  << ", " << std::setw(8) << rect.botleft.y << ")\n"; 
        }
    };

    std::cout << "\n------- PACMAN --------\n";
    printSprites(pacmanSprites);
    std::cout << "\n------- GHOST --------\n";
    printSprites(ghostSprites);
}

} // end namespace ost{}