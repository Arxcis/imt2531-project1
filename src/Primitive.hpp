#pragma once

#include <vector>

#include "./spritesheet.hpp"
#include "./Shader.hpp"
#include "./Logger.hpp"

namespace Primitive {

struct Text 
{
    Mesh::Mesh             mesh;
    glm::vec2              pos;
    std::vector<ost::Rect> uv;
    std::string            text;
    glm::vec2              size  = { 1.0f, 1.0f };

    Text() = default;    
    Text& operator=(const Text& other) = default;

    Text(Mesh::Mesh _mesh, glm::vec2 _pos, glm::vec2 _size, std::vector<ost::Rect> _uv, std::string _text)
    :mesh(_mesh)
    ,pos(_pos)
    ,size(_size)
    ,uv(_uv)
    ,text(_text)
    {
        print();
    }

    void print() 
    {
        auto step = glm::vec2{0.1, 0};
        auto letterPosition = pos;
        size_t i = 0;
        const float margin = 0.01f;

        for(auto t: text) {
            Mesh::bindRect(mesh, pos+(glm::vec2{size.x+margin, 0}*float(i)), size, uv[t], i);
            ++i;
        }
    }
};

}