#pragma once
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"

namespace ost
{
    using namespace glm;
    struct Level {
        Level(const std::vector<vec2> _vertices, const ivec2 _size)
        :vertices(_vertices)
        {
            size = _size;
            biggestSize = (_size.x > _size.y) ? _size.x : _size.y;
        }
        //
        // BUFFER COMPONENT
        //
        const std::vector<vec2> vertices;

        //TODO consider changing vertices to ivec2 as they should be in world space units
        int biggestSize;
        ivec2 size = { 28, 32 };

        // std::vector<vec2> getBuffer() const {
        //     return vertices.data();
        // }

        // check if the coordinate is walkable floor
        bool isWalkable(const vec2 coordinate) {
            return std::find_if(
                vertices.begin(),
                vertices.end(),
                [coordinate](vec2 _lvlCoord){
                    return distance(_lvlCoord, coordinate) < 0.5f;
                }
            ) != vertices.end();
        }
    };

}
