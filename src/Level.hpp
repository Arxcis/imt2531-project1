#pragma once
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"
#include "./Shader.hpp"

namespace ost
{
    enum TileTypes : int {
        FLOOR = 0,
        WALL = 1,
        PORTAL = 2,
        FOOD = 3
    };


    using namespace glm;
    struct Level {

        Level(const std::vector<vec2> _vertices, const ivec2 _size):
        vertices(_vertices),
        size(_size),
        biggestSize((_size.x > _size.y) ? _size.x : _size.y),
        levelUnit(2.0f/biggestSize),
        viewMatrix(
            mat4(levelUnit, 0,              0,      0,
                0,          levelUnit,      0,      0,
                0,          0,              1,      0,
                0,          0,              0,      1
            )
        )
        {
            printf("LEVEL DEBUG INFO:\n");
            printf("size: (x:%d,y:%d)\n", size.x, size.y);
            printf("biggestSize: %d\n", biggestSize);
            printf("levelUnit: %.4f\n", levelUnit);
            printf("viewMatrix: \n%.4f,%.4f,%.4f,%.4f\n%.4f,%.4f,%.4f,%.4f\n%.4f,%.4f,%.4f,%.4f\n%.4f,%.4f,%.4f,%.4f\n",
                viewMatrix[0][0],viewMatrix[0][1],viewMatrix[0][2],viewMatrix[0][3],
                viewMatrix[1][0],viewMatrix[1][1],viewMatrix[1][2],viewMatrix[1][3],
                viewMatrix[2][0],viewMatrix[2][1],viewMatrix[2][2],viewMatrix[2][3],
                viewMatrix[3][0],viewMatrix[3][1],viewMatrix[3][2],viewMatrix[3][3]
            );
        }
        //
        // BUFFER COMPONENT
        //
        const std::vector<std::vector<int>> grid;

        void bindBufferVertices(const std::vector<Vertex>::iterator vertexBufferIterator) const {
            auto it = vertexBufferIterator;
            for(auto v : vertices) {
                (*it).position = v;
                it++;
            }
        }

        const std::vector<vec2> vertices;
        const ivec2 size;
        const int biggestSize;
        const float levelUnit;
        const mat4 viewMatrix;

        // check if the coordinate is walkable floor
        bool isWalkable(const vec2 coordinate) const {
            return std::find_if(
                vertices.begin(),
                vertices.end(),
                [coordinate](vec2 _lvlCoord){
                    return distance(_lvlCoord, coordinate) < 0.8f;
                }
            ) != vertices.end();
        }
    };

}
