#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "GLFW/glfw3.h"
#include "./macro.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GL/glew.h"

namespace ost 
{

struct Vertex 
{
    glm::vec2 position{};
    glm::vec4 color{};
    glm::vec2 texCoord{};
};

struct Shader 
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint program;

    GLint positionAttribute;
    GLint colorAttribute;
    GLint texcoordAttribute;

    GLenum drawMode;
    GLenum updateMode;
    GLuint maxVertexCount;
    GLuint maxElementCount;

    std::vector<Vertex>           vertexBuffer{};
    size_t                        vertexBufferCount{};

    std::vector<int>              elementBuffer{};
    size_t                        elementBufferCount{};
};

inline void
_bindVertexArrayAttributes(Shader& shader) {
    shader.positionAttribute = glGetAttribLocation(shader.program, "position");
    if (shader.positionAttribute == -1) {
        PANIC("shader.positionAttribute == -1");
    }
    shader.colorAttribute = glGetAttribLocation(shader.program, "color");
    if (shader.colorAttribute == -1) {
        PANIC("shader.colorAttribute == -1");
    }
    shader.texcoordAttribute = glGetAttribLocation(shader.program, "texcoord");
    if (shader.texcoordAttribute == -1) {
        PANIC("shader.texcoordAttribute == -1");
    }

    glVertexAttribPointer(shader.positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(shader.colorAttribute,    4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec2));
    glVertexAttribPointer(shader.texcoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec2)+sizeof(glm::vec4)));

    glEnableVertexAttribArray(shader.positionAttribute);
    glEnableVertexAttribArray(shader.colorAttribute);
    glEnableVertexAttribArray(shader.texcoordAttribute);
} 

//
// @function makeShader_VBO
//
inline Shader 
makeShader_VBO(const GLuint program, 
               const GLuint maxVertexCount, 
               const GLenum updateMode, // GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
               const GLenum drawMode    // GL_POINTS, GL_TRIANGLES
               ) 
{
    Shader shader = Shader{};
    shader.program = program;
    glUseProgram(shader.program);
    
    glGenVertexArrays(1, &(shader.vao));
    glGenBuffers(1, &(shader.vbo));

    // "The ordering doesn’t matter as long as you bind the VBO before using glBufferData and glBindVertexArray before you call glVertexAttribPointer."
    //  @doc http://headerphile.com/sdl2/opengl-part-2-vertexes-vbos-and-vaos/ - 12.02.18

    shader.drawMode       = drawMode;
    shader.updateMode     = updateMode;
    shader.maxVertexCount = maxVertexCount;

    glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);
    glBufferData(GL_ARRAY_BUFFER, shader.maxVertexCount * sizeof(Vertex), shader.vertexBuffer.data(), shader.updateMode);

    glBindVertexArray(shader.vao);
    _bindVertexArrayAttributes(shader);

    glBindVertexArray(0);
    glUseProgram(0);
    return shader;
}

//
// @function makeShader_VBO_EBO
//
inline Shader 
makeShader_VBO_EBO(const GLuint program, 
                   const GLuint maxVertexCount, 
                   const GLuint maxElementCount,
                   const GLenum updateMode, // GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
                   const GLenum drawMode    // GL_POINTS, GL_TRIANGLES
           ) 
{
    
    Shader shader = Shader{};
    shader.program = program;
    glUseProgram(shader.program);
    
    glGenVertexArrays(1, &(shader.vao));
    glGenBuffers(1, &(shader.vbo));
    glGenBuffers(1, &(shader.ebo));

    // "The ordering doesn’t matter as long as you bind the VBO before using glBufferData and glBindVertexArray before you call glVertexAttribPointer."
    //  @doc http://headerphile.com/sdl2/opengl-part-2-vertexes-vbos-and-vaos/ - 12.02.18

    shader.drawMode       = drawMode;
    shader.updateMode     = updateMode;
    shader.maxVertexCount = maxVertexCount;
    shader.maxElementCount = maxElementCount;

    glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);
    glBufferData(GL_ARRAY_BUFFER, shader.maxVertexCount * sizeof(Vertex), shader.vertexBuffer.data(), shader.updateMode);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shader.maxElementCount * sizeof(int), shader.elementBuffer.data(), shader.updateMode);

    glBindVertexArray(shader.vao);
    _bindVertexArrayAttributes(shader);

    glBindVertexArray(0);
    glUseProgram(0);
    return shader;
}


//
// @function draw_VBO
//
inline void 
draw_VBO(const Shader& shader) 
{
    glUseProgram(shader.program);   
    glBindVertexArray(shader.vao);

    glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, shader.vertexBuffer.size()*sizeof(Vertex), shader.vertexBuffer.data());
   
    glDrawArrays(shader.drawMode, 0, shader.vertexBuffer.size());

    glBindVertexArray(0);
    glUseProgram(0);
}

//
// @function draw_VBO_EBO
//
inline void 
draw_VBO_EBO(const Shader& shader) 
{
    glUseProgram(shader.program);   
    glBindVertexArray(shader.vao);

    glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, shader.vertexBuffer.size()*sizeof(Vertex), shader.vertexBuffer.data());
   
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader.ebo);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, shader.elementBuffer.size()*sizeof(int), shader.elementBuffer.data());
    glDrawElements(shader.drawMode, shader.elementBuffer.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

//
// @function getVertexBufferIt
//
inline const std::vector<Vertex>::iterator
getVertexBufferIt(Shader& shader, const size_t vertexCount)
{
    if (vertexCount + shader.vertexBuffer.size() > shader.maxVertexCount ) {
        PANIC("PREVENTED VERTEX BUFFER OVERFLOW");
    }
    shader.vertexBuffer.resize(shader.vertexBuffer.size() + vertexCount);
    auto it = shader.vertexBuffer.begin() + shader.vertexBufferCount;
    shader.vertexBufferCount += vertexCount;
    return it;
}

//
// @function getElementBufferIt
//
inline const std::vector<int>::iterator
getElementBufferIt(Shader& shader, const size_t elementCount) 
{
    if (elementCount + shader.elementBuffer.size() > shader.maxElementCount ) {
        PANIC("PREVENTED ELEMENT BUFFER OVERFLOW");
    }

    shader.elementBuffer.resize(shader.elementBuffer.size()+ elementCount);
    auto it = shader.elementBuffer.begin() + shader.elementBufferCount;
    shader.elementBufferCount += elementCount;
    return it;
}

//
// @function setUniformFloat
//
inline void 
setUniformFloat(const Shader& shader, const std::string uniname, const float univalue) 
{
    glUseProgram(shader.program);
    GLint uniform = glGetUniformLocation(shader.program, uniname.c_str());
    if (uniform == -1) {
        PANIC("UNIFORM == -1");
    }
    glUniform1f(uniform, univalue);
    glUseProgram(0);
}

//
// @function setUniformVec4
//
inline void 
setUniformVec4(const Shader& shader, const std::string uniname, const glm::vec4 univalue) 
{
    glUseProgram(shader.program);
    GLint uniform = glGetUniformLocation(shader.program, uniname.c_str());
    if (uniform == -1) {
        PANIC("UNIFORM == -1");
    }
    glUniform4fv(uniform, 1, glm::value_ptr(univalue));
    glUseProgram(0);    
}

inline void
setUniformMat4(const Shader& shader, const std::string uniname, const glm::mat4 univalue) {

    glUseProgram(shader.program);
    GLint uniform = glGetUniformLocation(shader.program, uniname.c_str());
    if (uniform == -1) {
        PANIC("UNIFORM == -1");
    }
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(univalue));
    glUseProgram(0);    
}

} // END NAMESPACE OST