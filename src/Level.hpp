#pragma once
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"
#include "./Shader.hpp"

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
        
        Level(const std::vector<vec2> _vertices, const ivec2 _size, const std::vector<std::vector<int>> _grid)
        :vertices(_vertices)
        ,grid(_grid)
        {
            size = _size;
            biggestSize = (_size.x > _size.y) ? _size.x : _size.y;

          //  for (auto v : vertices)
            //    std::cout << "v.x :  " << v.x  << "  v.y" << v.y << '\n';
        }
        //
        // BUFFER COMPONENT
        //
        const std::vector<vec2>             vertices;
        const std::vector<std::vector<int>> grid;

        void bindBufferVertices(const std::vector<Vertex>::iterator vertexBufferIterator) const {
            auto it = vertexBufferIterator;
            for(auto v : vertices) {
                (*it).position = v;
                it++;
            }
        }

        //TODO consider changing vertices to ivec2 as they should be in world space units
        int biggestSize;
        ivec2 size = { 28, 32 };

        // std::vector<vec2> getBuffer() const {
        //     return vertices.data();
        // }


        bool isWalkable(glm::vec2 coordinate, glm::ivec2 direction) {

            const int ix   = int(coordinate.x + (size.x/2.0f));
            const int iy   = size.y - int(coordinate.y + (size.y/2.0f));
            const int tile = grid[iy+direction.y][ix+direction.x];

            std::cout << "ix: " << ix << " iy: " << iy << " tile: " << tile << " \n";

            return (tile != ost::WALL);
        }
    };

}
