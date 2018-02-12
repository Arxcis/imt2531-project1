#pragma once

#include <vector>
#include <string>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"


struct Mesh {

    struct Vertex {
        glm::vec2 position;
        glm::vec4 color;
        glm::vec2 textCoord;
    };

  public:
    std::vector<Vertex> vertices;
    GLuint texture;
    GLuint shaderProgram;
    GLuint drawType;

  private:
    GLuint vao;
    
    GLuint vbo;
    GLuint vboType = GL_ARRAY_BUFFER;
    GLuint vboUpdate = GL_STREAM_DRAW;

    GLuint ebo;
    GLuint eboType = GL_ELEMENT_ARRAY_BUFFER;
    GLuint eboUpdate = GL_STREAM_DRAW;

    std::string attrPositionString;
    std::string attrColorString;
    std::string attrTextureString;

    GLuint attrPosition;
    GLuint attrColor;
    GLuint attrTexture;
};

inline Mesh* makeMesh() {

    auto mesh = new Mesh{};


    return mesh;
}