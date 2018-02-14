#pragma once
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"
#include "./Shader.hpp"
#include "./logger.h"


namespace ost
{
    using Grid = const std::vector<std::vector<int>>;

    enum TileTypes : int {
        FLOOR  = 0,
        WALL   = 1,
        PORTAL = 2,
        FOOD   = 3
    };

    using namespace glm;
    struct Level {

        const std::vector<std::vector<int>> grid;
        const std::vector<vec2> vertices;
        const ivec2 size;
        const int biggestSize;
        const float levelUnit;
        const mat4 scaleMatrix;
        const mat4 moveMatrix;


        Level(const std::vector<vec2> _vertices, const ivec2 _size, Grid _grid):
        vertices(_vertices),
        size(_size),
        grid(_grid),
        biggestSize((_size.x > _size.y) ? _size.x : _size.y),
        levelUnit(2.0f/(biggestSize)),
        scaleMatrix(
            mat4(
                levelUnit,  0,              0,      0,
                0,          levelUnit,      0,      0,
                0,          0,              1,      0,
                0,          0,              0,      1
            )
        ),
        moveMatrix(
            mat4(
                1,             0,           0,      0,
                0,             1,           0,      0,
                0,             0,           1,      0,
                -_size.x*0.5, -_size.y*0.5, 0,      1
            )
        )
        {
        }
        //
        // BUFFER COMPONENT
        //

        void bindBufferVertices(Mesh::Mesh mesh) const {
            auto vbo = mesh.VBO;
            auto vboindex = mesh.VBOindex;
            for(auto v : vertices) {
                (*vbo)[vboindex].position = v;
                vboindex += 1;
            }
        }

        static bool canWalkToward(const Grid& grid, glm::vec2 coordinate, glm::vec2 size, glm::ivec2 direction) 
        {
            const auto center = Level::getCenterPosition(coordinate, size);
            const auto gridIndex = glm::ivec2{center.x, center.y};

            const auto wantTile = grid[gridIndex.y +  direction.y ][gridIndex.x + direction.x];

        //    LOG_DEBUG("cx: %.2f cy: %.2f ix: %d, iy: %d  dx: %.2f, dy: %.2f, tt: %d , wt: %d", center.x, center.y, gridIndex.x, gridIndex.y, direction.x*0.5, direction.y*0.5, grid[gridIndex.y][gridIndex.x],  grid[gridIndex.y+direction.y][gridIndex.x+direction.x]);

            if (wantTile == ost::WALL) {
                if (Level::isCloseEnoughToTheMiddleOfTile(direction, gridIndex, center)) {
                    return false;
                } 
            } 
            return true;
        }

        static bool canChangeDirection(const Grid& grid, glm::vec2 coordinate, glm::vec2 size, glm::ivec2 direction, glm::ivec2 wantedDirection) 
        {
            const auto center = Level::getCenterPosition(coordinate, size);
            const auto gridIndex = glm::ivec2{center.x, center.y};

            if (!canWalkToward(grid, coordinate, size, wantedDirection)) {
                return false;
            }
            
            return (Level::isCloseEnoughToTheMiddleOfTile(direction, gridIndex, center));
        }


        static glm::vec2 getCenterPosition(glm::vec2 coordinate, glm::vec2 size) 
        {
            return glm::vec2{coordinate.x+(size.x*0.5f), coordinate.y-(size.y*0.5f)};   
        }

        static glm::vec2 getTileSnapPosition(glm::vec2 coordinate, glm::vec2 size) 
        {
            const auto center = getCenterPosition(coordinate, size);            
            return glm::vec2{ int(center.x), int(center.y)+1 };
        }

        static bool isCloseEnoughToTheMiddleOfTile(const glm::ivec2 direction, const glm::ivec2 gridIndex, const glm::vec2 center) 
        {
            const float margin = 0.4f;

#ifndef OPTIMIZE
            if (direction.y > 0 && direction.x > 0) {
                LOG_ERROR("OMG, direction.x and direction.y is both > 0. DIAGONAL movement not supported!");
            }

            if( direction.y == 0 && direction.x == 0){
                LOG_ERROR("ENTITY does not have any direction. Undefined behaviour!");                
            }
#endif

            if (direction.x) {
                // LOG_DEBUG("(center.x - gridIndex.x > margin && gridIndex.x+1 - center.x > margin) %d\n", (center.x - gridIndex.x > margin && gridIndex.x+1 - center.x > margin));                
                return abs(center.x - (gridIndex.x+0.5)) < 0.1;
            } else {
               // LOG_DEBUG("(center.y - gridIndex.y > margin && gridIndex.y+1 - center.y > margin) %d\n", (center.y - gridIndex.y > margin && gridIndex.y+1 - center.y > margin));
                return abs(center.y - (gridIndex.y+0.5) < 0.1);
            }

            LOG_ERROR("HOW DA FUCK DID YOU GET ALL THE WAY HERE !??????? ");            
        }
    };
}
