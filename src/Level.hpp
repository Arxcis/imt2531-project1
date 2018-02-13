#pragma once
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"
#include "./Shader.hpp"
#include "./logger.h"
namespace ost
{
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


        Level(const std::vector<vec2> _vertices, const ivec2 _size, const std::vector<std::vector<int>> _grid):
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

        void bindBufferVertices(const std::vector<Vertex>::iterator vertexBufferIterator) const {
            auto it = vertexBufferIterator;
            for(auto v : vertices) {
                (*it).position = v;
                it++;
            }
        }

        bool isWalkable(glm::vec2 coordinate, glm::vec2 size, glm::ivec2 direction) {

            glm::ivec2 gridCoordinate{coordinate.x+(size.x*0.5f), coordinate.y-(size.y*0.5f)};
            LOG_DEBUG("x: %d, y: %d  dx: %d, dy: %d, tt: %d , wt: %d", gridCoordinate.x, gridCoordinate.y, direction.x, direction.y,grid[gridCoordinate.y][gridCoordinate.x],  grid[gridCoordinate.y+direction.y][gridCoordinate.x+direction.x]);

            auto wantTile = grid[gridCoordinate.y+direction.y][gridCoordinate.x+direction.x];
            return wantTile != ost::WALL;
        }
    };

}
